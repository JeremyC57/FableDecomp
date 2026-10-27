// behaviour test for 0x009F00D0 CUnprovenRecord_009f00d0::CUnprovenRecord_009f00d0() (CCharString's ctor is a counting stub)
#include <cstdio>
#include <cstring>
#include <new>
static int g_ctor = 0;
static void* g_order[2];
class CCharString {
public:
    CCharString() : PData(0) { g_order[g_ctor++] = this; }
    ~CCharString() { }
    void* PData;
};
class CUnprovenRecord_009f00d0 {
public:
    CUnprovenRecord_009f00d0();
    CCharString String0;
    CCharString String4;
};
CUnprovenRecord_009f00d0::CUnprovenRecord_009f00d0()
{
}
int main()
{
    static double raw[4];
    std::memset(raw, 0xCD, sizeof(raw));
    CUnprovenRecord_009f00d0* p = new (raw) CUnprovenRecord_009f00d0;
    int ok = g_ctor == 2 && g_order[0] == &p->String0 && g_order[1] == &p->String4 && (void*)p == (void*)raw;
    if (!ok) { std::printf("009f00d0_TEST FAIL\n"); return 1; }
    std::printf("009f00d0_TEST PASS\n");
    return 0;
}