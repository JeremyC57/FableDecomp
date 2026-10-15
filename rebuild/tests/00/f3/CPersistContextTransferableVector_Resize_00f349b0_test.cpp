#include <cstdio>
#ifndef _MSC_VER
#define __fastcall
#define __stdcall
typedef long long Int64;
typedef unsigned long long UInt64;
#else
typedef __int64 Int64;
typedef unsigned __int64 UInt64;
#endif
// Behaviour test for 00f349b0: Resize(count) resizes TransVector with a default 0x64-byte record whose destructor chain ends in CResource.
static int g_constructed = 0;
static int g_resourceDestroyed = 0;
static unsigned long g_count = 0;
static long g_valueTag = 0;
struct CResource {
    virtual ~CResource() { ++g_resourceDestroyed; }
    unsigned long m_refCount;
};
struct CDefPointeeBase : CResource { virtual ~CDefPointeeBase() {} };
struct CDefClassBase : CDefPointeeBase { virtual ~CDefClassBase() {} long m_defBody[3]; };
struct CDefRecord64 : CDefClassBase {
    long m_tag;
    CDefRecord64() : m_tag(64) { ++g_constructed; }
};
struct CDefRecordVector {
    CDefRecord64* m_pFirst; CDefRecord64* m_pLast; CDefRecord64* m_pEnd;
    void resize(unsigned long count, const CDefRecord64& value) { g_count = count; g_valueTag = value.m_tag; }
    void resize(unsigned long count) { resize(count, CDefRecord64()); }
};
struct CPersistContext_TransferableVector {
    virtual void Resize(unsigned long count);
    CDefRecordVector* TransVector;
};
void CPersistContext_TransferableVector::Resize(unsigned long count)
{
    TransVector->resize(count);
}
int main()
{
    CDefRecordVector v; v.m_pFirst = v.m_pLast = v.m_pEnd = 0;
    CPersistContext_TransferableVector t; t.TransVector = &v;
    t.Resize(5);
    if (g_count != 5 || g_valueTag != 64 || g_constructed != 1 || g_resourceDestroyed != 1) { std::printf("00f349b0_TEST FAIL\n"); return 1; }
    std::printf("00f349b0_TEST PASS\n");
    return 0;
}