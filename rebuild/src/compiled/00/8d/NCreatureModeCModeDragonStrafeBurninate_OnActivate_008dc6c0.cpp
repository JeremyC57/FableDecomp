// NCreatureMode::CModeDragonStrafeBurninate::OnActivate() -- the flying activation, the leg-animation blend reset to 0, and the type-9 component switched on
// Leaf: ?OnActivate@CModeDragonStrafeBurninate@NCreatureMode@@UAEXXZ
// Plan phase 3 (2026-11-04), CVectorMap wave: the local CVectorMap / CKeyPairCompareLess / CArray / STLport stand-ins replaced by the shared engine/CVectorMap.h (Ego_r: CVectorMap<K,V,CompareType> derives CArray<pair<K,V> >, Compare +0x0C, Dirty +0x0D); map ends read through the inherited end().
// Retail: Fable.exe (TLC, VC7.1) @ 0x008DC6C0, 85 bytes (+ int3 padding), `ret`; vtable 0x01268718 slot 5.
// Callees: CModeDragonFlying::OnActivate @ 0x008DBB30 (qualified), CVectorMap::LowerBound @ 0x0040F020 (inlined out-parameter
//   GetInterface, type 9 = bit 9 of the +0x20 dword).
// Name: FableWin.pdb `?OnActivate@CModeDragonStrafeBurninate@NCreatureMode@@UAEXXZ` (slot 5).
// Layout (retail, the landed ctor 0x008E1580): CModeDragonFlyingBase LegAnimAlpha +0x34; the type-9 component's flag +0x10.
// Shape: the GetInterface result is not tested (as the landed CModeCarryingStandBase::FrameUpdate 0x008D88A0): the absent
//   path stores through the never-written out variable in the key's frame word, and VC7.1 duplicates the store and the
//   epilogue into both paths.
// Naming (NF126, 2026-11-20): the local CModeBase stand-in replaced by the shared nested CTCCreatureModeManager::CModeBase
//   (engine/CTCCreatureModeManager.h, FABLE_RETAIL_CMODEBASE_VIRTUALS); family with 0x008DBB30: the qualified CModeDragonFlying::OnActivate call now binds its PDB virtual (UAE, the self leaf of 0x008DBB30 in this hand-back).
class CTCBase;

#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#include "engine/CThing.h"
#include "engine/CVectorMap.h"
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;

struct CTCType9 {                          // the type-9 component (as the landed 0x008E7B90)
    unsigned char _pad00[0x10];
    bool          m_active;                // +0x10
};

#define FABLE_RETAIL_CMODEBASE_VIRTUALS
#include "engine/CTCCreatureModeManager.h"

namespace NCreatureMode {
struct __declspec(novtable) CModeDragonFlyingBase : CTCCreatureModeManager::CModeBase {
    unsigned char FlightSpline[0x14];      // +0x1c CCardinalSpline
    float         SplinePercentageComplete;   // +0x30
    float         LegAnimAlpha;            // +0x34
};
struct CModeDragonFlying : CModeDragonFlyingBase {   // retail sizeof 0x94
    unsigned char _flying[0x94 - 0x38];
    virtual void OnActivate();                     // 0x008DBB30
    virtual void OnDeactivate();                   // 0x008DC590
};
struct CModeDragonStrafeBurninate : CModeDragonFlying {   // vtable 0x01268718
    long          Timer;                   // +0x94
    unsigned char PBeamWeaponEffect[8];    // +0x98 CIntelligentPointer<CThing>
    long          LastExplodeFrame;        // +0xa0
    float         FlameStartPos[3];        // +0xa4
    float         FlameEndPos[3];          // +0xb0
    bool          FlameActive;             // +0xbc
    float         FlameStartPercentage;    // +0xc0
    float         FlameStopPercentage;     // +0xc4
    void EndBeamEffect();                  // 0x008D7BB0
    virtual void OnActivate();
};
}

void NCreatureMode::CModeDragonStrafeBurninate::OnActivate()
{
    CModeDragonFlying::OnActivate();
    LegAnimAlpha = 0.0f;
    CTCType9* tc;
    ParentThing.GetTC(TCI_TARGETED, &tc);
    tc->m_active = true;
}