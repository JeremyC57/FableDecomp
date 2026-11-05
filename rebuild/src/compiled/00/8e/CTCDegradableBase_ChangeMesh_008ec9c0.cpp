// CTCDegradableBase::ChangeMesh(const CEngineGraphic&) -- install the stage's graphic as the parent's main appearance graphic, flag the appearance changed, and rebuild the physics mesh info from it
// Leaf: ChangeMesh
// Plan phase 3 (2026-11-04), CVectorMap wave: the local CVectorMap / CKeyPairCompareLess / CArray / STLport stand-ins replaced by the shared engine/CVectorMap.h (Ego_r: CVectorMap<K,V,CompareType> derives CArray<pair<K,V> >, Compare +0x0C, Dirty +0x0D); map ends read through the inherited end(). Also: re-padded by-value maps +8.
//
// Retail 0x008EC9C0, 135 bytes (`ret 4`). Ego_r.pdb `?ChangeMesh@CTCDegradableBase@@AAEXABVCEngineGraphic@@@Z`.
// Callees: CTCGraphicAppearance::SetAsChanged(unsigned long) @ 0x004BF9E0, CVectorMap<ETCInterfaceType,
// CTCBase*>::LowerBound @ 0x0040F020 (inlined GetInterface for interface 2, bit 2 of +0x20),
// CTCPhysicsBase::CreatePPhysicsMeshInfo(const CEngineGraphic&) @ 0x006B08E0.
// Layout: CEngineGraphic is the PDB's 14-byte record (BankIndex, AnimStep, RenderSizeX, AdditiveAlpha,
// Type; packed to 2, so the implicit copy is three dwords and a word); retail CThing keeps its
// graphic-appearance component at +0x64; retail CTCGraphicAppearance MainGraphic +0x44 (PDB +0x5c: the
// containers ahead of it are smaller in retail).
// Shape: the physics component comes from a `__forceinline` helper around the out-parameter
// GetInterface whose uninitialised out pointer shares the dead argument slot with the lookup key, so
// the absent path calls through whatever the slot holds (`mov eax,[esp+0Ch]`) and both arms keep their
// own call and `ret 4`.
#pragma pack(push, 2)
class CEngineGraphic { public:
    unsigned long BankIndex;                                // +0x00
    float         AnimStep;                                 // +0x04
    float         RenderSizeX;                              // +0x08
    bool          AdditiveAlpha;                            // +0x0c
    unsigned char Type;                                     // +0x0d
};
#pragma pack(pop)

class CTCBase;

#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#include "engine/CThing.h"
#include "engine/CVectorMap.h"
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;

namespace NGraphicAppearance {
struct CTCGraphicAppearance {
    unsigned char  _pad00[0x44];
    CEngineGraphic MainGraphic;                             // +0x44
    void SetAsChanged(unsigned long reasons);               // 0x004BF9E0
};
}
using NGraphicAppearance::CTCGraphicAppearance;

class CTCPhysicsBase { public:
    void CreatePPhysicsMeshInfo(const CEngineGraphic& graphic);   // 0x006B08E0
};

__forceinline CTCPhysicsBase* GetPhysics(CThing* thing)
{
    CTCPhysicsBase* physics;
    thing->GetTC(TCI_PHYSICS, &physics);                       // TC_PHYSICS
    return physics;
}

class CTCDegradableBase { public:
    virtual void v00();
    CThing* ParentThing;                                    // +0x04 (CTCBase)
    void ChangeMesh(const CEngineGraphic& graphic);
};

void CTCDegradableBase::ChangeMesh(const CEngineGraphic& graphic)
{
    ParentThing->GetTCGraphicAppearance()->MainGraphic = graphic;
    ParentThing->GetTCGraphicAppearance()->SetAsChanged(4);
    GetPhysics(ParentThing)->CreatePPhysicsMeshInfo(graphic);
}