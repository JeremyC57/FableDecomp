#include <cstdio>
struct E { long v; }; static void __fastcall C(E* w, const E& v) { w->v = v.v; }
static E* __fastcall F(E* f, unsigned long n, const E& v) { for (; 0 < n; --n, ++f) C(f, v); return f; }
int main()
{
    E v; v.v = 3; E buf[2]; if (F(buf, 2, v) != buf + 2 || buf[1].v != 3) { std::printf("00469149_TEST FAIL\n"); return 1; }

    std::printf("00469149_TEST PASS\n");
    return 0;
}
