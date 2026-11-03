// p10: is the cost pre- or post-optimisation? Variants that compile to the same code.
void Mark(int); extern int G[64];
inline void Y(int a) { G[0] = a; G[1] = a + 1; G[2] = a + 2; Mark(a); }
inline void Ydead(int a) { int t = a * 3; int u = t + 7; G[0] = a; G[1] = a + 1; G[2] = a + 2; Mark(a); }
inline void Yredundant(int a) { G[0] = 5; G[0] = a; G[1] = a + 1; G[1] = a + 1; G[2] = a + 2; Mark(a); }
inline void Ybytes(int a) { unsigned char* p = (unsigned char*)&G[0]; p[0] = (unsigned char)a; p[1] = (unsigned char)(a >> 8); p[2] = (unsigned char)(a >> 16); p[3] = (unsigned char)(a >> 24); G[1] = a + 1; G[2] = a + 2; Mark(a); }
inline void Ytemp(int a) { int x = a, y = a + 1, z = a + 2; G[0] = x; G[1] = y; G[2] = z; Mark(a); }
struct V { int x, y, z; V(int a, int b, int c) : x(a), y(b), z(c) {} };
inline void Ystruct(int a) { V v(a, a + 1, a + 2); G[0] = v.x; G[1] = v.y; G[2] = v.z; Mark(a); }
void uY(int a) { Y(a); }
void uYdead(int a) { Ydead(a); }
void uYredundant(int a) { Yredundant(a); }
void uYbytes(int a) { Ybytes(a); }
void uYtemp(int a) { Ytemp(a); }
void uYstruct(int a) { Ystruct(a); }
