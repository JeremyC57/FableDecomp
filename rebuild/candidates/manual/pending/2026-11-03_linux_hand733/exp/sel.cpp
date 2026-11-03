#include <stdlib.h>
extern const long g_opts[4];
struct C { char pad[0x4c]; unsigned long R;
  void s1(long rate); void s2(long rate); void s3(long rate); void s4(long rate); void s5(long rate); void s6(long rate);
};
void C::s1(long rate) { R = 0; for (unsigned long i = 1; i < 4; ++i) { if (abs(rate - g_opts[i]) < abs(rate - g_opts[R]) && g_opts[i] <= rate) R = i; } }
void C::s2(long rate) { R = 0; for (unsigned long i = 1; i < 4; ++i) { if (abs(rate - g_opts[R]) > abs(rate - g_opts[i]) && g_opts[i] <= rate) R = i; } }
void C::s3(long rate) { R = 0; for (unsigned long i = 1; i < 4; ++i) { long d = abs(rate - g_opts[i]); if (d < abs(rate - g_opts[R]) && g_opts[i] <= rate) R = i; } }
void C::s4(long rate) { R = 0; for (unsigned long i = 1; i < 4; ++i) { if (abs(rate - g_opts[i]) < abs(rate - g_opts[R])) { if (g_opts[i] <= rate) R = i; } } }
void C::s5(long rate) { R = 0; for (unsigned long i = 0; i < 4; ++i) { if (abs(rate - g_opts[i]) < abs(rate - g_opts[R]) && g_opts[i] <= rate) R = i; } }
void C::s6(long rate) { R = 0; unsigned long i = 1; while (i < 4) { if (abs(rate - g_opts[i]) < abs(rate - g_opts[R]) && g_opts[i] <= rate) R = i; ++i; } }
