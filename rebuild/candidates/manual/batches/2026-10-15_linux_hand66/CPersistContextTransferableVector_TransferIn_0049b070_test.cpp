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
// Behaviour test for 0049b070: a CPersistTraits<CCharString> (owning a default string) transfers element [index] (In) and destroys its string.
struct CPersistContext;
struct CStringParser;
static int g_constructed = 0;
static int g_destroyed = 0;
static const void* g_target = 0;
static int g_liveDuringCall = 0;
struct CCharString {
    long m_tag;
    CCharString() : m_tag(0) { ++g_constructed; }
    ~CCharString() { ++g_destroyed; }
};
template <class T> struct CRetailVector {
    T* m_pFirst; T* m_pLast; T* m_pEnd;
    T& operator[](long index) { return m_pFirst[index]; }
};
template <class T> struct CPersistTraits {
    T DefaultVal;
    void TransferIn(CPersistContext*, CStringParser* parser, T* value) { g_target = value; g_liveDuringCall = g_constructed - g_destroyed; }
};
struct CPersistContext_TransferableVector {
    virtual void TransferIn(CPersistContext* context, CStringParser* parser, long index);
    CRetailVector<CCharString>* TransVector;
};
void CPersistContext_TransferableVector::TransferIn(CPersistContext* context, CStringParser* parser, long index)
{
    CPersistTraits<CCharString> traits;
    traits.TransferIn(context, parser, &(*TransVector)[index]);
}
int main()
{
    {
        CCharString data[3];
        CRetailVector<CCharString> v; v.m_pFirst = data; v.m_pLast = data + 3; v.m_pEnd = data + 3;
        CPersistContext_TransferableVector t; t.TransVector = &v;
        g_constructed = 0; g_destroyed = 0;
        t.TransferIn(0, 0, 2);
        if (g_target != &data[2] || g_liveDuringCall != 1 || g_constructed != 1 || g_destroyed != 1) { std::printf("0049b070_TEST FAIL\n"); return 1; }
    }
    std::printf("0049b070_TEST PASS\n");
    return 0;
}
