// Behaviour test for 0082d7d0: the row body verbatim over fixture models of the shared-header interfaces (CThing::PeekDefinitionManager, CDefinitionManager::GetDef<T>, the counted CDefPointer, the inline CThingBuilding::PeekDefinition, the appearance setters).
#include <stdio.h>
#include <string.h>
static int fails = 0;
#define CHECK(c) do { if (!(c)) { ++fails; printf("FAIL %d: %s\n", __LINE__, #c); } } while (0)
// Fixture models of the shared-header interfaces: a counted def (count starts at 1 for the manager's own reference, Delete counted),
// the counted CDefPointer handle, and the definition manager's GetDef<T> (fills the handle with an add-ref, returns whether found).
class CDefPointeeBase { public:
    long Refs; int Deleted;
    CDefPointeeBase() : Refs(1), Deleted(0) { }
    void Delete() { ++Deleted; }
};
template <class T> class CDefPointer { public:
    CDefPointeeBase* Object;
    CDefPointer() : Object(0) { }
    CDefPointer(const CDefPointer& o) : Object(o.Object) { if (Object) ++Object->Refs; }
    ~CDefPointer() { if (Object) { if (--Object->Refs == 0) Object->Delete(); } }
    T* operator->() const { return static_cast<T*>(Object); }
    T& operator*() const { return *static_cast<T*>(Object); }
    bool IsNull() const { return Object == 0; }
    void Set(CDefPointeeBase* p) { if (Object && --Object->Refs == 0) Object->Delete(); Object = p; if (p) ++p->Refs; }
};
class CGameDefinitionManager { public:
    CDefPointeeBase* Table[16]; mutable long Calls; mutable long LastIndex;
    template <class T> bool GetDef(long index, CDefPointer<const T>* out) const
    {
        ++Calls; LastIndex = index;
        if (index > 0 && index < 16 && Table[index]) { out->Set(Table[index]); return true; }
        return false;
    }
};
static CGameDefinitionManager g_manager;
struct CEngineGraphic { unsigned long BankIndex; float AnimStep; float RenderSizeX; bool AdditiveAlpha; unsigned char Type; };
enum EReverbEnvironmentType { REVERB_NONE = 0, REVERB_HALL = 7 };
class CThingBuildingDef : public CDefPointeeBase { public:
    float Health; unsigned long SimBuildingDef; CEngineGraphic Graphic; long WallMaterial; long FloorMaterial;
    EReverbEnvironmentType ReverbEnvironmentType; float ReverbEnvironmentLevel; bool UseHighDetailQuadTree;
};
class CSimBuildingDef : public CDefPointeeBase { };
class CMaterialDef : public CDefPointeeBase { };
namespace NGraphicAppearance {
class CTCGraphicAppearance { public:
    CEngineGraphic MainGraphic; float ScaleSet; int ScaleCalls;
    void SetMainGraphic(const CEngineGraphic& g) { MainGraphic = g; }
    void SetScale(float s) { ScaleSet = s; ++ScaleCalls; }
};
}
class CThingGameObject { public:
    unsigned short DefGlobalIndex; NGraphicAppearance::CTCGraphicAppearance* PTCGraphicAppearance;
    float Scale, MaxHealth, Health; bool BaseResult; int BaseCalls;
    const CGameDefinitionManager& PeekDefinitionManager() const { return g_manager; }
    bool OnCreate() { ++BaseCalls; return BaseResult; }
};
class CThingBuilding : public CThingGameObject { public:
    bool OverrideAutomaticLocking;
    const CThingBuildingDef& PeekDefinition() const
    {
        CDefPointer<const CThingBuildingDef> def;
        PeekDefinitionManager().GetDef(DefGlobalIndex, &def);
        return *def;
    }
    bool OnCreate();
    CDefPointer<const CMaterialDef> PeekPMaterial() const;
    CDefPointer<const CSimBuildingDef> PeekPSimDefinition() const;
    long GetMaterialOnHitByMelee() const;
    long GetFloorMaterial() const;
    bool UseHighDetailQuadTree() const;
    void GetReverbSettings(EReverbEnvironmentType& type, float& level) const;
};
static CThingBuildingDef g_def; static CSimBuildingDef g_sim; static CMaterialDef g_mat;
static void Setup(CThingBuilding& b)
{
    memset(&g_manager, 0, sizeof(g_manager));
    g_def.Refs = 1; g_sim.Refs = 1; g_mat.Refs = 1; g_def.Deleted = g_sim.Deleted = g_mat.Deleted = 0;
    g_manager.Table[3] = &g_def; g_manager.Table[5] = &g_sim; g_manager.Table[6] = &g_mat;
    memset(&b, 0, sizeof(b)); b.DefGlobalIndex = 3;
}
// ---- row body (verbatim) ----
long CThingBuilding::GetFloorMaterial() const
{
    return PeekDefinition().FloorMaterial;
}
// ---- harness ----
int main()
{
    CThingBuilding b; Setup(b); g_def.FloorMaterial = 22;
    CHECK(b.GetFloorMaterial() == 22); CHECK(g_manager.Calls == 1); CHECK(g_def.Refs == 1);
    if (fails == 0) printf("0082d7d0_TEST PASS\n");
    return fails;
}
