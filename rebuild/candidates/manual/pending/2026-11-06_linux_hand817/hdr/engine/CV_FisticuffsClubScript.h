#ifndef FABLE_ENGINE_CV_FISTICUFFSCLUBSCRIPT_H
#define FABLE_ENGINE_CV_FISTICUFFSCLUBSCRIPT_H
// Retail opt-in (chunk 813): NScript::CV_FisticuffsClubScript::CFisticuffsMember, new header. The owner class itself is the
// existing engine/NScript_CV_FisticuffsClubScript.h (Ego_r.pdb CV_FisticuffsClubScript, 220 bytes = retail 0xC8; its fields at
// PDB - 0x14: HeroInFisticuffsClub +0x53, HeroBeatTyler +0x55, FighterOneInRing +0x4C, FighterTwoInRing +0x4D, ChallengeAccepted
// +0x4E, FightFinished +0x4F, HeroPaidEntryFee +0x52, Fighter1Timer +0x5C, Fighter2Timer +0x60, SFighterOne +0x64, SFighterTwo
// +0x70, EverybodyAttack +0x8C, FisticuffsActive +0x8E, PlayerLost +0xA8, CheerNumber +0xAC, BooNumber +0xB0), which forward-
// declares the nested entity; this header includes it and defines the entity. No includer before chunk 813.
// Nested CFisticuffsMember (Ego_r.pdb, 48 bytes): CEntityScriptBase (0x14), CV_FisticuffsClubScript* ParentClass +0x14,
// CQ_SunnyvaleMasterData* PMasterData +0x18, bool PerformingCheerOrBoo +0x1C, long FC_Status +0x20, CTimer ShoutTimer +0x24,
// CCharString CheerName +0x28, CCharString BooName +0x2C. Entity vtable 0x012EB0F8 (stored by the ctor 0x00E83920): slot 0
// scalar deleting dtor 0x00E839D0, 1 Main 0x00E84B80, 2 Init 0x00E83A20, 3 GetParentScript 0x00E83980, 5 OnPredicateFail
// 0x00E83990. Names: Ego_r.pdb / FableWin.pdb ?Main@CFisticuffsMember@CV_FisticuffsClubScript@NScript@@EAEXXZ,
// ?Init@..@EAEXXZ, ?OnPredicateFail@..@EAEXXZ, ?Alloc@..@SI?AV?$CCountedPointer@VCEntityScriptBase@@@@AAV23@PAVCScriptDataBase@@ABVCScriptThing@@@Z.
#ifdef FABLE_RETAIL_NSCRIPT   // the retail NScript model only; no #error branch (VC7.1 under Wine spins on #error)
#include "rebuild_abi.h"
#include "fable_string.h"
#include "engine/CCountedPointer.h"
#include "engine/CScriptThing.h"
#include "engine/CEntityScriptBase.h"
#include "engine/CTimer.h"
#include "engine/NScript_CV_FisticuffsClubScript.h"
class CScriptDataBase;
namespace NScript {
class CV_FisticuffsClubScript::CFisticuffsMember : public CEntityScriptBase {
public:
    static CCountedPointer<CEntityScriptBase> FABLE_FASTCALL Alloc(CV_FisticuffsClubScript&, CScriptDataBase*, const CScriptThing&);
    virtual CScriptBase* GetParentScript() const;                        // 0x00E83980 (slot 3)
protected:
    CV_FisticuffsClubScript* ParentClass;                                // +0x14
    CQ_SunnyvaleMasterData* PMasterData;                                 // +0x18
private:
    bool PerformingCheerOrBoo;                                           // +0x1C
    long FC_Status;                                                      // +0x20 (CV_FisticuffsClubScript::FC_STATUS_*)
    CTimer ShoutTimer;                                                   // +0x24
    CCharString CheerName;                                               // +0x28
    CCharString BooName;                                                 // +0x2C
    virtual void Init();                                                 // 0x00E83A20
    virtual void OnPredicateFail();                                      // 0x00E83990
    virtual void Main();                                                 // 0x00E84B80
};
}
FABLE_STATIC_ASSERT(sizeof(NScript::CV_FisticuffsClubScript) == 0xc8);
FABLE_STATIC_ASSERT(sizeof(NScript::CV_FisticuffsClubScript::CFisticuffsMember) == 0x30);

#endif // FABLE_RETAIL_NSCRIPT
#endif // FABLE_ENGINE_CV_FISTICUFFSCLUBSCRIPT_H
