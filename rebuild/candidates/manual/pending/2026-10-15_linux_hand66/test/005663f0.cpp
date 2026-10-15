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
// Behaviour test for 005663f0: element [index] of the float vector is formatted with six digits and appended.
struct CPersistContext;
struct CStringParser;
static long g_digits = 0;
static double g_formatted = 0.0;
static int g_destroyed = 0;
struct CCharString {
    long m_tag;
    ~CCharString() { ++g_destroyed; }
    CCharString& operator+=(const CCharString& other) { m_tag += other.m_tag; return *this; }
    static CCharString FormatDouble(long digits, double value) { g_digits = digits; g_formatted = value; CCharString s; s.m_tag = 10; return s; }
};
template <class T> struct CRetailVector { T* m_pFirst; T* m_pLast; T* m_pEnd; };
struct CPersistContext_TransferableVector {
    virtual void TransferOut(CPersistContext* context, CCharString* out, long index);
    CRetailVector<float>* TransVector;
};
void CPersistContext_TransferableVector::TransferOut(CPersistContext* context, CCharString* out, long index)
{
    float* first = TransVector->m_pFirst;
    *out += CCharString::FormatDouble(6, first[index]);
}
int main()
{
    float data[3] = { 1.0f, -0.75f, 3.0f };
    CRetailVector<float> v; v.m_pFirst = data; v.m_pLast = data + 3; v.m_pEnd = data + 3;
    CPersistContext_TransferableVector t; t.TransVector = &v;
    {
        CCharString out; out.m_tag = 1;
        g_destroyed = 0;
        t.TransferOut(0, &out, 1);
        if (g_digits != 6 || g_formatted != -0.75 || out.m_tag != 11 || g_destroyed < 1) { std::printf("005663f0_TEST FAIL\n"); return 1; }
    }
    std::printf("005663f0_TEST PASS\n");
    return 0;
}
