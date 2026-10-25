// Behaviour test for 0x00F28F20: std::_Destroy(first, last) runs the virtual destructor of every 100-byte CFeatDef in [first, last)
#include <cstdio>
#include <new>

static int g_baseDestroyed = 0;
static int g_derivedDestroyed = 0;

class CGameDefinitionBase {
public:
    virtual ~CGameDefinitionBase() { ++g_baseDestroyed; }
    long DefData;
};

class CFeatDef : public CGameDefinitionBase {
public:
    long NumWaves;
    virtual ~CFeatDef() { ++g_derivedDestroyed; }
};

template <class _ForwardIterator>
void TestDestroy(_ForwardIterator __first, _ForwardIterator __last)     // the reconstructed body
{
    for (; __first != __last; ++__first)
        (&*__first)->~CFeatDef();
}

int main()
{
    static double storage[4 * sizeof(CFeatDef) / sizeof(double) + 1];
    CFeatDef* p = (CFeatDef*)storage;
    for (int i = 0; i < 4; ++i) new ((void*)(p + i)) CFeatDef();
    TestDestroy(p, p);                              // empty range
    if (g_derivedDestroyed != 0 || g_baseDestroyed != 0) { std::printf("00f28f20_TEST FAIL\n"); return 1; }
    TestDestroy(p, p + 4);
    if (g_derivedDestroyed != 4 || g_baseDestroyed != 4) { std::printf("00f28f20_TEST FAIL\n"); return 1; }
    std::printf("00f28f20_TEST PASS\n");
    return 0;
}