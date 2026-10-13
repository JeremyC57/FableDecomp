#include <cstdio>
struct Q { virtual void v00() {} virtual bool E() { return on; } bool on; Q* Self() { return E() ? this : 0; } };
int main()
{
    Q q; q.on = true; if (q.Self() != &q) { std::printf("00c33cd0_TEST FAIL\n"); return 1; }
    q.on = false; if (q.Self()) { std::printf("00c33cd0_TEST FAIL\n"); return 1; }

    std::printf("00c33cd0_TEST PASS\n");
    return 0;
}
