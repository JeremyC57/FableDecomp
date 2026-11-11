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
protected:
    long IdleCounter;                                                    // +0x1c
    bool PlayingIdle;                                                    // +0x20
    CCountedPointer<CCreatureActionBase> PRemoveAction;                  // +0x24
};
class CModeSitFloor : public CModeSitBase {
public:
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
}
FABLE_STATIC_ASSERT(sizeof(NCreatureMode::CModeSitBase) == 0x2c);
FABLE_STATIC_ASSERT(sizeof(NCreatureMode::CModeSitFloor) == 0x38);
#endif // FABLE_ENGINE_NCREATUREMODE_CMODESITFLOOR_H
