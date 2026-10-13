#include <cstdio>

struct CWideString { void* p; CWideString() : p(0) {} ~CWideString() {} };
static int g_calls = 0;
struct F { virtual CWideString GetPathname() { ++g_calls; CWideString w; w.p = (void*)7; return w; } };
struct B { unsigned char pad[0x7c]; F* BankFile_Data; CWideString GetBankPathname() { return BankFile_Data->GetPathname(); } };
int main()
{
    F f; B b; b.BankFile_Data = &f; CWideString w = b.GetBankPathname();
    if (g_calls != 1 || w.p != (void*)7) { std::printf("009cbf10_TEST FAIL\n"); return 1; }
    std::printf("009cbf10_TEST PASS\n");
    return 0;
}
