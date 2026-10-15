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
// Behaviour test for 0044b800: the 64-bit value is formatted by the CCharString(unsigned __int64) constructor, appended and destroyed.
struct CPersistContext;
static UInt64 g_formatted = 0;
static int g_appended = 0;
static int g_destroyed = 0;
struct CCharString {
    long m_tag;
    CCharString() : m_tag(0) {}
    explicit CCharString(UInt64 value) : m_tag(3) { g_formatted = value; }
    ~CCharString() { ++g_destroyed; }
    CCharString& operator+=(const CCharString& other) { m_tag += other.m_tag; ++g_appended; return *this; }
};
struct CPersistContext_TransferableComponent {
    virtual void TransferOut(CPersistContext* context, CCharString* out);
    UInt64* m_pValue;
};
void CPersistContext_TransferableComponent::TransferOut(CPersistContext* context, CCharString* out)
{
    *out += CCharString(*m_pValue);
}
int main()
{
    UInt64 v = ((UInt64)0x89ABCDEF << 16) + 7;
    CPersistContext_TransferableComponent c;
    c.m_pValue = &v;
    {
        CCharString out;
        g_destroyed = 0;
        c.TransferOut(0, &out);
        if (g_formatted != v || g_appended != 1 || out.m_tag != 3 || g_destroyed != 1) { std::printf("0044b800_TEST FAIL\n"); return 1; }
    }
    std::printf("0044b800_TEST PASS\n");
    return 0;
}
