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
#define FABLE_RETAIL_CTCCREATUREMODEMANAGER_ADDMODEPOINTER
class CTCBase;
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#include "engine/CThing.h"
#include "engine/CVectorMap.h"
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;

class CThingCreatureBase : public CThing { public: };

class CBaseObject;
struct CBaseIntelligentPointer {
    void*   __vftable;
    CThing* m_pItem;                            // +0x04
    CBaseObject* GetPItem() const;                   // 0x00A01B50
    operator CThing*() const { return ((CThing*)GetPItem()); }
};
struct CIntelligentPointerCreature : CBaseIntelligentPointer {   // CIntelligentPointer<CThingCreatureBase>
    CThingCreatureBase* GetPItem() const { return (CThingCreatureBase*)CBaseIntelligentPointer::GetPItem(); }
};

#include "engine/CTCCreatureModeManager.h"
#include "engine/CCountedPointer.h"
namespace NCreatureMode {
class CModeDragonFireBeam : public CTCCreatureModeManager::CModeBase {   // class-keyed (RTTI .?AVCModeDragonFireBeam@NCreatureMode@@)
    unsigned char _pad[0x94 - 0x1c];
public:
    CModeDragonFireBeam(CThing& owner, CThing& target, float timer);   // 0x008D86E0
    virtual unsigned long GetType() const;
};
}

class CCombatActionBase {
public:
    virtual void Initialise();                  // 0x00857520
    unsigned char           _pad04[4];
    CBaseIntelligentPointer PCreature;          // +0x08
    unsigned char           _pad10[0xa8 - 0x10];
    CBaseIntelligentPointer PTarget;            // +0xa8
    unsigned char           _padb0[0x114 - 0xb0];
    void SetCombatAnimation(const CCharString& name);   // 0x008574D0
};

class CCreatureAction_DragonFireBeamInto : public CCombatActionBase {
public:
    float Timer;                                // +0x114 (PDB +0x12c)
    virtual void Initialise();
};

void CCreatureAction_DragonFireBeamInto::Initialise()
{
    {
        CCharString name("FIRE_BEAM_INTO");
        SetCombatAnimation(name);
    }
    CCombatActionBase::Initialise();
    CTCCreatureModeManager* modeManager;
    if (((CThing*)PCreature.GetPItem())->GetTC(TCI_ENTITY_MODE_MANAGER, &modeManager))
        modeManager->AddMode(CCountedPointer<CTCCreatureModeManager::CModeBase>(new NCreatureMode::CModeDragonFireBeam(*PCreature, *PTarget, Timer)));
}