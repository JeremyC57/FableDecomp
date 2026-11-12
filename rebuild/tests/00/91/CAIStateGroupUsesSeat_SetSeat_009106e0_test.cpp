#include <cstdio>
struct PB { virtual void v00() {} void* p; PB() : p(0) {} ~PB() {} void* Get() { return p; } void Set(void* x) { p = x; } };
struct P : PB { virtual void v00() {} P() : PB() {} };
struct X { unsigned char pad[0x34]; PB t; void Assign(void* v) { P tmp; tmp.Set(v); t.Set(tmp.Get()); } };
int main()
{
    int v; X x; x.Assign(&v); if (x.t.Get() != &v) { std::printf("009106e0_TEST FAIL\n"); return 1; }

    std::printf("009106e0_TEST PASS\n");
    return 0;
}