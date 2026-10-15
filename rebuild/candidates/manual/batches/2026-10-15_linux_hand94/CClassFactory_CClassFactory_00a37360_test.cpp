#include <cstdio>
#include <cstdlib>
#include <cstring>
// Behaviour test for 0x00A37360: ClassList destroyed, then the ClassTypeInfo range destroyed and its block freed, then ~CBase.
static char g_log[16]; static int g_n = 0;
static void Log(char c) { g_log[g_n++] = c; g_log[g_n] = 0; }
struct SScalarTag { SScalarTag() {} };
struct CClassTypeInfo { int id; };
static long g_rangeCount = -1;
static void _Destroy_range(CClassTypeInfo* first, CClassTypeInfo* last, const SScalarTag&) { g_rangeCount = (long)(last - first); Log('R'); }
struct SClassTypeInfoVector {
    CClassTypeInfo* _M_start; CClassTypeInfo* _M_finish; CClassTypeInfo* _M_end_of_storage;
    ~SClassTypeInfoVector()
    {
        _Destroy_range(_M_start, _M_finish, SScalarTag());
        if (_M_start) { Log('F'); std::free(_M_start); }
    }
};
struct SClassList { void* head; ~SClassList() { Log('L'); } };
struct CBase { virtual void v00() {} ~CBase() { Log('B'); } };
struct CClassFactory : CBase {
    SClassTypeInfoVector ClassTypeInfo;
    unsigned long MaxClassTypeID, NoAllocatedClasses;
    SClassList ClassList;
    virtual void v00() {}
    ~CClassFactory();
};
CClassFactory::~CClassFactory()
{
}
int main()
{
    {
        CClassFactory f; f.ClassTypeInfo._M_start = (CClassTypeInfo*)std::malloc(sizeof(CClassTypeInfo) * 3);
        f.ClassTypeInfo._M_finish = f.ClassTypeInfo._M_start + 2;
    }
    if (std::strcmp(g_log, "LRFB") != 0 || g_rangeCount != 2) { std::printf("00a37360_TEST FAIL\n"); return 1; }
    g_n = 0; g_log[0] = 0;
    { CClassFactory f; f.ClassTypeInfo._M_start = f.ClassTypeInfo._M_finish = 0; }
    if (std::strcmp(g_log, "LRB") != 0 || g_rangeCount != 0) { std::printf("00a37360_TEST FAIL\n"); return 1; }
    std::printf("00a37360_TEST PASS\n");
    return 0;
}
