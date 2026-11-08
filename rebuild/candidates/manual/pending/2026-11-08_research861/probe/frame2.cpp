// Same as frame.cpp fr3/fr4 but the callee has a visible same-TU body under auto_inline(off) (chunk 860 lever).
extern int gsrc;
#pragma auto_inline(off)
void take(int* p) { *p = gsrc; }
#pragma auto_inline(on)
int fr3v() { int s = 0; { int a; take(&a); s += a; } { int b; take(&b); s += b * 2; } { int c; take(&c); s += c * 3; } return s; }
int fr4v() { int s = 0; int a; take(&a); s += a; int b; take(&b); s += b * 2; int c; take(&c); s += c * 3; return s; }
