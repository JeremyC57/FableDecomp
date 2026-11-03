#include <stdlib.h>
extern const long g_opts[4];
#define ABS(x) ((x) < 0 ? -(x) : (x))
template<class T> inline T TAbs(T x) { return x < 0 ? -x : x; }
template<class T> inline T TAbs2(T x) { if (x < 0) return -x; return x; }
struct C { char pad[0x4c]; unsigned long R;
  void u1(long rate); void u2(long rate); void u3(long rate); void u4(long rate); void u5(long rate);
};
void C::u1(long rate) { R = 0; for (unsigned long i = 1; i < 4; ++i) { if (ABS(rate - g_opts[i]) < ABS(rate - g_opts[R]) && g_opts[i] <= rate) R = i; } }
void C::u2(long rate) { R = 0; for (unsigned long i = 1; i < 4; ++i) { if (labs(rate - g_opts[i]) < labs(rate - g_opts[R]) && g_opts[i] <= rate) R = i; } }
void C::u3(long rate) { R = 0; for (unsigned long i = 1; i < 4; ++i) { if (TAbs(rate - g_opts[i]) < TAbs(rate - g_opts[R]) && g_opts[i] <= rate) R = i; } }
void C::u4(long rate) { R = 0; for (unsigned long i = 1; i < 4; ++i) { if (TAbs2(rate - g_opts[i]) < TAbs2(rate - g_opts[R]) && g_opts[i] <= rate) R = i; } }
void C::u5(long rate) { R = 0; for (unsigned long i = 1; i < 4; ++i) { if (abs(g_opts[i] - rate) < abs(g_opts[R] - rate) && g_opts[i] <= rate) R = i; } }
