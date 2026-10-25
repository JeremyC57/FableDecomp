// Behaviour test for 0x00CDB820: _STL::_Destroy(first, last) runs the virtual destructor of every 56-byte CHeroSoulsRoundDef in [first, last)
#include <cstdio>
#include <new>

static int g_baseDestroyed = 0;
static int g_derivedDestroyed = 0;

class CGameDefinitionBase {
public:
    virtual ~CGameDefinitionBase() { ++g_baseDestroyed; }
    long DefData;
};

class CHeroSoulsRoundDef : public CGameDefinitionBase {
public:
    long NumWaves;
    virtual ~CHeroSoulsRoundDef() { ++g_derivedDestroyed; }
};

template <class _ForwardIterator>
void TestDestroy(_ForwardIterator __first, _ForwardIterator __last)     // the reconstructed body
{
    for (; __first != __last; ++__first)
        (&*__first)->~CHeroSoulsRoundDef();
}

int main()
{
    static double storage[4 * sizeof(CHeroSoulsRoundDef) / sizeof(double) + 1];
    CHeroSoulsRoundDef* p = (CHeroSoulsRoundDef*)storage;
    for (int i = 0; i < 4; ++i) new ((void*)(p + i)) CHeroSoulsRoundDef();
    TestDestroy(p, p);                              // empty range
    if (g_derivedDestroyed != 0 || g_baseDestroyed != 0) { std::printf("00cdb820_TEST FAIL\n"); return 1; }
    TestDestroy(p, p + 4);
    if (g_derivedDestroyed != 4 || g_baseDestroyed != 4) { std::printf("00cdb820_TEST FAIL\n"); return 1; }
    std::printf("00cdb820_TEST PASS\n");
    return 0;
}