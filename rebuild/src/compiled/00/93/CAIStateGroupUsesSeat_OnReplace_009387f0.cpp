// CAIStateGroup_UsesSeat::OnReplace() -- release the seat's action point when one was taken, and when sitting get up (movement 8, or 9 with the alternate get-up name), removing the movement mode if the get-up fails
// Leaf: ?OnReplace@CAIStateGroup_UsesSeat@@UAEXXZ
// Plan phase 3 (2026-11-04), CVectorMap wave: the local CVectorMap / CKeyPairCompareLess / CArray / STLport stand-ins replaced by the shared engine/CVectorMap.h (Ego_r: CVectorMap<K,V,CompareType> derives CArray<pair<K,V> >, Compare +0x0C, Dirty +0x0D); map ends read through the inherited end(). Also: re-padded by-value maps +8.
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009387F0, length 368 bytes (+ int3 padding).
// Callees: CBaseIntelligentPointer::GetPItem @ 0x00A01B50 (PSeat +0x14), the inlined GetTC 0x2d (CTCUserActionPointBase,
//          bit 13 of +0x24) and 0x31 (CTCCreatureModeManager, bit 17 of +0x24), the action-point release @ 0x007FEF40,
//          CCharString copy ctor @ 0x0099EC30 (from the globals @ 0x013BB6E0 / 0x013BB6E8) / dtor @ 0x0099EAE0,
//          CBrainOutput ctor @ 0x00835620 (implicit destructor: intelligent pointer +0x40, then the counted pointer +0x20),
//          GetUpFromSitting @ 0x0085FF10 (__fastcall), CTCCreatureModeManager::RemoveMode @ 0x0073ABA0, operator delete
//          @ 0x00BFE9BC, ~CBaseIntelligentPointer @ 0x00A01C10.
// Shape: the seat release is guarded by the flag and a live seat (out-param GetTC, miss reads the key slot); the get-up
//          name is copied from one of two global strings; an unused CBrainOutput local (0x48 bytes) is constructed and
//          destroyed around the get-up call.
// Layout: engine/CAIStateGroup_UsesSeat.h (Ego_r.pdb CAIStateGroup_UsesSeat: PSeat +0x14, GotSeatPos +0x1c, Sitting +0x1d,
//          SeatPosIndex +0x20, SeatPos +0x24, SittingOnBench +0x30) over engine/CAIStateGroupBase.h (PBrain +0x04).
// Name fix (agentNF26, 2026-11-12, FAM09 follow-up): the object binders below now use the PDB object names (Ego_r.pdb + FableWin.pdb
//   `dynamic initializer for '...'`, defining rows named by agentFAM09):
//   0x013BB6E8 GET_UP_FROM_BENCH_ANIM_NAME (anonymous namespace; initializer 0x01221B00), was g_GetUpNameAlternate.
//   0x013BB6E0 GET_UP_FROM_CHAIR_ANIM_NAME (anonymous namespace; initializer 0x01221AC0), was g_GetUpName.
// NF84 (round 7, q11045 binder): ported onto the shared headers (engine/CAIStateGroup_UsesSeat.h + CAIStateGroupBase.h GetCreature,
//   CAIBrain.h, CThingAICreature.h, CThingObject.h, CTCUserActionPointBase.h, CTCCreatureModeManager.h, ThingAlive.h IS_THING_ALIVE<T>,
//   CCountedPointer.h); the local CAIStateGroupBase / CAIStateGroup_UsesSeat / CAIBrain / CTCCreatureModeManager / CTCUserActionPointBase
//   / CCountedPointer models are gone. GetUpFromSitting is declared with its Ego_r.pdb / FableWin.pdb spelling
//   (?GetUpFromSitting@NCreatureMode@@YI_NAAVCThingCreatureBase@@JABVCCharString@@PAVCAIBrain@@@Z, the landed self row 0x0085FF10).
//   CBrainOutput has no shared header and stays local (implicit destructor, expanded here as retail does).
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CTHINGAICREATURE
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CINTELLIGENTPOINTER_DEREFERENCE
#define FABLE_RETAIL_CINTELLIGENTPOINTER_COMPARE_NULL_INT
#define FABLE_RETAIL_CAISTATEGROUPBASE
#define FABLE_RETAIL_CAISTATEGROUPBASE_MEMBERS
#define FABLE_RETAIL_CAISTATEGROUPBASE_GETCREATURE
#define FABLE_RETAIL_CAIBRAIN_GETCREATURE
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_CTCCREATUREMODEMANAGER
#define FABLE_RETAIL_CTCUSERACTIONPOINTBASE
#define FABLE_RETAIL_IS_THING_ALIVE
#include "fable_string.h"
#include "engine/C3DVector.h"
#include "engine/CThingAICreature.h"
#include "engine/CThingObject.h"
#include "engine/CAIStateGroupBase.h"
#include "engine/CAIBrain.h"
#include "engine/CAIStateGroup_UsesSeat.h"
#include "engine/CTCUserActionPointBase.h"
#include "engine/CTCCreatureModeManager.h"
#include "engine/CCountedPointer.h"
#include "engine/ThingAlive.h"

namespace { extern CCharString GET_UP_FROM_CHAIR_ANIM_NAME; }      // 0x013BB6E0
namespace { extern CCharString GET_UP_FROM_BENCH_ANIM_NAME; }      // 0x013BB6E8

class CCreatureActionBase;
class CBrainOutput {                                        // 0x48 bytes (no shared header; implicit destructor)
public:
    unsigned long Types;                                    // +0x00
    long          LookFlags;                                // +0x04
    unsigned char NavigationInput[0x18];                    // +0x08
    CCountedPointer<CCreatureActionBase> PAction;           // +0x20
    unsigned char _pad28[0x40 - 0x28];
    CIntelligentPointer<CThing> PTarget;                    // +0x40
    CBrainOutput();                                         // 0x00835620
};

namespace NCreatureMode {
bool __fastcall GetUpFromSitting(CThingCreatureBase& creature, long sit_mode_type, const CCharString& anim, CAIBrain* brain);   // 0x0085FF10
}

void CAIStateGroup_UsesSeat::OnReplace()
{
    if (GotSeatPos && IS_THING_ALIVE(PSeat)) {
        CTCUserActionPointBase* points;
        PSeat->GetTC(TCI_SINGLE_USER_ACTION_POINTS, &points);
        points->SetActionPointAsFree(SeatPosIndex);
        GotSeatPos = false;
    }
    SeatPosIndex = -1;
    if (Sitting) {
        long movement = (SittingOnBench != 0) + 8;
        CCharString anim(SittingOnBench ? GET_UP_FROM_BENCH_ANIM_NAME : GET_UP_FROM_CHAIR_ANIM_NAME);
        CBrainOutput output;
        if (!NCreatureMode::GetUpFromSitting(GetCreature(), movement, anim, PBrain)) {
            CTCCreatureModeManager* modes;
            GetCreature().GetTC(TCI_ENTITY_MODE_MANAGER, &modes);
            modes->RemoveMode(movement);
        }
        Sitting = false;
    }
}
