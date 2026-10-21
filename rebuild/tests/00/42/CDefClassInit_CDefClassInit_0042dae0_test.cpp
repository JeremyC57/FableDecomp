// behaviour test for 0x0042DAE0
#include <stdio.h>
static int g_copies;
class CCharStringData;
class CCharString {
public:
    CCharStringData* PStringData;
    CCharString(CCharStringData* d) : PStringData(d) {}
    CCharString(const CCharString& other) : PStringData(other.PStringData) { ++g_copies; }
    ~CCharString() {}
};
class CDefClassBase;
typedef CDefClassBase* (*DefClassAllocFunc)();
class CDefClassInit {
public:
    CCharString       Name;                              // +0x00
    DefClassAllocFunc AllocFunc;                         // +0x04
    CDefClassInit(const CCharString& name, DefClassAllocFunc allocFunc);
};
CDefClassInit::CDefClassInit(const CCharString& name, DefClassAllocFunc allocFunc)
    : Name(name), AllocFunc(allocFunc)
{
}
static CDefClassBase* AllocX() { return 0; }
int main()
{
    CCharString n((CCharStringData*)0x77); CDefClassInit i(n, AllocX);
    int ok = i.Name.PStringData == (CCharStringData*)0x77 && i.AllocFunc == AllocX && g_copies == 1 && sizeof(i) == 8;
    printf(ok ? "0042dae0_TEST PASS\n" : "0042dae0_TEST FAIL\n");
    return ok ? 0 : 1;
}