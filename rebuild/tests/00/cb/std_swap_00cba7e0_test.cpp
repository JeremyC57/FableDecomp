// Test: std::swap<pair<unsigned __int64, CCountedPointer<CActiveEntityScriptBase>>> -- key and counted pointer exchange, and the shared count is back where it started.
#include <cstdio>
struct CObj { int refs; };
struct CCounted { CObj* p; CCounted(CObj* o) : p(o) { if (p) ++p->refs; } CCounted(const CCounted& o) : p(o.p) { if (p) ++p->refs; }
    ~CCounted() { if (p) --p->refs; } CCounted& operator=(const CCounted& o) { if (o.p) ++o.p->refs; if (p) --p->refs; p = o.p; return *this; } };
struct SPair { unsigned int lo, hi; CCounted second; SPair(unsigned int l, unsigned int h, CObj* o) : lo(l), hi(h), second(o) { } };
template <class T> void swapT(T& a, T& b) { T tmp = a; a = b; b = tmp; }
int main()
{
    CObj x, y; x.refs = 0; y.refs = 0;
    { SPair a(1, 2, &x), b(3, 4, &y); swapT(a, b);
      bool ok = a.lo == 3 && a.hi == 4 && a.second.p == &y && b.lo == 1 && b.second.p == &x && x.refs == 1 && y.refs == 1;
      if (!ok) { std::printf("00cba7e0_TEST FAIL\n"); return 1; } }
    bool ok = x.refs == 0 && y.refs == 0;
    std::printf(ok ? "00cba7e0_TEST PASS\n" : "00cba7e0_TEST FAIL\n");
    return ok ? 0 : 1;
}