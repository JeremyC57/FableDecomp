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
// Behaviour test for 0074cda0: a default-constructed CPersistTraits<CDefIndex> transfers element [index] of the vector (In).
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
    void TransferIn(CPersistContext*, CStringParser* parser, T* value) { g_target = value; g_defaultOk = (DefaultVal.Val == 0); }
};
struct CPersistContext_TransferableVector {
    virtual void TransferIn(CPersistContext* context, CStringParser* parser, long index);
    CRetailVector<CDefIndex>* TransVector;
};
void CPersistContext_TransferableVector::TransferIn(CPersistContext* context, CStringParser* parser, long index)
{
    CPersistTraits<CDefIndex> traits;
    traits.TransferIn(context, parser, &(*TransVector)[index]);
}
int main()
{
    CDefIndex data[4];
    CRetailVector<CDefIndex> v; v.m_pFirst = data; v.m_pLast = data + 4; v.m_pEnd = data + 4;
    CPersistContext_TransferableVector t; t.TransVector = &v;
    t.TransferIn(0, 0, 3);
    if (g_target != &data[3] || !g_defaultOk) { std::printf("0074cda0_TEST FAIL\n"); return 1; }
    std::printf("0074cda0_TEST PASS\n");
    return 0;
}