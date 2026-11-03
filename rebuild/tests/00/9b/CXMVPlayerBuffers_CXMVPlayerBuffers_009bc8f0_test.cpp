// Behaviour test for CXMVPlayerBuffers::~CXMVPlayerBuffers (0x009BC8F0): an empty derived destructor runs, then the base destructor; while the base
// destructor runs the object is already of base type (the derived vtable store happens first, the base's own store follows).
#include <cstdio>
static int g_order[4]; static int g_n = 0; static int g_baseSawDerived = -1;
struct Base {
    virtual ~Base() { g_order[g_n++] = 2; g_baseSawDerived = Kind(); }
    virtual int Kind() const { return 0; }
};
class CXMVPlayerBuffers : public Base { public:
    virtual ~CXMVPlayerBuffers() { g_order[g_n++] = 1; }
    virtual int Kind() const { return 1; }
};
int main()
{
    Base* p = new CXMVPlayerBuffers;
    delete p;
    if (g_n != 2 || g_order[0] != 1 || g_order[1] != 2 || g_baseSawDerived != 0) { std::printf("009bc8f0_TEST FAIL\n"); return 1; }
    std::printf("009bc8f0_TEST PASS\n");
    return 0;
}