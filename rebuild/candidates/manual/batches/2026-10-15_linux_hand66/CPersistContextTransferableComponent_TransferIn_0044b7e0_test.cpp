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
// Behaviour test for 0044b7e0: the 64-bit component stores the parser's ReadInt64 result through Object.
struct CPersistContext;
static Int64 g_next = 0;
struct CStringParser {
    Int64 ReadInt64() { return g_next; }
};
struct CPersistContext_TransferableComponent {
    virtual void TransferIn(CPersistContext* context, CStringParser* parser);
    Int64* m_pValue;
};
void CPersistContext_TransferableComponent::TransferIn(CPersistContext* context, CStringParser* parser)
{
    Int64* target = m_pValue;
    *target = parser->ReadInt64();
}
int main()
{
    Int64 v = 0;
    g_next = ((Int64)0x12345678 << 20) + 99;
    CPersistContext_TransferableComponent c;
    c.m_pValue = &v;
    CStringParser p;
    c.TransferIn(0, &p);
    if (v != g_next) { std::printf("0044b7e0_TEST FAIL\n"); return 1; }
    std::printf("0044b7e0_TEST PASS\n");
    return 0;
}
