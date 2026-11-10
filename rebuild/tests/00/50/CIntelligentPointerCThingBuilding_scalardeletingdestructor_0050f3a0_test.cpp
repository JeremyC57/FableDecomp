// Behaviour test for 0x0050F3A0 CIntelligentPointer<CThingBuilding>::`scalar deleting destructor': deleting through the
// base pointer runs ~CBaseIntelligentPointer exactly once on the object and frees it.
#include <stdio.h>
// Portable model of engine/CIntelligentPointer.h (vptr + PData; the derived template declares no destructor).
class CBaseObject;
class CThingBuilding { public: int Value; };
static int g_ctor = 0, g_dtor = 0; static const void* g_last = 0;
class CBaseIntelligentPointer {
public:
    CBaseIntelligentPointer() : PData(0) { ++g_ctor; }
    virtual ~CBaseIntelligentPointer() { ++g_dtor; g_last = this; }
    CBaseObject* GetPItem() const { return PData; }
private:
    CBaseObject* PData;
};
template<class T> class CIntelligentPointer : public CBaseIntelligentPointer {
public:
    T* Get() const { return (T*)GetPItem(); }
};
int main()
{
    CIntelligentPointer<CThingBuilding>* pointer = new CIntelligentPointer<CThingBuilding>;
    CBaseIntelligentPointer* base = pointer;
    if (g_ctor != 1 || pointer->Get() != 0) { printf("FAIL ctor\n"); return 1; }
    delete base;
    if (g_dtor != 1 || g_last != (const void*)pointer) { printf("FAIL dtor\n"); return 2; }
    { CIntelligentPointer<CThingBuilding> local; }
    if (g_dtor != 2) { printf("FAIL local\n"); return 3; }
    printf("0050f3a0_TEST PASS\n");
    return 0;
}
