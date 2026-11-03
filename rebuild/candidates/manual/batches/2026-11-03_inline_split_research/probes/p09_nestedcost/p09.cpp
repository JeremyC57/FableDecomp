// p09: is a callee's cost measured after its own inline expansions? X1 = Y once, X3 = Y three times.
void Mark(int); extern int G[64];
inline void Y(int a) { G[0] = a; G[1] = a + 1; G[2] = a + 2; Mark(a); }
inline void X1(int a) { Y(a); }
inline void X3(int a) { Y(a); Y(a + 1); Y(a + 2); }
inline void Z3(int a) { G[0] = a; G[1] = a + 1; G[2] = a + 2; Mark(a); G[0] = a+1; G[1] = a + 2; G[2] = a + 3; Mark(a+1); G[0] = a+2; G[1] = a + 3; G[2] = a + 4; Mark(a+2); }
void uX1(int a) { X1(a); }
void uX3(int a) { X3(a); }
void uZ3(int a) { Z3(a); }
void uY(int a) { Y(a); }
