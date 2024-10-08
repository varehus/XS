/* syntax.h -- abstract syntax tree interface ($Revision: 1.1.1.1 $) */

#define	CAR	u[0].p
#define	CDR	u[1].p


/* syntax.c */

extern Tree errornode;

extern Tree *treecons(Tree *car, Tree *cdr);
extern Tree *treecons2(Tree *car, Tree *cdr);
extern Tree *treeconsend(Tree *p, Tree *q);
extern Tree *treeconsend2(Tree *p, Tree *q);
extern Tree *treeappend(Tree *head, Tree *tail);
extern Tree *thunkify(Tree *tree);

extern Tree *prefix(const char *s, Tree *t);
extern Tree *backquote(Tree *ifs, Tree *body);
extern Tree *flatten(Tree *t, const char *sep);
extern Tree *fnassign(Tree *name, Tree *defn);
extern Tree *mklambda(Tree *params, Tree *body);
extern Tree *mkseq(const char *op, Tree *t1, Tree *t2);
extern Tree *mkpipe(Tree *t1, int outfd, int infd, Tree *t2);

extern Tree *mkclose(int fd);
extern Tree *mkdup(int fd0, int fd1);
extern Tree *redirect(Tree *t);
extern Tree *mkredir(Tree *cmd, Tree *file);
extern Tree *mkredircmd(const char *cmd, int fd);
extern Tree *redirappend(Tree *t, Tree *r);


/* heredoc.c */

extern bool readheredocs(bool endfile);
extern bool queueheredoc(Tree *t);
