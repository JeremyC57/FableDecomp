// Behaviour model of CCreatureAction_CallSpouseHome::`scalar deleting destructor' (row 0x00933D30, ??_G): a class with an implicit destructor over a base whose
// virtual destructor is the one callee; delete through a base pointer runs ~CCreatureActionBase once and frees the object, an explicit virtual
// destructor call (flags 0) runs it without freeing. Self-contained: the row object carries an 80-slot-class vtable whose other slots
// need the whole engine, so the test models the same class shape (NF419 r1: the row's leaf bytes and relocations equal HEAD's).
#include <cstdio>
#include <cstdlib>
static int g_baseDtors = 0, g_frees = 0; static void* g_freed = 0;
void* operator new(unsigned int n) { return std::malloc(n ? n : 1); }
void operator delete(void* p) { ++g_frees; g_freed = p; std::free(p); }
class CCreatureActionBase { public: int baseState; CCreatureActionBase() : baseState(1) { } virtual ~CCreatureActionBase() { ++g_baseDtors; } };
class CCreatureAction_CallSpouseHome : public CCreatureActionBase { public: int ownState[3]; };
int main()
{
    CCreatureAction_CallSpouseHome* p = new CCreatureAction_CallSpouseHome;
    CCreatureActionBase* b = p;
    delete b;
    if (g_baseDtors != 1 || g_frees != 1 || g_freed != (void*)p) { std::printf("00933d30 FAIL delete\n"); return 1; }
    CCreatureAction_CallSpouseHome* q = new CCreatureAction_CallSpouseHome;
    q->~CCreatureAction_CallSpouseHome();
    if (g_baseDtors != 2 || g_frees != 1) { std::printf("00933d30 FAIL no-free path\n"); return 1; }
    std::free(q);
    std::printf("CLandscapeBackgroundPatch_00933d30_TEST PASS\n");
    return 0;
}
