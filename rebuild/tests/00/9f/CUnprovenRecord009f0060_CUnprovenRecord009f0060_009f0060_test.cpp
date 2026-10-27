// behaviour test for 0x009F0060 CUnprovenRecord_009f0060::CUnprovenRecord_009f0060() (CCharString's ctor is a counting stub)
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
class CUnprovenRecord_009f0060 {
public:
    CUnprovenRecord_009f0060();
    CCharString String0;
    CCharString String4;
};
CUnprovenRecord_009f0060::CUnprovenRecord_009f0060()
{
}
int main()
{
    static double raw[4];
    std::memset(raw, 0xCD, sizeof(raw));
    CUnprovenRecord_009f0060* p = new (raw) CUnprovenRecord_009f0060;
    int ok = g_ctor == 2 && g_order[0] == &p->String0 && g_order[1] == &p->String4 && (void*)p == (void*)raw;
    if (!ok) { std::printf("009f0060_TEST FAIL\n"); return 1; }
    std::printf("009f0060_TEST PASS\n");
    return 0;
}