// Test: CPredicate_And<CIsThingAlive, CIsVariableFalse> scalar deleting destructor -- Right then Left destroyed, and the memory freed only when bit 0 of the flags is set.
#include <cstdio>
#include <cstring>
#include <new>
static char g_seq[16]; static int g_frees = 0;
struct CBase { virtual ~CBase() { std::strcat(g_seq, "B"); } };
struct CAlive : CBase { int t[3]; ~CAlive() { std::strcat(g_seq, "L"); } };
struct CFalse : CBase { const bool* v; ~CFalse() { std::strcat(g_seq, "R"); } };
struct CAnd : CBase { CAlive Left; CFalse Right; ~CAnd() { std::strcat(g_seq, "A"); }
    static void operator delete(void* p) { ++g_frees; ::operator delete(p); } };
static void* ScalarDeletingDtor(CAnd* p, unsigned int flags) { p->~CAnd(); if (flags & 1) CAnd::operator delete(p); return p; }
int main()
{
    CAnd* a = new CAnd; ScalarDeletingDtor(a, 1);
    bool ok = !std::strcmp(g_seq, "ARBLBB") && g_frees == 1;
    static unsigned char raw[sizeof(CAnd) + 8]; g_seq[0] = 0; CAnd* b = new (raw) CAnd; ScalarDeletingDtor(b, 0);
    ok = ok && !std::strcmp(g_seq, "ARBLBB") && g_frees == 1;
    std::printf(ok ? "00e7dfe0_TEST PASS\n" : "00e7dfe0_TEST FAIL\n");
    return ok ? 0 : 1;
}