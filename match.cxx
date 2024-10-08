/* match.c -- pattern matching routines ($Revision: 1.1.1.1 $) */

#include "es.hxx"

enum { RANGE_FAIL = -1, RANGE_ERROR = -2 };

/*
   From the ed(1) man pages (on ranges):

	The `-' is treated as an ordinary character if it occurs first
	(or first after an initial ^) or last in the string.

	The right square bracket does not terminate the enclosed string
	if it is the first character (after an initial `^', if any), in
	the bracketed string.

   rangematch() matches a single character against a class, and returns
   an integer offset to the end of the range on success, or -1 on
   failure.
*/

#define	ISQUOTED(q, n)	((q) == QUOTED || ((q) != UNQUOTED && (q)[n] == 'q'))
#define TAILQUOTE(q, n) ((q) == UNQUOTED ? UNQUOTED : ((q) + (n)))

/* rangematch -- match a character against a character class */
static int rangematch(const char *p, const char *q, char c) {
	const char *orig = p;
	bool neg;
	bool matched = false;
	if (*p == '~' && !ISQUOTED(q, 0)) {
		p++, q++;
	    	neg = true;
	} else
		neg = false;
	if (*p == ']' && !ISQUOTED(q, 0)) {
		p++, q++;
		matched = (c == ']');
	}
	for (; *p != ']' || ISQUOTED(q, 0); p++, q++) {
		if (*p == '\0')
			return RANGE_ERROR;	/* bad syntax */
		if (p[1] == '-' && !ISQUOTED(q, 1) && ((p[2] != ']' && p[2] != '\0') || ISQUOTED(q, 2))) {
			/* check for [..-..] but ignore [..-] */
			if (c >= *p && c <= p[2])
				matched = true;
			p += 2;
			q += 2;
		} else if (*p == c)
			matched = true;
	}
	if (matched ^ neg)
		return p - orig + 1; /* skip the right-bracket */
	else
		return RANGE_FAIL;
}

/* match -- match a single pattern against a single string. */
extern bool match(const char *s, const char *p, const char *q) {
	if (q == QUOTED) return streq(s, p);
	for (int i = 0 ;;) {
		int c = p[i++];
		if (c == '\0')
			return *s == '\0';
		else if (q == UNQUOTED || q[i - 1] == 'r') {
			switch (c) {
			case '?':
				if (*s++ == '\0') return false;
			case '*':
				while (p[i] == '*' && (q == UNQUOTED || q[i] == 'r'))	// collapse multiple stars
					i++;
				if (p[i] == '\0') 	/* star at end of pattern? */
					return true;
				while (*s != '\0')
					if (match(s++, p + i, TAILQUOTE(q, i)))
						return true;
				return false;
			case '[': {
				if (*s == '\0')
					return false;
				int j;
				switch (j = rangematch(p + i, TAILQUOTE(q, i), *s)) {
				default:
					i += j;
					break;
				case RANGE_FAIL:
					return false;
				case RANGE_ERROR:
					if (*s != '[')
						return false;
				}
				s++;
				break;
			}
			default:
				if (c != *s++)
					return false;
			}
		} else if (c != *s++)
			return false;
	}
}


/*
 * listmatch
 *
 * 	Matches a list of words s against a list of patterns p.
 *	Returns true iff a pattern in p matches a word in s.
 *	() matches (), but otherwise null patterns match nothing.
 */

extern bool listmatch(List* subject, List* pattern, StrList* quote) {
	if (subject == NULL) {
		if (pattern == NULL)
			return true;
		for (; pattern; pattern = pattern->next, quote = quote->next) {
			/* one or more stars match null */
			const char *pw = getstr(pattern->term), *qw = quote->str;
			if (*pw != '\0' && qw != QUOTED) {
				int i;
				bool matched = true;
				for (i = 0; pw[i] != '\0'; i++)
					if (pw[i] != '*'
					    || (qw != UNQUOTED && qw[i] != 'r')) {
						matched = false;
						break;
					}
				if (matched) {
					return true;
				}
			}
		}
		return false;
	} else {
		for (; pattern; pattern = pattern->next, quote = quote->next) {
			assert(quote != NULL);
			assert(pattern->term != NULL);
			assert(quote->str != NULL);
			const char* pw = getstr(pattern->term);
			const char* qw = quote->str;
			List* t = subject;
			for (; t != NULL; t = t->next) {
				const char* tw = getstr(t->term);
				if (match(tw, pw, qw)) {
					return true;
				}
			}
		}
		return false;
	}
}

/*
 * extractsinglematch -- extract matching parts of a single subject and a
 *			 single pattern, returning it backwards
 */

static List *extractsinglematch(const char *subject, const char *pattern,
				const char *quoting, List *result) {
	int i;
	const char *s;

	if (!haswild(pattern, quoting) /* no wildcards, so no matches */
	    || !match(subject, pattern, quoting))
		return NULL;

	for (s = subject, i = 0; pattern[i] != '\0'; s++) {
		if (ISQUOTED(quoting, i))
			i++;
		else {
			int c = pattern[i++];
			switch (c) {
			    case '*': {
				const char *begin;
				if (pattern[i] == '\0')
					return mklist(mkstr(gcdup(s)), result);
				for (begin = s;; s++) {
					const char *q = TAILQUOTE(quoting, i);
					assert(*s != '\0');
					if (match(s, pattern + i, q)) {
						result = mklist(mkstr(gcndup(begin, s - begin)), result);
						return haswild(pattern + i, q)
							? extractsinglematch(s, pattern + i, q, result)
							: result;
					}
				}
			    }
			    case '[': {
				int j = rangematch(pattern + i, TAILQUOTE(quoting, i), *s);
				assert(j != RANGE_FAIL);
				if (j == RANGE_ERROR) {
					assert(*s == '[');
					break;
				}
				i += j;
			    }
			    /* FALLTHROUGH */
			    case '?':
				result = mklist(mkstr(str("%c", *s)), result);
				break;
			    default:
				break;
			}
		}
	}

	return result;
}

/*
 * extractmatches
 *
 *	Compare subject and patterns like listmatch().  For all subjects
 *	that match a pattern, return the wildcarded portions of the
 *	subjects as the result.
 */

extern List *extractmatches(List *subjects, List *patterns, StrList *quotes) {
	List *result;
	List **prevp = &result;

	for (List *subject = subjects; subject != NULL; subject = subject->next) {
		List *pattern;
		StrList *quote;
		for (pattern = patterns, quote = quotes;
		     pattern != NULL;
		     pattern = pattern->next, quote = quote->next) {
			List *match;
			const char *pat = getstr(pattern->term);
			match = extractsinglematch(getstr(subject->term),
						   pat, quote->str, NULL);
			if (match != NULL) {
				/* match is returned backwards, so reverse it */
				match = reverse(match);
				for (*prevp = match; match != NULL; match = *prevp)
					prevp = &match->next;
				break;
			}
		}
	}

	
	return result;
}
