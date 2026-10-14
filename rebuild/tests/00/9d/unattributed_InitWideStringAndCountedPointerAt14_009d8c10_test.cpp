#include <cstdio>
static int g_s = 0;
struct WS { wchar_t* p; WS() : p(0) { ++g_s; } };
struct CP { void* a; void* b; CP() : a(0), b(0) {} };
struct U { unsigned char lead[0x14]; WS name; CP p; U() : name(), p() {} };
int main()
{
    U u;
    if (g_s != 1 || u.name.p || u.p.a || u.p.b || sizeof(U) != 0x20) { std::printf("009d8c10_TEST FAIL\n"); return 1; }
    std::printf("009d8c10_TEST PASS\n");
    return 0;
}