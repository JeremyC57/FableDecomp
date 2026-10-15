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
// Behaviour test for 00614430: the parser's ReadInt64 result is stored into element [index] of the 64-bit vector.
struct CPersistContext;
struct CCharString;
static Int64 g_next = 0;
struct CStringParser { Int64 ReadInt64() { return g_next; } };
template <class T> struct CRetailVector { T* m_pFirst; T* m_pLast; T* m_pEnd; };
struct CPersistContext_TransferableVector {
    virtual void TransferIn(CPersistContext* context, CStringParser* parser, long index);
    CRetailVector<Int64>* TransVector;
};
void CPersistContext_TransferableVector::TransferIn(CPersistContext* context, CStringParser* parser, long index)
{
    Int64* first = TransVector->m_pFirst;
    first[index] = parser->ReadInt64();
}
int main()
{
    Int64 data[3]; data[0] = 1; data[1] = 2; data[2] = 3;
    CRetailVector<Int64> v; v.m_pFirst = data; v.m_pLast = data + 3; v.m_pEnd = data + 3;
    CPersistContext_TransferableVector t; t.TransVector = &v;
    CStringParser p;
    g_next = -((Int64)1 << 35);
    t.TransferIn(0, &p, 2);
    if (data[2] != g_next || data[1] != 2 || data[0] != 1) { std::printf("00614430_TEST FAIL\n"); return 1; }
    std::printf("00614430_TEST PASS\n");
    return 0;
}