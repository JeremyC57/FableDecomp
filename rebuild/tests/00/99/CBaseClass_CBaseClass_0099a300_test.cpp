// Behaviour test for 0x0099A300 CBaseClass::~CBaseClass(): the empty root destructor, run through a virtual delete after
// the derived destructor.
#include <cstdio>

static int g_order[4];
static int g_n = 0;
class BaseClass { public: BaseClass() { } virtual ~BaseClass() { g_order[g_n++] = 1; } };
class Derived : public BaseClass { public: ~Derived() { g_order[g_n++] = 2; } };

int main()
{
    BaseClass* p = new Derived;
    delete p;
    if (g_n != 2 || g_order[0] != 2 || g_order[1] != 1) { std::printf("0099a300_TEST FAIL\n"); return 1; }
    { BaseClass b; }
    if (g_n != 3 || g_order[2] != 1) { std::printf("0099a300_TEST FAIL\n"); return 1; }
    std::printf("0099a300_TEST PASS\n");
    return 0;
}