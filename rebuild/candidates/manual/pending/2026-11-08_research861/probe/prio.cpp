// MSVC5 claim: locals live across calls are coloured esi, edi, ebx, ebp in priority (weighted ref count) order; ties go to
// the value that appears first. Check VC7.1.
int f(int); void g(int, int, int);
extern int G[8];
int p1_a3_b2(int x) { int a = f(x); int b = f(x + 1); g(a, b, 0); g(a, 0, 0); g(a, 0, 0); return b; }      // a 4 refs, b 2
int p2_a2_b3(int x) { int a = f(x); int b = f(x + 1); g(a, b, 0); g(b, 0, 0); g(b, 0, 0); return a; }      // b 4 refs, a 2
int p3_tie_ab(int x) { int a = f(x); int b = f(x + 1); g(a, b, 0); g(a, b, 0); return 0; }                 // tie: a first
int p4_tie_ba(int x) { int b = f(x + 1); int a = f(x); g(a, b, 0); g(a, b, 0); return 0; }                 // tie: b first
int p5_three(int x) { int a = f(x); int b = f(x + 1); int c = f(x + 2); g(a, b, c); g(a, b, c); g(c, c, c); return a; }  // c highest
int p6_four(int x) { int a = f(x); int b = f(x + 1); int c = f(x + 2); int d = f(x + 3); g(a, b, c); g(d, d, d); g(c, c, b); g(a, a, a); g(a, b, 0); return d; }
int p7_loop(int x) { int a = f(x); int b = f(x + 1); for (int i = 0; i < x; ++i) g(b, i, 0); g(a, a, a); g(a, a, a); return 0; }  // b in loop (2 refs x loop) vs a 7 refs
int p8_leaf_order(int* p) { int a = p[0]; int b = p[1]; int c = p[2]; G[0] = a * b + a * c + a; G[1] = b + c; G[2] = c; return a + b + c; }
