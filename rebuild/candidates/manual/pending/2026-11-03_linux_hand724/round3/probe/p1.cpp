struct X { unsigned long a:11; unsigned long b:14; };
float g1, g2, g3, g4, g5, g6;
void f1(X& x) { g1 = x.a / 2047.0f * 20.0f; }
void f2(X& x) { g2 = 20.0f * (x.a / 2047.0f); }
void f3(X& x) { float n = x.a / 2047.0f; g3 = n * 20.0f; }
void f4(X& x) { g4 = (x.a * (1.0f / 2047.0f)) * (20.0f - 0.0f) + 0.0f; }
inline float Conv(unsigned long v, unsigned long maxv, float mn, float mx) { return (float)v / (float)maxv * (mx - mn) + mn; }
void f5(X& x) { g5 = Conv(x.a, 2047, 0.0f, 20.0f); }
void f6(X& x) { g6 = x.b / 16383.0f * 99.9f + 0.1f; }
