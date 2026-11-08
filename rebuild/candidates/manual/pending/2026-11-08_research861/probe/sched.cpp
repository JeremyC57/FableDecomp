// Scheduler ties: `this` copy into a callee-saved register vs a parameter load; which goes first depends on what?
struct H { int a, b, c; };
int f(int); int g(H*, int);
struct C { int m0, m1, m2; H h;
  int s1(int x, int y);  int s2(int x, int y); int s3(int x, int y); int s4(int x, int y); int s5(int x, int y); int s6(int x, int y); };
// s1: `this` used first (m0), then x, then y, all live across calls
int C::s1(int x, int y) { int t = f(m0); t += f(x); t += f(y); return t + m1 + x + y; }
// s2: x used first
int C::s2(int x, int y) { int t = f(x); t += f(m0); t += f(y); return t + m1 + x + y; }
// s3: y used first
int C::s3(int x, int y) { int t = f(y); t += f(x); t += f(m0); return t + m1 + x + y; }
// s4: this used only late
int C::s4(int x, int y) { int t = f(x); t += f(y); t += f(x + y); return t + m1 + x + y; }
// s5: a load from this before the first call, this live across
int C::s5(int x, int y) { int t = m2; t += f(x); t += f(y); return t + m1 + x + y; }
// s6: param used via memory only once (not live across) vs this
int C::s6(int x, int y) { int t = f(m0 + x); t += f(m1); return t + m2 + y; }
