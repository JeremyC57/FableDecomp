#include <cstdio>
#include <cstring>
struct S { void* vt; unsigned long pos; long a; const unsigned char* p; long b; long rem; static int g_o; void O(unsigned char*, long) { ++g_o; }
  void R(void* buf, long n) { __int64 np = (__int64)(pos + n); if (!(np >= 0 && np <= 0x7fffffff)) return; if (rem >= n) { std::memcpy(buf, p, n); p += n; rem -= n; pos += n; } else O((unsigned char*)buf, n); } };
int S::g_o = 0;
static void __stdcall T(S* s, long* o) { long v; s->R(&v, 4); *o = v; }
int main()
{
    unsigned char data[8] = { 1, 0, 0, 0, 2, 0, 0, 0 }; S s; s.pos = 0; s.p = data; s.rem = 8; long v = 0; T(&s, &v); if (v != 1 || s.pos != 4 || s.rem != 4 || s.p != data + 4) { std::printf("00632900_TEST FAIL\n"); return 1; }
    s.rem = 2; T(&s, &v); if (S::g_o != 1) { std::printf("00632900_TEST FAIL\n"); return 1; }

    std::printf("00632900_TEST PASS\n");
    return 0;
}
