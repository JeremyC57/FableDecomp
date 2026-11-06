#ifndef FABLE_ENGINE_CQ_GUILDTRAININGMELEESCRIPT_H
#define FABLE_ENGINE_CQ_GUILDTRAININGMELEESCRIPT_H
// Retail opt-in (chunk 810): NScript::CQ_GuildTrainingMeleeScript, new header (the landed rows of this script carry local models).
// Layout: Ego_r.pdb CQ_GuildTrainingMeleeScript (struct_layouts_egor.tsv, 108 bytes) at retail offsets = PDB - 0x14 (retail
// CLevelScriptBase is 0x48; no STLport container in the class): GenericTutorialCounter +0x48, TutorialState +0x4c, TalkedToWhisper
// +0x50, WhisperArrived +0x51, EarlyHitWhisper +0x52, WhisperStopWalking +0x53, MeleeRepeating +0x54, MeleeRepeatKnown +0x55,
// MeleeOpponentReset +0x56, sizeof 0x58 (the landed Alloc 0x00D5A9F0 news 0x58). Checked against retail: CMeleeOpponent::Main
// 0x00D56790 reads +0x48 / +0x4c / +0x50 / +0x53 / +0x54 / +0x55 and writes +0x48 / +0x51 / +0x52 / +0x56; CMeleeThunder::Main
// 0x00D58080 reads TutorialState +0x4c.
// Script vtable 0x012D0274 (Alloc 0x00D5A9F0, "Q_GuildTrainingMelee"): RegisterMain 0x00D55DD0, Main 0x00D55E90, Init 0x00D55DA0.
// Nested CMeleeOpponent (Ego_r.pdb, 36 bytes = the retail Alloc new size): CEntityScriptBase (0x14), ParentClass +0x14 (a pointer in
// the PDB; the PDB ctor ??0CMeleeOpponent@..@QAE@AAV12@.. takes the parent by reference), PMasterData +0x18, BadHit +0x1c,
// RepeatMelee +0x20. Entity vtable 0x012D0328 (stored by the landed Alloc 0x00D566A0, registration "MeleeOpponent" 0x00D55F42 in
// Main 0x00D55E90): 0 ??_E 0x00D56770, 1 Main 0x00D56790, 2 Init 0x00D56750, 3 GetParentScript 0x00D56740, 4..6 the
// CEntityScriptBase defaults 0x00CDEBC0 / 0x00CDEBD0 / 0x00CDEBE0. Methods: the Ego_r.pdb / FableWin.pdb name set
// ?<Name>@CMeleeOpponent@CQ_GuildTrainingMeleeScript@NScript@@...
// Base: CLevelScriptBase<CQ_SunnyvaleMasterData> (the entity scripts' PMasterData is CQ_SunnyvaleMasterData*).
#ifdef FABLE_RETAIL_NSCRIPT   // the retail NScript model only; no #error branch (VC7.1 under Wine spins on #error)
#include "rebuild_abi.h"
#include "fable_string.h"
#include "engine/CCountedPointer.h"
#include "engine/CScriptThing.h"
#include "engine/CLevelScriptBase.h"
#include "engine/CEntityScriptBase.h"
namespace NScript {
class CQ_SunnyvaleMasterData;
class CQ_GuildTrainingMeleeScript : public CLevelScriptBase<CQ_SunnyvaleMasterData> {
public:
    virtual void RegisterMain();                                         // 0x00D55DD0 (slot 1)
    virtual void Main();                                                 // 0x00D55E90 (slot 2)
    virtual void Init();                                                 // 0x00D55DA0 (slot 3)
    long GenericTutorialCounter;                                         // +0x48
    long TutorialState;                                                  // +0x4c
    bool TalkedToWhisper;                                                // +0x50
    bool WhisperArrived;                                                 // +0x51
    bool EarlyHitWhisper;                                                // +0x52
    bool WhisperStopWalking;                                             // +0x53
    bool MeleeRepeating;                                                 // +0x54
    bool MeleeRepeatKnown;                                               // +0x55
    bool MeleeOpponentReset;                                             // +0x56
    class CTheRealGuildmaster;
    class CMeleeThunder;                                                 // Main 0x00D58080
    class CMeleeOpponent : public CEntityScriptBase {
    public:
        CMeleeOpponent(CQ_GuildTrainingMeleeScript&, CScriptDataBase*, const CScriptThing&);
        static CCountedPointer<CEntityScriptBase> FABLE_FASTCALL Alloc(CQ_GuildTrainingMeleeScript&, CScriptDataBase*, const CScriptThing&); // 0x00D566A0
        virtual CScriptBase* GetParentScript() const;                    // 0x00D56740
    protected:
        CQ_GuildTrainingMeleeScript* ParentClass;                        // +0x14
        CQ_SunnyvaleMasterData* PMasterData;                             // +0x18
    private:
        long BadHit;                                                     // +0x1c
        bool RepeatMelee;                                                // +0x20
        virtual void Init();                                             // 0x00D56750
        virtual void Main();                                             // 0x00D56790
    };
};
}
FABLE_STATIC_ASSERT(sizeof(NScript::CQ_GuildTrainingMeleeScript) == 0x58);
FABLE_STATIC_ASSERT(sizeof(NScript::CQ_GuildTrainingMeleeScript::CMeleeOpponent) == 0x24);

#endif // FABLE_RETAIL_NSCRIPT
#endif // FABLE_ENGINE_CQ_GUILDTRAININGMELEESCRIPT_H
