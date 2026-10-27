// behaviour test for 0x00768B80 CUnprovenRecord_00768b80::CUnprovenRecord_00768b80() (CCharString's ctor is a counting stub)
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
class CUnprovenRecord_00768b80 {
public:
    CUnprovenRecord_00768b80();
    CCharString String0;
    CCharString String4;
};
CUnprovenRecord_00768b80::CUnprovenRecord_00768b80()
{
}
int main()
{
    static double raw[4];
    std::memset(raw, 0xCD, sizeof(raw));
    CUnprovenRecord_00768b80* p = new (raw) CUnprovenRecord_00768b80;
    int ok = g_ctor == 2 && g_order[0] == &p->String0 && g_order[1] == &p->String4 && (void*)p == (void*)raw;
    if (!ok) { std::printf("00768b80_TEST FAIL\n"); return 1; }
    std::printf("00768b80_TEST PASS\n");
    return 0;
}