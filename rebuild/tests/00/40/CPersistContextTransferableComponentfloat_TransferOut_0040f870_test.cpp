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
// Behaviour test for 0040f870: the float is formatted with six digits into a temporary that is appended to the output and destroyed.
class CPersistContext;
static long g_digits = 0;
static double g_formatted = 0.0;
static int g_appended = 0;
static int g_destroyed = 0;
class CCharString { public:
    long m_tag;
    ~CCharString() { ++g_destroyed; }
    const CCharString& operator+=(const CCharString& other) { m_tag += other.m_tag; ++g_appended; return *this; }
    static CCharString FormatDouble(long digits, double value) { g_digits = digits; g_formatted = value; CCharString s; s.m_tag = 5; return s; }
};
struct CPersistContext_TransferableComponent {
    virtual void TransferOut(CPersistContext* context, CCharString* out);
    float* m_pValue;
};
void CPersistContext_TransferableComponent::TransferOut(CPersistContext* context, CCharString* out)
{
    float* value = m_pValue;
    *out += CCharString::FormatDouble(6, *value);
}
int main()
{
    float f = 2.5f;
    CPersistContext_TransferableComponent c;
    c.m_pValue = &f;
    {
        CCharString out; out.m_tag = 1;
        g_destroyed = 0;
        c.TransferOut(0, &out);
        if (g_digits != 6 || g_formatted != 2.5 || g_appended != 1 || out.m_tag != 6 || g_destroyed < 1) { std::printf("0040f870_TEST FAIL\n"); return 1; }
    }
    std::printf("0040f870_TEST PASS\n");
    return 0;
}