#include <cstdio>
#include <new>
struct CEnumPins { void* filter; CEnumPins(void* f) : filter(f) {} };
static bool g_fail = false;
static long EnumPins(void* self, CEnumPins** pp)
{
    if (pp == 0) return (long)0x80004003;
    *pp = g_fail ? 0 : new CEnumPins(self);
    return *pp == 0 ? (long)0x8007000E : 0;
}
int main()
{
    int self; CEnumPins* e = 0;
    if (EnumPins(&self, 0) != (long)0x80004003 || EnumPins(&self, &e) != 0 || !e || e->filter != &self) { std::printf("00cac890_TEST FAIL\n"); return 1; }
    delete e; g_fail = true;
    if (EnumPins(&self, &e) != (long)0x8007000E || e) { std::printf("00cac890_TEST FAIL\n"); return 1; }
    std::printf("00cac890_TEST PASS\n");
    return 0;
}