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
// Behaviour test for 004aa3f0: the long reader parses a value that is stored, converted to double, through Object.
struct CPersistContext;
struct CStringParser;
static long g_parsed = 7;
static void __fastcall TransferInLong(CPersistContext*, CStringParser*, long* result) { *result = g_parsed; }

struct CPersistContext_TransferableComponent {
    virtual void TransferIn(CPersistContext* context, CStringParser* parser);
    double* m_pValue;
};
void CPersistContext_TransferableComponent::TransferIn(CPersistContext* context, CStringParser* parser)
{
    double* target = m_pValue;
    long value;
    TransferInLong(context, parser, &value);
    *target = (double)value;
}

int main()
{
    double v = 0;
    CPersistContext_TransferableComponent c;
    c.m_pValue = &v;
    c.TransferIn(0, 0);
    if (!(v == 7.0)) { std::printf("004aa3f0_TEST FAIL\n"); return 1; }
    std::printf("004aa3f0_TEST PASS\n");
    return 0;
}