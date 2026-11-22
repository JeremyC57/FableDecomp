#ifndef FABLE_ENGINE_NCREATUREMODE_CMODESITFLOOR_H
#define FABLE_ENGINE_NCREATUREMODE_CMODESITFLOOR_H
// NCreatureMode::CModeSitBase / CModeSitFloor (new shared header, chunk 1010b, row 0x008A8C60
// CGameScriptInterface::EntitySetAsSittingOnFloor): the creature sitting modes. RTTI .?AVCModeSitBase@NCreatureMode@@ :
// CTCCreatureModeManager::CModeBase; .?AVCModeSitFloor@NCreatureMode@@ : CModeSitBase (ghidra_out/rtti_bases.tsv).
// Layout (Ego_r.pdb, no STL containers, so retail agrees): CModeBase 0x1C (engine/CTCCreatureModeManager.h), CModeSitBase
// IdleCounter +0x1C, PlayingIdle +0x20, PRemoveAction +0x24 (CCountedPointer<CCreatureActionBase>), size 0x2C; CModeSitFloor
// Filter +0x2C (CAnimFilterOfGroup: vptr, CCharString Group, GroupCRC; opaque here), size 0x38 (`push 38h ; call new`
// 0x008A8D41). Ctors (Ego_r.pdb / FableWin.pdb): ??0CModeSitBase@NCreatureMode@@QAE@AAVCThing@@@Z,
// ??0CModeSitFloor@NCreatureMode@@QAE@AAVCThing@@ABVCCharString@@@Z (retail 0x008AC3C0, `ret 8`), virtual dtors
// ??1CModeSitBase@NCreatureMode@@UAE@XZ / ??1CModeSitFloor@NCreatureMode@@UAE@XZ. Declarations only. Needs the includer's
// engine/CCountedPointer.h and engine/CTCCreatureModeManager.h (FABLE_RETAIL_CTCBASE chain).
#include "rebuild_abi.h"
#include "engine/CCountedPointer.h"
#include "engine/CTCCreatureModeManager.h"
#ifdef FABLE_RETAIL_NCREATUREMODE_CMODESITBENCH_PASS27
#include "engine/CAnimFilterOfGroup.h"
#endif
class CThing;
class CCharString;
class CCreatureActionBase;
#ifdef FABLE_RETAIL_NCREATUREMODE_CMODESITFLOOR_GETANIMFILTER
// agentFAM04 (2026-11-11): CModeSitFloor's GetAnimFilter override (row 0x00740000), slot 4 of CModeBase.
class CAnimFilterBase;
#endif // FABLE_RETAIL_NCREATUREMODE_CMODESITFLOOR_GETANIMFILTER
namespace NCreatureMode {
class CModeSitBase : public CTCCreatureModeManager::CModeBase {
public:
    CModeSitBase(CThing& thing);
    virtual ~CModeSitBase();
#ifdef FABLE_RETAIL_NCREATUREMODE_CMODESITBASE_FRAMEUPDATE
    // NF83 opt-in (round 7, row 0x008D6C80): Ego_r.pdb 0x00818010 / FableWin.pdb 0x02842320 ?FrameUpdate@CModeSitBase@NCreatureMode@@UAEXXZ,
    // the CModeBase slot-3 override (needs FABLE_RETAIL_CMODEBASE_VIRTUALS). Declaration only: an override adds no slot and no layout.
    virtual void FrameUpdate();                                          // slot 3, 0x008D6C80
#endif
#ifdef FABLE_RETAIL_NCREATUREMODE_CMODESITBASE_ANIMUPDATE
    // NF128 opt-in (q6453, row 0x008D6AC0): Ego_r.pdb 0x00817E40 / FableWin.pdb 0x02841E30 ?AnimUpdate@CModeSitBase@NCreatureMode@@UAEXAAVCAnimateThing@@@Z, the CModeBase
    // slot-2 override (CModeSitChair vtable 0x01267B74 slot 2; needs FABLE_RETAIL_CMODEBASE_VIRTUALS). Declaration only: no layout change.
    virtual void AnimUpdate(CAnimateThing& anim);                        // slot 2, 0x008D6AC0
#endif
#ifdef FABLE_RETAIL_NCREATUREMODE_CMODESITBASE_ONACTIVATE
    // NF124 opt-in (q6453 / q6454, row 0x008D6580): Ego_r.pdb 0x008178B0 / FableWin.pdb 0x02841C90 ?OnActivate@CModeSitBase@NCreatureMode@@UAEXXZ,
    // the CModeBase slot-5 override (needs FABLE_RETAIL_CMODEBASE_VIRTUALS). Declaration only: an override adds no slot and no layout.
    virtual void OnActivate();                                           // slot 5, 0x008D6580
#endif
#ifdef FABLE_RETAIL_NCREATUREMODE_CMODESITBASE_REMOVEMODEWITHACTION
    // NF83 opt-in (round 7, row 0x008D7230): Ego_r.pdb 0x008185A0 / FableWin.pdb 0x028423A0
    // ?RemoveModeWithAction@CModeSitBase@NCreatureMode@@QAEXABVCCreatureActionBase@@@Z (public; PRemoveAction.Reset(action.Clone())),
    // out of line in retail at 0x008D7230. Declaration only: no layout change.
    void RemoveModeWithAction(const CCreatureActionBase& action);        // 0x008D7230
#endif
protected:
    long IdleCounter;                                                    // +0x1c
    bool PlayingIdle;                                                    // +0x20
    CCountedPointer<CCreatureActionBase> PRemoveAction;                  // +0x24
};
class CModeSitFloor : public CModeSitBase {
public:
#ifdef FABLE_RETAIL_NCREATUREMODE_ALLOC
    // Opt-in (row 0x0073B130 CTCCreatureModeManager::RegisterModes): FableWin.pdb ?Alloc@CModeSitFloor@NCreatureMode@@SIPAVCModeBase@CTCCreatureModeManager@@AAVCThing@@@Z.
    static CTCCreatureModeManager::CModeBase* __fastcall Alloc(CThing& owner);
#endif
    CModeSitFloor(CThing& thing, const CCharString& group);              // 0x008AC3C0
    virtual ~CModeSitFloor();
    virtual unsigned long GetType() const;                               // MODE_SIT_FLOOR
#ifdef FABLE_RETAIL_NCREATUREMODE_CMODESITFLOOR_GETANIMFILTER
// agentFAM04 (2026-11-11): Ego_r.pdb / FableWin.pdb ?GetAnimFilter@CModeSitFloor@NCreatureMode@@UBEABVCAnimFilterBase@@XZ, CModeBase slot 4
// (TLC CModeSitFloor vtable slot 4 = 0x00740000, `lea eax,[ecx+2Ch]; ret`: the Filter member). Declaration only.
    virtual const CAnimFilterBase& GetAnimFilter() const;               // slot 4, 0x00740000
#endif // FABLE_RETAIL_NCREATUREMODE_CMODESITFLOOR_GETANIMFILTER
private:
    unsigned char Filter[0xc];                                           // +0x2c CAnimFilterOfGroup
};
#ifdef FABLE_RETAIL_NCREATUREMODE_CMODESITBENCH_PASS27
// Both PDBs and retail allocation/constructor at 0x00938421 / 0x00740040.
class CModeSitBench : public CModeSitBase {
public:
#ifdef FABLE_RETAIL_NCREATUREMODE_CMODESITBENCH_ALLOC
    // NF238 opt-in (row 0x00742790): Ego_r.pdb / FableWin.pdb / Ego_d.pdb
    // ?Alloc@CModeSitBench@NCreatureMode@@SIPAVCModeBase@CTCCreatureModeManager@@AAVCThing@@@Z, the MODE_SIT_BENCH factory that
    // CTCCreatureModeManager::RegisterModes 0x0073B130 stores. Declaration only: a static member adds no slot and no layout.
    static CTCCreatureModeManager::CModeBase* __fastcall Alloc(CThing& owner);   // 0x00742790
#endif
    CModeSitBench(CThing&);
    virtual ~CModeSitBench();
    virtual unsigned long GetType() const;
    virtual const CAnimFilterBase& GetAnimFilter() const;
private:
    CAnimFilterOfGroup Filter;
};
FABLE_STATIC_ASSERT_AT(85, sizeof(CModeSitBench) == 0x38);
#endif
#ifdef FABLE_RETAIL_NCREATUREMODE_CMODESITTAVERNSTOOL
// Retail opt-in (chunk Q22, row 0x0096CD70 CAIStateGroup_EatAtTable::Update): RTTI .?AVCModeSitTavernStool@NCreatureMode@@ (vtable
// 0x01292FF8: slot 0 0x0094B5D0, 1 GetType 0x0094B5B0, 4 GetAnimFilter 0x0094B5C0). Ego_r.pdb: CModeSitBase + Filter
// (CAnimFilterOfGroup) +0x2C, size 0x38 (`push 38h ; call new`, 0x0096CEF4); ctor ??0CModeSitTavernStool@NCreatureMode@@QAE@AAVCThing@@@Z
// (Ego_r 0x0096F820, retail 0x0094B530: `ret 4`, stores the vtable and builds the filter). Ego_r / FableWin.pdb spell the virtual
// dtor, GetType and GetAnimFilter. Declarations only.
class CModeSitTavernStool : public CModeSitBase {
public:
    CModeSitTavernStool(CThing& thing);                                  // 0x0094B530
    virtual ~CModeSitTavernStool();
    virtual unsigned long GetType() const;                               // 0x0094B5B0
private:
    unsigned char Filter[0xc];                                           // +0x2c CAnimFilterOfGroup
};
#endif
}
FABLE_STATIC_ASSERT_AT(66, sizeof(NCreatureMode::CModeSitBase) == 0x2c);
FABLE_STATIC_ASSERT_AT(67, sizeof(NCreatureMode::CModeSitFloor) == 0x38);
#ifdef FABLE_RETAIL_NCREATUREMODE_CMODESITTAVERNSTOOL
FABLE_STATIC_ASSERT_AT(69, sizeof(NCreatureMode::CModeSitTavernStool) == 0x38);
#endif
#endif // FABLE_ENGINE_NCREATUREMODE_CMODESITFLOOR_H
