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
// Behaviour test for 00405a10: the component widens *Object (unsigned char) to a long local and passes its address to the long writer.
struct CPersistContext;
struct CCharString;
static long g_written = 0;
static CCharString* g_out = 0;
static void __fastcall TransferOutLong(CPersistContext*, CCharString* out, long* value) { g_written = *value; g_out = out; }

struct CPersistContext_TransferableComponent {
    virtual void TransferOut(CPersistContext* context, CCharString* out);
    unsigned char* m_pValue;
};
void CPersistContext_TransferableComponent::TransferOut(CPersistContext* context, CCharString* out)
{
    long value = (long)*m_pValue;
    TransferOutLong(context, out, &value);
}

int main()
{
    unsigned char v = 200;
    CPersistContext_TransferableComponent c;
    c.m_pValue = &v;
    CCharString* out = (CCharString*)&v;
    c.TransferOut(0, out);
    if (g_written != 200L || g_out != out) { std::printf("00405a10_TEST FAIL\n"); return 1; }
    std::printf("00405a10_TEST PASS\n");
    return 0;
}
