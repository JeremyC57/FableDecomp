// Behaviour test for 0x00CA7C10 CEnumMediaTypes::AddRef: InterlockedIncrement of m_cRef, returning the new count (the interlocked import is modelled
// by a plain increment; the retail member sits at 0x10 in the 32-bit object).
#include <cstdio>
static long InterlockedIncrementModel(long* addend) { return ++*addend; }
struct CEnumMediaTypesModel {
    long m_cRef;
    unsigned long AddRef() { return InterlockedIncrementModel(&m_cRef); }
};
int main()
{
    CEnumMediaTypesModel obj;
    obj.m_cRef = 1;
    unsigned long a = obj.AddRef();
    unsigned long b = obj.AddRef();
    if (a != 2 || b != 3 || obj.m_cRef != 3) { std::printf("00ca7c10_TEST FAIL\n"); return 1; }
    std::printf("00ca7c10_TEST PASS\n");
    return 0;
}