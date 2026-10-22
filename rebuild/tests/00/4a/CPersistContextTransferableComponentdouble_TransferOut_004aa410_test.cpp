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
// Behaviour test for 004aa410: the component widens *Object (double) to a long local and passes its address to the long writer.
struct CPersistContext;
struct CCharString;
static long g_written = 0;
static CCharString* g_out = 0;
static void __fastcall TransferOutLong(CPersistContext*, CCharString* out, long* value) { g_written = *value; g_out = out; }

struct CPersistContext_TransferableComponent {
    virtual void TransferOut(CPersistContext* context, CCharString* out);
    double* m_pValue;
};
void CPersistContext_TransferableComponent::TransferOut(CPersistContext* context, CCharString* out)
{
    long value = (long)*m_pValue;
    TransferOutLong(context, out, &value);
}

int main()
{
    double v = 41.9;
    CPersistContext_TransferableComponent c;
    c.m_pValue = &v;
    CCharString* out = (CCharString*)&v;
    c.TransferOut(0, out);
    if (g_written != 41L || g_out != out) { std::printf("004aa410_TEST FAIL\n"); return 1; }
    std::printf("004aa410_TEST PASS\n");
    return 0;
}