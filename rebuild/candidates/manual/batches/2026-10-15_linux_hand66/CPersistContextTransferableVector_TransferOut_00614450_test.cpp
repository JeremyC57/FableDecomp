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
// Behaviour test for 00614450: element [index] of the unsigned 64-bit vector is formatted by the CCharString constructor and appended.
struct CPersistContext;
struct CStringParser;
static UInt64 g_formatted = 0;
static int g_destroyed = 0;
struct CCharString {
    long m_tag;
    CCharString() : m_tag(0) {}
    explicit CCharString(UInt64 value) : m_tag(4) { g_formatted = value; }
    ~CCharString() { ++g_destroyed; }
    CCharString& operator+=(const CCharString& other) { m_tag += other.m_tag; return *this; }
};
template <class T> struct CRetailVector { T* m_pFirst; T* m_pLast; T* m_pEnd; };
struct CPersistContext_TransferableVector {
    virtual void TransferOut(CPersistContext* context, CCharString* out, long index);
    CRetailVector<UInt64>* TransVector;
};
void CPersistContext_TransferableVector::TransferOut(CPersistContext* context, CCharString* out, long index)
{
    UInt64* first = TransVector->m_pFirst;
    *out += CCharString(first[index]);
}
int main()
{
    UInt64 data[3]; data[0] = 1; data[1] = ((UInt64)1 << 40) + 5; data[2] = 3;
    CRetailVector<UInt64> v; v.m_pFirst = data; v.m_pLast = data + 3; v.m_pEnd = data + 3;
    CPersistContext_TransferableVector t; t.TransVector = &v;
    {
        CCharString out;
        g_destroyed = 0;
        t.TransferOut(0, &out, 1);
        if (g_formatted != data[1] || out.m_tag != 4 || g_destroyed != 1) { std::printf("00614450_TEST FAIL\n"); return 1; }
    }
    std::printf("00614450_TEST PASS\n");
    return 0;
}
