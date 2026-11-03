#include <stdlib.h>
extern const long g_opts[4];
struct C { char pad[0x4c]; unsigned long R;
  void w1(long rate); void Sel(unsigned long& best, long rate); void w3(long rate); void w4(long rate); void w5(long rate);
};
void C::Sel(unsigned long& best, long rate) { best = 0; for (unsigned long i = 1; i < 4; ++i) { if (abs(rate - g_opts[i]) < abs(rate - g_opts[best]) && g_opts[i] <= rate) best = i; } }
void C::w1(long rate) { Sel(R, rate); }
void C::w3(long rate) { R = 0; for (unsigned long i = 1; i != 4; ++i) { if (abs(rate - g_opts[i]) < abs(rate - g_opts[R]) && g_opts[i] <= rate) R = i; } }
void C::w4(long rate) { R = 0; for (unsigned long i = 1; i < 4; ++i) { if (i >= 0 && abs(rate - g_opts[i]) < abs(rate - g_opts[R]) && g_opts[i] <= rate) R = i; } }
void C::w5(long rate) { R = 0; for (unsigned long i = 1; i < sizeof(g_opts)/sizeof(g_opts[0]); ++i) { const long d = rate - g_opts[i]; if (abs(d) < abs(rate - g_opts[R]) && g_opts[i] <= rate) R = i; } }
