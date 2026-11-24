// Behaviour test for NUISystem::CUIMiscThingsDef::Alloc (0x004304E0): Alloc returns a fresh, zero-initialised def object.
#include <stdio.h>
struct DefBase { void* PDefManager; void* Setup; void* Template; bool DefaultValsApplied;
    DefBase() : PDefManager(0), Setup(0), Template(0), DefaultValsApplied(false) {} virtual ~DefBase() {} };
struct VectorMap { int* first; int* last; int* end; bool Dirty; VectorMap() : first(0), last(0), end(0), Dirty(false) {} };
struct MiscThingsDef : DefBase { VectorMap Things; };
static DefBase* Alloc() { return new MiscThingsDef; }
int main()
{
    MiscThingsDef* d = (MiscThingsDef*)Alloc();
    bool ok = d != 0 && d->PDefManager == 0 && d->Setup == 0 && d->Template == 0 && !d->DefaultValsApplied
           && d->Things.first == 0 && !d->Things.Dirty;
    delete d;
    printf(ok ? "004304e0_TEST PASS\n" : "004304e0_TEST FAIL\n");
    return ok ? 0 : 1;
}