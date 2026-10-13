#include <cstdio>

struct N { void* vt; long A; long B; bool C; bool D; unsigned char pad[2];
    N& operator=(const N& o) { if (this != &o) { A = o.A; B = o.B; C = o.C; D = o.D; } return *this; } };
int main()
{
    N a, b; a.vt = (void*)1; b.vt = (void*)2; b.A = 5; b.B = 6; b.C = true; b.D = false; a.A = a.B = 0; a.C = a.D = false;
    N& r = (a = b);
    if (&r != &a || a.A != 5 || a.B != 6 || !a.C || a.D || a.vt != (void*)1) { std::printf("00a76240_TEST FAIL\n"); return 1; }
    std::printf("00a76240_TEST PASS\n");
    return 0;
}
