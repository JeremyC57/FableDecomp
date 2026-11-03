// p17: callee defined after the caller -- is its cost the raw (pre-inlining) size?
void Mark(int); extern int G[64];
inline void Y(int a) { G[0] = a; G[1] = a + 1; G[2] = a + 2; Mark(a); }
inline void X3(int a);
void uX3_before_def(int a) { X3(a); }
inline void X3(int a) { Y(a); Y(a + 1); Y(a + 2); }
void uX3_after_def(int a) { X3(a); }
struct C { void M(int a); void N(int a) { Y(a); Y(a + 1); Y(a + 2); } };
void uN(C& c, int a) { c.N(a); }
