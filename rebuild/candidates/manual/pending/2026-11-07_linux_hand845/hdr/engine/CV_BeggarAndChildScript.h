#ifndef FABLE_ENGINE_CV_BEGGARANDCHILDSCRIPT_H
#define FABLE_ENGINE_CV_BEGGARANDCHILDSCRIPT_H
// Retail opt-in (chunk 845): NScript::CV_BeggarAndChildScript, new header (no includer before chunk 845; the landed sibling rows
// Init 0x00E57C00 / RegisterMain 0x00E57C60 / OnPersist 0x00E57D20 keep their local models, whose member list this follows).
// Layout: Ego_r.pdb CV_BeggarAndChildScript (struct_layouts_egor.tsv, 120 bytes) at retail offsets = PDB - 0x14 (retail
// CLevelScriptBase is 0x48): BeggarHit +0x48, BeggarLeft +0x49, BullyLeft +0x4a, BullyHit +0x4b, TaughtBelch +0x4c, TaughtSneer
// +0x4d, TaughtThanks +0x4e, TaughtBattleCry +0x4f, BullyHasTaunted +0x50, BelchedAtBeggar +0x54, BelchedAtBully +0x58,
// QuestCardGiven +0x59, ExpressionTutorialShown +0x5a, TauntTimer +0x5c, BeggarReplyTimer +0x60; sizeof 0x64 (the bytes Main
// 0x00E57E60 reads: +0x48..+0x4f, +0x58, +0x5a).
// Script vtable 0x012E65E8 (Main 0x00E57E60, RegisterMain 0x00E57C60, Init 0x00E57C00); nested entity scripts (Ego_r.pdb /
// FableWin.pdb ?Alloc@<C>@CV_BeggarAndChildScript@NScript@@SI?AV?$CCountedPointer@VCEntityScriptBase@@@@AAV23@PAVCScriptDataBase@@
// ABVCScriptThing@@@Z): CLookoutPointBeggar::Alloc 0x00E58C60 ("LookoutPointBeggar" binding), CBeggarBully::Alloc 0x00E5B2D0
// ("BeggarBully" binding; the address Main stores as the binding's ClassAllocFunc).
#ifdef FABLE_RETAIL_NSCRIPT
#include "rebuild_abi.h"
#include "engine/CCountedPointer.h"
#include "engine/CScriptThing.h"
#include "engine/CLevelScriptBase.h"
#include "engine/CEntityScriptBase.h"
#include "engine/CTimer.h"
namespace NScript {
class CQ_SunnyvaleMasterData;
class CV_BeggarAndChildScript : public CLevelScriptBase<CQ_SunnyvaleMasterData> {
public:
    virtual void Main();                                                 // 0x00E57E60
    bool BeggarHit;                                                      // +0x48
    bool BeggarLeft;                                                     // +0x49
    bool BullyLeft;                                                      // +0x4a
    bool BullyHit;                                                       // +0x4b
    bool TaughtBelch;                                                    // +0x4c
    bool TaughtSneer;                                                    // +0x4d
    bool TaughtThanks;                                                   // +0x4e
    bool TaughtBattleCry;                                                // +0x4f
    bool BullyHasTaunted;                                                // +0x50
    long BelchedAtBeggar;                                                // +0x54
    bool BelchedAtBully;                                                 // +0x58
    bool QuestCardGiven;                                                 // +0x59
    bool ExpressionTutorialShown;                                        // +0x5a
    CTimer TauntTimer;                                                   // +0x5c
    CTimer BeggarReplyTimer;                                             // +0x60
    class CLookoutPointBeggar : public CEntityScriptBase {
    public:
        static CCountedPointer<CEntityScriptBase> FABLE_FASTCALL Alloc(CV_BeggarAndChildScript&, CScriptDataBase*, const CScriptThing&);   // 0x00E58C60
    };
    class CBeggarBully : public CEntityScriptBase {
    public:
        static CCountedPointer<CEntityScriptBase> FABLE_FASTCALL Alloc(CV_BeggarAndChildScript&, CScriptDataBase*, const CScriptThing&);   // 0x00E5B2D0
    };
};
} // namespace NScript
FABLE_STATIC_ASSERT(sizeof(NScript::CV_BeggarAndChildScript) == 0x64);
#endif // FABLE_RETAIL_NSCRIPT
#endif
