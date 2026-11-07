#ifndef FABLE_ENGINE_CV_BEGGARANDCHILDSCRIPT_H
#define FABLE_ENGINE_CV_BEGGARANDCHILDSCRIPT_H
// Retail opt-in (chunk 813): NScript::CV_BeggarAndChildScript, new header (no includer before chunk 813; the landed sibling rows
// keep their local models).
// Layout: Ego_r.pdb CV_BeggarAndChildScript (struct_layouts_egor.tsv, 120 bytes) at retail offsets = PDB - 0x14 (retail
// CLevelScriptBase is 0x48): BeggarHit +0x48, BeggarLeft +0x49, BullyLeft +0x4A, BullyHit +0x4B, TaughtBelch +0x4C, TaughtSneer
// +0x4D, TaughtThanks +0x4E, TaughtBattleCry +0x4F, BullyHasTaunted +0x50, BelchedAtBeggar +0x54 (long), BelchedAtBully +0x58,
// QuestCardGiven +0x59, ExpressionTutorialShown +0x5A, TauntTimer +0x5C, BeggarReplyTimer +0x60, sizeof 0x64. CBeggarBully::Main
// 0x00E5B3B0 reads them through ParentClass. Methods: Ego_r.pdb / FableWin.pdb ?RegisterMain / ?Main / ?Init / ?OnPersist
// @CV_BeggarAndChildScript@NScript@@ (landed 0x00E57C60, 0x00E57E60, 0x00E57C00, 0x00E57D20).
// Nested CBeggarBully (Ego_r.pdb, 28 bytes): CEntityScriptBase (0x14), CV_BeggarAndChildScript* ParentClass +0x14,
// CQ_SunnyvaleMasterData* PMasterData +0x18. Entity vtable 0x012E6C1C (stored by the Alloc 0x00E5B2D0, reference 0x00E5B306): slot 0
// scalar deleting dtor 0x00E5B390, 1 Main 0x00E5B3B0, 2 Init 0x00E5B380, 3 GetParentScript 0x00E5B370. Names: Ego_r.pdb /
// FableWin.pdb ?Main@CBeggarBully@CV_BeggarAndChildScript@NScript@@EAEXXZ, ?Init@..@EAEXXZ, ?GetParentScript@..@UBEPAVCScriptBase@@XZ,
// ?Alloc@..@SI?AV?$CCountedPointer@VCEntityScriptBase@@@@AAV23@PAVCScriptDataBase@@ABVCScriptThing@@@Z.
// Nested CLookoutPointBeggar (chunk 816; Ego_r.pdb, 28 bytes, same shape as CBeggarBully): ParentClass +0x14, PMasterData +0x18.
// Entity vtable 0x012E68C4 (stored by the Alloc 0x00E58C60): slot 0 scalar deleting dtor 0x00E58D20, 1 Main 0x00E58D40, 2 Init
// 0x00E58D10, 3 GetParentScript 0x00E58D00. Names: Ego_r.pdb / FableWin.pdb ?Main@CLookoutPointBeggar@CV_BeggarAndChildScript@NScript@@EAEXXZ.
#ifdef FABLE_RETAIL_NSCRIPT   // the retail NScript model only; no #error branch (VC7.1 under Wine spins on #error)
#include "rebuild_abi.h"
#include "fable_string.h"
#include "engine/CCountedPointer.h"
#include "engine/CScriptThing.h"
#include "engine/CLevelScriptBase.h"
#include "engine/CEntityScriptBase.h"
#include "engine/CTimer.h"
class CScriptDataBase;
class CPersistContext;
namespace NScript {
class CQ_SunnyvaleMasterData;
class CV_BeggarAndChildScript : public CLevelScriptBase<CQ_SunnyvaleMasterData> {
public:
    virtual void RegisterMain();                                         // 0x00E57C60
    virtual void Main();                                                 // 0x00E57E60
    virtual void Init();                                                 // 0x00E57C00
    virtual void OnPersist(CPersistContext&);                            // 0x00E57D20
    bool BeggarHit;                                                      // +0x48
    bool BeggarLeft;                                                     // +0x49
    bool BullyLeft;                                                      // +0x4A
    bool BullyHit;                                                       // +0x4B
    bool TaughtBelch;                                                    // +0x4C
    bool TaughtSneer;                                                    // +0x4D
    bool TaughtThanks;                                                   // +0x4E
    bool TaughtBattleCry;                                                // +0x4F
    bool BullyHasTaunted;                                                // +0x50
    long BelchedAtBeggar;                                                // +0x54
    bool BelchedAtBully;                                                 // +0x58
    bool QuestCardGiven;                                                 // +0x59
    bool ExpressionTutorialShown;                                        // +0x5A
    CTimer TauntTimer;                                                   // +0x5C
    CTimer BeggarReplyTimer;                                             // +0x60
    class CLookoutPointBeggar : public CEntityScriptBase {
    public:
        static CCountedPointer<CEntityScriptBase> FABLE_FASTCALL Alloc(CV_BeggarAndChildScript&, CScriptDataBase*, const CScriptThing&); // 0x00E58C60
        virtual CScriptBase* GetParentScript() const;                    // 0x00E58D00
    protected:
        CV_BeggarAndChildScript* ParentClass;                            // +0x14
        CQ_SunnyvaleMasterData* PMasterData;                             // +0x18
    private:
        virtual void Init();                                             // 0x00E58D10
        virtual void Main();                                             // 0x00E58D40
    };
    class CBeggarBully : public CEntityScriptBase {
    public:
        static CCountedPointer<CEntityScriptBase> FABLE_FASTCALL Alloc(CV_BeggarAndChildScript&, CScriptDataBase*, const CScriptThing&); // 0x00E5B2D0
        virtual CScriptBase* GetParentScript() const;                    // 0x00E5B370
    protected:
        CV_BeggarAndChildScript* ParentClass;                            // +0x14
        CQ_SunnyvaleMasterData* PMasterData;                             // +0x18
    private:
        virtual void Init();                                             // 0x00E5B380
        virtual void Main();                                             // 0x00E5B3B0
    };
};
}
FABLE_STATIC_ASSERT(sizeof(NScript::CV_BeggarAndChildScript) == 0x64);
FABLE_STATIC_ASSERT(sizeof(NScript::CV_BeggarAndChildScript::CBeggarBully) == 0x1c);
FABLE_STATIC_ASSERT(sizeof(NScript::CV_BeggarAndChildScript::CLookoutPointBeggar) == 0x1c);

#endif // FABLE_RETAIL_NSCRIPT
#endif // FABLE_ENGINE_CV_BEGGARANDCHILDSCRIPT_H
