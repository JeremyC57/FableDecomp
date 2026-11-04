// Behaviour test for the mixer voice-effect destructor 0x00C701D0: destroying the derived effect runs the voice-effect base destructor
// (0x00C6E630) once, after the derived part, also through a base pointer.
#include <cstdio>
static int g_order = 0, g_base = 0, g_derived = 0;
struct CCodaObjectBase { virtual ~CCodaObjectBase() {} };
struct CVtable_012beda4 : CCodaObjectBase { virtual ~CVtable_012beda4() { g_base = ++g_order; } };
struct CEffect : CVtable_012beda4 { virtual ~CEffect() { g_derived = ++g_order; } };
int main()
{
    CCodaObjectBase* p = new CEffect; delete p;
    if (g_derived != 1 || g_base != 2) { std::printf("00c701d0_TEST FAIL\n"); return 1; }
    std::printf("00c701d0_TEST PASS\n");
    return 0;
}