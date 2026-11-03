// Behaviour test for CVtable_012b8d98::~CVtable_012b8d98 (0x00C56D60): an empty derived destructor runs, then the base destructor; while the base
// destructor runs the object is already of base type (the derived vtable store happens first, the base's own store follows).
#include <cstdio>
static int g_order[4]; static int g_n = 0; static int g_baseSawDerived = -1;
struct Base {
    virtual ~Base() { g_order[g_n++] = 2; g_baseSawDerived = Kind(); }
    virtual int Kind() const { return 0; }
};
struct CVtable_012b8d98 : Base {
    virtual ~CVtable_012b8d98() { g_order[g_n++] = 1; }
    virtual int Kind() const { return 1; }
};
int main()
{
    Base* p = new CVtable_012b8d98;
    delete p;
    if (g_n != 2 || g_order[0] != 1 || g_order[1] != 2 || g_baseSawDerived != 0) { std::printf("00c56d60_TEST FAIL\n"); return 1; }
    std::printf("00c56d60_TEST PASS\n");
    return 0;
}
