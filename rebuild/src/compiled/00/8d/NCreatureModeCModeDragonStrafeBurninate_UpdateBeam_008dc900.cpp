// NCreatureMode::CModeDragonStrafeBurninate::UpdateBeam(float progress) -- while the flame is active and the def's burninate interval has passed since the last explosion: place a point along the flame line by how far progress is between the start and stop percentages, and when that point is on the world map drop it to the ground, create a DRAGON_BURNINATE_EXPLOSION there, make the dragon immune to it and restamp the explosion frame
// Leaf: ?UpdateBeam@CModeDragonStrafeBurninate@NCreatureMode@@AAEXM@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x008DC900, 0x1F6 bytes, `ret 4`.
// Callees: CDefClassBase::GetSubDef<CJackDragonDef> 0x008E3300 on the parent's base def (read in place: CThing::PeekBaseDef, +0x70, no AddRef),
//   CWorld::GetFrame 0x0049D870 (static __fastcall, twice), CCharString 0x0099EBF0 / ~ 0x0099EAE0, NGraphicAppearance::CTCGraphicAppearance::
//   GetDummyObject 0x004C08C0 (CThing +0x64), CModeBase::GetWorld 0x0073A680 (twice: slot 4 PeekThingManager then the inline PeekWorldMap +0x28,
//   slot 3 GetThingManager then GetWorldMap), CWorldMap::IsPosInMap 0x004FC600 / GetHeightZBelowPos 0x0063E480, CGameDefinitionManager::Get
//   0x0044C6B0 / CDefinitionManager::GetDefGlobalIndexFromName 0x009AD410, CTCDExplosion::Create 0x0080C460 (static __fastcall),
//   CVectorMap LowerBound 0x0040F020 (GetTC of TCI_EXPLOSION 0x41), CTCDExplosion::SetThingAsImmune 0x0080CBF0; the def's release (slot 1).
// Data: "DRAGON_MOUTH_DUMMY" 0x01285B4C, "DRAGON_BURNINATE_EXPLOSION" 0x01285BB8, 1.0f.
// Layout: Ego_r.pdb CModeDragonStrafeBurninate : CModeDragonFlying (retail sizeof 0x94): Timer +0x94, PBeamWeaponEffect +0x98,
//   LastExplodeFrame +0xA0, FlameStartPos +0xA4, FlameEndPos +0xB0, FlameActive +0xBC, FlameStartPercentage +0xC0, FlameStopPercentage +0xC4
//   (the landed sibling 0x008DC6C0's model); CJackDragonDef FramesBetweenBurninateUpdate +0x58.
// Naming (NF135, 2026-11-20; q6453 / q6455): moved off the local CCharString / CDefVirtual / CDefBase / CDragonDef / CDefClassBase /
//   CTCGraphicAppearance / CWorldMap / CWorldHolder / CDisplayEngine / CThing / CTCDependents / CDefinitionManager / CGameDefinitionManager / CWorld
//   models, the same-TU GetSubDef stand-in and the free CreateExplosionObject stand-in onto the shared headers: the nested CModeBase (VIRTUALS:
//   GetWorld; HEAD called it GetDisplayEngine with slots 4 / 3 GetWorldA / GetWorldB: they are CWorld slot 4 PeekThingManager / slot 3
//   GetThingManager, then the inline CThingManager PeekWorldMap / GetWorldMap +0x28), engine/CThing.h (PeekBaseDef: the def read in place,
//   GetTCGraphicAppearance, GETTC_EXPANDED), engine/CJackDragonDef.h (FABLE_RETAIL_CJACKDRAGONDEF: the stand-in CDragonDef ExplodeInterval +0x58
//   is FramesBetweenBurninateUpdate; 0x008E3300 is GetSubDef<CJackDragonDef>) + engine/CDefClassBase_GetSubDef.h,
//   engine/NGraphicAppearance_CTCGraphicAppearance.h (DUMMYOBJECT), engine/CRightHandedSet.h, engine/CWorldMap.h (retail + HEIGHTZ),
//   engine/CThingManager.h (WORLDMAP), engine/CGameDefinitionManager.h / CDefinitionManager.h, engine/CTCDExplosion.h (Create, SetThingAsImmune:
//   HEAD CTCDependents::AddDependentThing on component 0x41 = TCI_EXPLOSION) and engine/CWorld.h (GetFrame). The mode classes keep the landed
//   sibling 0x008DC6C0 row-local shapes (NCreatureMode::CModeDragonFlyingBase / CModeDragonFlying / CModeDragonStrafeBurninate over the shared
//   nested CModeBase; no shared header for any of the three anywhere under rebuild/include). RELOCATION_MATCH in the third form (form 1 C2039:
//   CThingManager.h needs FABLE_RETAIL_CCLASSFACTORY, CWorldMap.h its retail branch; form 2 DIFFER(478v502): the position built straight into
//   its initialiser; a declared position assigned from the expression matches, as HEAD). Self symbol ...@NCreatureMode@@QAEXM@Z -> AAE
//   (private: Ego_r.pdb 0x0081E0B0, FableWin.pdb 0x0286A550; no landed row references 0x008DC900).
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_CCLASSFACTORY
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_CRESOURCELIST
#define FABLE_RETAIL_CDEFINITIONMANAGER
#define FABLE_RETAIL_CGAMEDEFINITIONMANAGER
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CMODEBASE_VIRTUALS
#define FABLE_RETAIL_CJACKDRAGONDEF
#define FABLE_RETAIL_CTCGRAPHICAPPEARANCE_DUMMYOBJECT
#define FABLE_RETAIL_CTCSHOTDEFINITIONBASE_COLLISION
#define FABLE_RETAIL_CTHINGMANAGER
#define FABLE_RETAIL_CTHINGMANAGER_WORLDMAP
#define FABLE_RETAIL_CWORLD
#define FABLE_RETAIL_CWORLDMAP
#define FABLE_RETAIL_CWORLDMAP_HEIGHTZ
#include "stlport/fable_containers.h"
#include "fable_string.h"
#include "engine/C3DVector.h"
#include "engine/CRightHandedSet.h"
#include "engine/ETCInterfaceType.h"
#include "engine/CDefClassBase.h"
#include "engine/CDefinitionManager.h"
#include "engine/CGameDefinitionManager.h"
#include "engine/CThing.h"
#include "engine/CIntelligentPointer.h"
#include "engine/CThingBaseDef.h"
#include "engine/CJackDragonDef.h"
#include "engine/CDefClassBase_GetSubDef.h"
#include "engine/NGraphicAppearance_CTCGraphicAppearance.h"
#include "engine/CTCDExplosion.h"
#include "engine/CTCCreatureModeManager.h"
#include "engine/CThingManager.h"
#include "engine/CWorldMap.h"
#include "engine/CWorld.h"

namespace NCreatureMode {
// The landed sibling 0x008DC6C0's row-local models (RTTI CModeDragonStrafeBurninate : CModeDragonFlying : CModeDragonFlyingBase : CModeBase;
// no shared header for the three anywhere under rebuild/include), over the shared nested CModeBase; only the member this row defines is declared.
struct __declspec(novtable) CModeDragonFlyingBase : CTCCreatureModeManager::CModeBase {
    unsigned char FlightSpline[0x14];      // +0x1c CCardinalSpline
    float         SplinePercentageComplete;   // +0x30
    float         LegAnimAlpha;            // +0x34
};
struct CModeDragonFlying : CModeDragonFlyingBase {   // retail sizeof 0x94
    unsigned char _flying[0x94 - 0x38];
};
class CModeDragonStrafeBurninate : public CModeDragonFlying {   // vtable 0x01268718 (class key V: Ego_r / FableWin, as HEAD)
private:
    void UpdateBeam(float progress);
    long                        Timer;                  // +0x94
    CIntelligentPointer<CThing> PBeamWeaponEffect;      // +0x98
    long                        LastExplodeFrame;       // +0xa0
    C3DVector                   FlameStartPos;          // +0xa4
    C3DVector                   FlameEndPos;            // +0xb0
    bool                        FlameActive;            // +0xbc
    float                       FlameStartPercentage;   // +0xc0
    float                       FlameStopPercentage;    // +0xc4
};

void CModeDragonStrafeBurninate::UpdateBeam(float progress)
{
    if (!FlameActive)
        return;
    CDefPointer<const CJackDragonDef> pdef;
    ParentThing.PeekBaseDef().GetSubDef(&pdef);
    if (CWorld::GetFrame() - LastExplodeFrame > pdef->FramesBetweenBurninateUpdate) {
        float fraction = (progress - FlameStartPercentage) / (FlameStopPercentage - FlameStartPercentage);
        C3DVector mouth;
        CRightHandedSet orientation;
        ParentThing.GetTCGraphicAppearance()->GetDummyObject("DRAGON_MOUTH_DUMMY", 0, 0, mouth, orientation, 1.0f);
        C3DVector position;
        position = (FlameEndPos - FlameStartPos) * fraction + FlameStartPos;
        if (GetWorld().PeekThingManager().PeekWorldMap().IsPosInMap(position)) {
            position.Z = GetWorld().GetThingManager().GetWorldMap().GetHeightZBelowPos(position);
            long index = CGameDefinitionManager::Get().GetDefGlobalIndexFromName("DRAGON_BURNINATE_EXPLOSION");
            CThing* explosion = CTCDExplosion::Create(index, position);
            if (explosion) {
                CTCDExplosion* tc;
                if (explosion->GetTC(TCI_EXPLOSION, &tc))
                    tc->SetThingAsImmune(ParentThing);
                LastExplodeFrame = CWorld::GetFrame();
            }
        }
    }
}

}
