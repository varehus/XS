/* prim.c -- primitives and primitive dispatching ($Revision: 1.1.1.1 $) */

#include "es.hxx"
#include "prim.hxx"

static Prim_dict prims;
#include <iostream>

extern const List* 
    prim(const char *s, List* list, Binding* binding, int evalflags) {
	Prim p = prims[s];
	if (p) return (*prims[s])(list, binding, evalflags);
	else fail("es:prim", "unknown primitive: %s", s);
}

PRIM(primitives) {
	static List *primlist = NULL;
	if (primlist == NULL) {
		for (Prim_dict::iterator i = prims.begin(); i != prims.end(); ++i) {
			Term* term = mkstr(i->first.c_str());
			primlist = mklist(term, primlist);
		}
	}
	return primlist;
}

extern void initprims(void) {
	initprims_controlflow(prims);
	initprims_io(prims);
	initprims_etc(prims);
	initprims_sys(prims);
	initprims_proc(prims);
	initprims_access(prims);

#define	primdict prims
	X(primitives);
}
