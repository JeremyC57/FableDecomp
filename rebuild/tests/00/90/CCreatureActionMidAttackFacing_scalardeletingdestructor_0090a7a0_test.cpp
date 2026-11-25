// Behaviour model of CCreatureAction_MidAttackFacing::`scalar deleting destructor' (row 0x0090A7A0, ??_G): a class with an implicit destructor over a base whose
// virtual destructor is the one callee; delete through a base pointer runs ~CCombatActionBase once and frees the object, an explicit virtual
// destructor call (flags 0) runs it without freeing. Self-contained: the row object carries an 80-slot-class vtable whose other slots
// need the whole engine, so the test models the same class shape (NF419 r1: the row's leaf bytes and relocations equal HEAD's).
#include <cstdio>
#include <cstdlib>
static int g_baseDtors = 0, g_frees = 0; static void* g_freed = 0;
void* operator new(unsigned int n) { return std::malloc(n ? n : 1); }
void operator delete(void* p) { ++g_frees; g_freed = p; std::free(p); }
class CCombatActionBase { public: int baseState; CCombatActionBase() : baseState(1) { } virtual ~CCombatActionBase() { ++g_baseDtors; } };
class CCreatureAction_MidAttackFacing : public CCombatActionBase { public: int ownState[3]; };
int main()
{
    CCreatureAction_MidAttackFacing* p = new CCreatureAction_MidAttackFacing;
    CCombatActionBase* b = p;
    delete b;
    if (g_baseDtors != 1 || g_frees != 1 || g_freed != (void*)p) { std::printf("0090a7a0 FAIL delete\n"); return 1; }
    CCreatureAction_MidAttackFacing* q = new CCreatureAction_MidAttackFacing;
    q->~CCreatureAction_MidAttackFacing();
    if (g_baseDtors != 2 || g_frees != 1) { std::printf("0090a7a0 FAIL no-free path\n"); return 1; }
    std::free(q);
    std::printf("CLandscapeBackgroundPatch_0090a7a0_TEST PASS\n");
    return 0;
}
