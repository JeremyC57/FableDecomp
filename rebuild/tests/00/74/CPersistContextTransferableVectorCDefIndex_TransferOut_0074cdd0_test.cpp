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
// Behaviour test for 0074cdd0: a default-constructed CPersistTraits<CDefIndex> transfers element [index] of the vector (Out).
class CPersistContext;
class CStringParser;
class CCharString; class CDefIndex { public: long Val; };
static const void* g_target = 0;
static int g_defaultOk = 0;
template <class T> struct CRetailVector {
    T* m_pFirst; T* m_pLast; T* m_pEnd;
    T& operator[](long index) { return m_pFirst[index]; }
};
template <class T> struct CPersistTraits {
    T DefaultVal;
    CPersistTraits() : DefaultVal() {}
    void TransferOut(CPersistContext*, CCharString* out, T* value) { g_target = value; g_defaultOk = (DefaultVal.Val == 0); }
};
struct CPersistContext_TransferableVector {
    virtual void TransferOut(CPersistContext* context, CCharString* out, long index);
    CRetailVector<CDefIndex>* TransVector;
};
void CPersistContext_TransferableVector::TransferOut(CPersistContext* context, CCharString* out, long index)
{
    CPersistTraits<CDefIndex> traits;
    traits.TransferOut(context, out, &(*TransVector)[index]);
}
int main()
{
    CDefIndex data[4];
    CRetailVector<CDefIndex> v; v.m_pFirst = data; v.m_pLast = data + 4; v.m_pEnd = data + 4;
    CPersistContext_TransferableVector t; t.TransVector = &v;
    t.TransferOut(0, 0, 3);
    if (g_target != &data[3] || !g_defaultOk) { std::printf("0074cdd0_TEST FAIL\n"); return 1; }
    std::printf("0074cdd0_TEST PASS\n");
    return 0;
}