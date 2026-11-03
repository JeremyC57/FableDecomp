#define FABLE_RETAIL_CCHARSTRING
#include "fable_string.h"
extern CCharString g_s;
struct CCS2 { CCharStringData* PStringData;
  long Len1() const { return PStringData ? (long)PStringData->Data.GetLength() : 0; }
  long Len2() const { if (!PStringData) return 0; return (long)PStringData->Data.GetLength(); }
  long Len3() const { long l = 0; if (PStringData) l = PStringData->Data.GetLength(); return l; }
  long Len4() const { return !PStringData ? 0 : (long)PStringData->Data.GetLength(); }
};
extern CCS2 g_t;
void use(long, long*);
void f1() { long a; long n = g_t.Len1(); a = 0; use(n, &a); }
void f2() { long a; long n = g_t.Len2(); a = 0; use(n, &a); }
void f3() { long a; long n = g_t.Len3(); a = 0; use(n, &a); }
void f4() { long a; long n = g_t.Len4(); a = 0; use(n, &a); }
long g1(CCS2* b, CCS2* e, long lim) { long c=0; for (; b != e; ++b) if (b->Len1() >= lim) use(c++, 0); return c; }
long g2(CCS2* b, CCS2* e, long lim) { long c=0; for (; b != e; ++b) if (b->Len2() >= lim) use(c++, 0); return c; }
long g4(CCS2* b, CCS2* e, long lim) { long c=0; for (; b != e; ++b) if (b->Len4() >= lim) use(c++, 0); return c; }
