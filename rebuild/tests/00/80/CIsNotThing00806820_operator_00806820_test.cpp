// Behaviour test for 0x00806820: predicate: the pointer's item differs from the remembered thing
#include <stdio.h>
static int g_fail = 0;
#define CHECK(c) do { if (!(c)) { printf("FAIL line %d\n", __LINE__); g_fail = 1; } } while (0)

struct SPtr { void* Item; void* GetPItem() const { return Item; } };
struct SIsNot { void* Thing; bool operator()(const SPtr& p) const { void* t = Thing; return p.GetPItem() != t; } };
int main()
{
    int a, b; SIsNot f; f.Thing = &a; SPtr p;
    p.Item = &a; CHECK(!f(p));
    p.Item = &b; CHECK(f(p));
    p.Item = 0; CHECK(f(p));
    if (!g_fail) printf("00806820_TEST PASS\n");
    return g_fail;
}