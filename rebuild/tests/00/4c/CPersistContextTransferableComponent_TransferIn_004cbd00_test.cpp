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
// Behaviour test for 004cbd00: the long reader parses a value that is stored, converted to short, through Object.
struct CPersistContext;
struct CStringParser;
static long g_parsed = 0x12345;
static void __fastcall TransferInLong(CPersistContext*, CStringParser*, long* result) { *result = g_parsed; }

struct CPersistContext_TransferableComponent {
    virtual void TransferIn(CPersistContext* context, CStringParser* parser);
    short* m_pValue;
};
void CPersistContext_TransferableComponent::TransferIn(CPersistContext* context, CStringParser* parser)
{
    short* target = m_pValue;
    long value;
    TransferInLong(context, parser, &value);
    *target = (short)value;
}

int main()
{
    short v = 0;
    CPersistContext_TransferableComponent c;
    c.m_pValue = &v;
    c.TransferIn(0, 0);
    if (!(v == (short)0x2345)) { std::printf("004cbd00_TEST FAIL\n"); return 1; }
    std::printf("004cbd00_TEST PASS\n");
    return 0;
}