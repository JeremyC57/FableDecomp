// CCreatureAction_DragonFireBeamInto::Initialise() -- animation "FIRE_BEAM_INTO", base Initialise, then (mode manager present) push a new CModeDragonFireBeam(owner, target, Timer) onto the owner's mode manager
// Leaf: ?Initialise@CCreatureAction_DragonFireBeamInto@@UAEXXZ
// Plan phase 5 (2026-11-05), CCountedPointer wave: the local CCountedPointer / count-block stand-ins replaced by the shared engine/CCountedPointer.h (Ego_r.pdb: CCountedPointer<T> Data +0x0 / Info +0x4; CCPPointerInfo RefCount +0x0 / DeleteFunc +0x4 / Data +0x8). Also: the local CCountedPointer members m_pObject->Data, m_pCount->Info are the header CCountedPointer's (same offsets).
// Plan phase 3 (2026-11-04), CVectorMap wave: the local CVectorMap / CKeyPairCompareLess / CArray / STLport stand-ins replaced by the shared engine/CVectorMap.h (Ego_r: CVectorMap<K,V,CompareType> derives CArray<pair<K,V> >, Compare +0x0C, Dirty +0x0D); map ends read through the inherited end(). Also: re-padded by-value maps +8.
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008F41D0, length 199 bytes (+ int3 padding), plain `ret` (virtual, no arguments).
// Callees: CCharString ctor 0x0099EBF0 / dtor 0x0099EAE0 ("FIRE_BEAM_INTO" @ 0x012884CC), SetCombatAnimation 0x008574D0,
// CCombatActionBase::Initialise 0x00857520 (direct), CBaseIntelligentPointer::GetPItem 0x00A01B50, CVectorMap::LowerBound 0x0040F020
// (inlined GetInterface 0x31 = CTCCreatureModeManager), operator new 0x00BFEA1A (0x94 bytes),
// CModeDragonFireBeam::CModeDragonFireBeam(CThing*, CThing*, float) 0x008D86E0, CCountedPointer<CModeBase>::CCountedPointer(CModeBase*)
// 0x00718A00, CTCCreatureModeManager::AddMode(CCountedPointer<CModeBase>) 0x0073ACC0.
// Shape: as CCreatureAction_DragonBurninateInto::Initialise 0x008F4320 (in-chunk): both pointer arguments go through the inline
// `operator CThing*()`, so both GetPItem calls run (right to left: target, owner) before any argument is pushed; the manager
// out-parameter is then spilled to the frame across the new (plain `.GetPItem()` arguments interleave with the pushes).
// Layout: Timer at retail +0x114 (PDB +0x12c); the combat target pointer at +0xa8.
// Naming (NF144, 2026-11-20; q6408): the local empty `struct CModeBase { };` and the local CTCCreatureModeManager replaced by the shared
//   engine/CTCCreatureModeManager.h (ADDMODEPOINTER, nested CModeBase); 0x0073ACC0 now binds the Ego_r.pdb
//   ?AddMode@CTCCreatureModeManager@@QAE_NV?$CCountedPointer@VCModeBase@CTCCreatureModeManager@@@@@Z (was ...QAE_NV?$CCountedPointer@UCModeBase@@@@@Z) and
//   0x00718A00 the FableWin.pdb ??0?$CCountedPointer@VCModeBase@CTCCreatureModeManager@@@@QAE@PAVCModeBase@CTCCreatureModeManager@@@Z (was
//   ??0?$CCountedPointer@UCModeBase@@@@QAE@PAUCModeBase@@@Z). The header-less mode class keeps its token and its constructor spelling, now on
//   the shared CModeBase (the landed 0x0096DBC0 shape), its pad resized so sizeof stays the retail allocation.
// Naming (NF157, 2026-11-20; PRIORITY q11678): in place: the row's header-less local mode class sits in NCreatureMode (RTTI
//   .?AVCModeDragonFireBeam@NCreatureMode@@) and its constructor takes the two things by reference, as Ego_r.pdb 0x008195A0 / FableWin.pdb
//   0x02865640 spell it: 0x008D86E0 now binds ??0CModeDragonFireBeam@NCreatureMode@@QAE@AAVCThing@@0M@Z (was the global stand-in
//   ??0CModeDragonFireBeam@@QAE@PAVCThing@@0M@Z; the self row respells its leaf in the same hand-back). Same token, now class-keyed as both PDBs, same layout.
// NF259 (2026-11-22, q12732, nf255 port debt): moved off the row-local CThingCreatureBase (empty) / CBaseIntelligentPointer /
//   CIntelligentPointerCreature / CCombatActionBase / CCreatureAction_DragonFireBeamInto models onto the shared headers: the L92 prelude of the
//   landed class rows 0x009535B0 / 0x00953CF0 / 0x009539C0 (CCombatActionBase.h MELEE branch with 0x00955620's opt-in set) and the nf255
//   engine/CCreatureAction_DragonFireBeamInto.h INITIALISE opt-in; engine/CTCCreatureModeManager.h (ADDMODEPOINTER) as before. The
//   header-less NCreatureMode::CModeDragonFireBeam keeps the HEAD row's local declaration over the shared CModeBase.
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CINTELLIGENTPOINTER_DEREFERENCE
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_C3DVECTOR_PACKED
#define FABLE_RETAIL_CCREATUREACTIONBASE
#define FABLE_RETAIL_CCREATUREACTIONBASE_FULL_MEMBERS
#define FABLE_RETAIL_MAZE_DEFENSIVE_MELEE_PASS27
#define FABLE_RETAIL_CCOMBATACTIONBASE_MELEE
#define FABLE_RETAIL_CCOMBATACTIONBASE_MELEE_COPYCTOR
#define FABLE_RETAIL_CCOMBATACTIONBASE_FRAMEUPDATE
#define FABLE_RETAIL_CCOMBATACTIONBASE_INITIALISE
#define FABLE_RETAIL_CCOMBATACTIONBASE_FINISHACTION
#define FABLE_RETAIL_CCOMBATACTIONBASE_ONANIMATIONLOADED
#define FABLE_RETAIL_CCOMBATACTIONBASE_MELEE_OWN_VIRTUALS
#define FABLE_RETAIL_CCOMBATACTIONBASE_SOUNDCTOR
#define FABLE_RETAIL_CCOMBATACTIONBASE_TARGETCTOR
#define FABLE_RETAIL_JACKDRAGON_HOVER_PASS27
#define FABLE_RETAIL_CCREATUREACTION_DRAGONFIREBEAMINTO_INITIALISE
#define FABLE_RETAIL_CTCCREATUREMODEMANAGER_ADDMODEPOINTER
#include "fable_string.h"
#include "engine/fable_ai_action_enums.h"
#include "engine/C3DVector.h"
#include "engine/CThingCreatureBase.h"
#include "engine/CIntelligentPointer.h"
#include "engine/CCountedPointer.h"
#include "stlport/fable_containers.h"
#include "engine/CCreatureActionBase.h"
#include "engine/CCombatActionBase.h"
#include "engine/CSoundPair.h"
#include "engine/CCreatureAction_DragonFireBeamInto.h"
#include "engine/CTCCreatureModeManager.h"
namespace NCreatureMode {
class CModeDragonFireBeam : public CTCCreatureModeManager::CModeBase {   // class-keyed (RTTI .?AVCModeDragonFireBeam@NCreatureMode@@)
    unsigned char _pad[0x94 - 0x1c];
public:
    CModeDragonFireBeam(CThing& owner, CThing& target, float timer);   // 0x008D86E0
    virtual unsigned long GetType() const;
};
}

void CCreatureAction_DragonFireBeamInto::Initialise()
{
    {
        CCharString name("FIRE_BEAM_INTO");
        SetCombatAnimation(name);
    }
    CCombatActionBase::Initialise();
    CTCCreatureModeManager* modeManager;
    if (PCreature.Get()->GetTC(TCI_ENTITY_MODE_MANAGER, &modeManager))
        modeManager->AddMode(CCountedPointer<CTCCreatureModeManager::CModeBase>(new NCreatureMode::CModeDragonFireBeam(*PCreature, *PTarget, Timer)));
}
