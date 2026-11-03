#include <stdlib.h>
extern const long g_opts[4];
struct C { char pad[0x4c]; unsigned long R;
  void x1(long rate); void x2(long rate); void x3(long rate);
};
void C::x1(long rate) { R = 0; for (long i = 1; i < sizeof(g_opts)/sizeof(g_opts[0]); ++i) { if (abs(rate - g_opts[i]) < abs(rate - g_opts[R]) && g_opts[i] <= rate) R = i; } }
void C::x2(long rate) { R = 0; for (unsigned long i = 1; i < 4; ++i) { if (abs(rate - g_opts[i]) < abs(rate - g_opts[R]) && g_opts[i] <= rate && R >= 0) R = i; } }
void C::x3(long rate) { R = 0; for (long i = 1; i < 4u; ++i) { if (abs(rate - g_opts[i]) < abs(rate - g_opts[R]) && g_opts[i] <= rate) R = i; } }
