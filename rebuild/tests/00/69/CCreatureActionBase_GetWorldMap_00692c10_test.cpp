// behaviour test for 0x00692C10
#include <stdio.h>
class CWorld; static CWorld* g_world;
class CBaseObject;
class CWorldMap;
class CWorld {
public:
    virtual CWorldMap& GetWorldMap() { return *(CWorldMap*)Map; }
    void* Map;
};
class CThing {
public:
    CWorld& GetWorld() { return *g_world; }
};
class CThingCreatureBase : public CThing { };
class CBaseObjectPointer;
class CBaseIntelligentPointer {                          // Ego_r.pdb: 8 bytes, PData +0x04
public:
    void*               _pad00;
    CBaseObjectPointer* PData;                           // +0x04
    CBaseObject* GetPItem() const { return (CBaseObject*)PData; }
};
template <class T> class CIntelligentPointer : public CBaseIntelligentPointer {
public:
    T* operator->() const { return (T*)GetPItem(); }
};
struct CCreatureActionBase { void* vt; void* pad; unsigned char PCreature[8]; };
struct CCreatureActionBase_Methods : CCreatureActionBase {
    CWorldMap& GetWorldMap();
};
CWorldMap& CCreatureActionBase_Methods::GetWorldMap()
{
    return (*(CIntelligentPointer<CThingCreatureBase>*)PCreature)->GetWorld().GetWorldMap();
}
int main()
{
    static CWorld w; int mapObj = 0; w.Map = &mapObj; g_world = &w;
    static CThingCreatureBase creature; CCreatureActionBase_Methods a;
    ((CBaseIntelligentPointer*)a.PCreature)->PData = (CBaseObjectPointer*)&creature;
    int ok = (void*)&a.GetWorldMap() == (void*)&mapObj;
    printf(ok ? "00692c10_TEST PASS\n" : "00692c10_TEST FAIL\n");
    return ok ? 0 : 1;
}