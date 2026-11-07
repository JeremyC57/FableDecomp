#ifndef FABLE_ENGINE_CQ_GUILDTRAININGMELEESCRIPT_H
#define FABLE_ENGINE_CQ_GUILDTRAININGMELEESCRIPT_H
// Retail opt-in (chunk 815): NScript::CQ_GuildTrainingMeleeScript, new header (the landed sibling rows keep their local models).
// Layout: Ego_r.pdb CQ_GuildTrainingMeleeScript (struct_layouts_egor.tsv, 108 bytes) at retail offsets = PDB - 0x14 (retail
// CLevelScriptBase is 0x48): GenericTutorialCounter +0x48, TutorialState +0x4c, TalkedToWhisper +0x50 .. MeleeOpponentReset +0x56,
// sizeof 0x58 -- the offsets CTheRealGuildmaster::Main 0x00D58490 reads (TutorialState +0x4c, WhisperArrived +0x51, EarlyHitWhisper
// +0x52, WhisperStopWalking +0x53, MeleeRepeating +0x54, MeleeRepeatKnown +0x55, MeleeOpponentReset +0x56, GenericTutorialCounter
// +0x48).
// Nested CTheRealGuildmaster (Ego_r.pdb, 32 bytes, the record whose ParentClass is CQ_GuildTrainingMeleeScript*): CEntityScriptBase
// (0x14), ParentClass +0x14, PMasterData +0x18, WhisperLate +0x1c, WhisperEarly +0x1d, HeroStanding +0x1e, WhisperStanding +0x1f.
// Entity vtable 0x012D030C (stored by Alloc 0x00D565C0 at 0x00D565F6): 0 scalar deleting dtor 0x00D56680, 1 Main 0x00D58490, 2 Init
// 0x00D56670, 3 GetParentScript 0x00D56660. Spellings: Ego_r.pdb / FableWin.pdb ?<Name>@CTheRealGuildmaster@CQ_GuildTrainingMeleeScript@
// NScript@@ (Main / Init EAE, GetParentScript UBE, Alloc SI). CMeleeOpponent / CMeleeThunder: Alloc 0x00D566A0 / 0x00D57FA0.
#ifdef FABLE_RETAIL_NSCRIPT
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
    long GenericTutorialCounter;                                         // +0x48
    long TutorialState;                                                  // +0x4c
    bool TalkedToWhisper;                                                // +0x50
    bool WhisperArrived;                                                 // +0x51
    bool EarlyHitWhisper;                                                // +0x52
    bool WhisperStopWalking;                                             // +0x53
    bool MeleeRepeating;                                                 // +0x54
    bool MeleeRepeatKnown;                                               // +0x55
    bool MeleeOpponentReset;                                             // +0x56
    class CTheRealGuildmaster : public CEntityScriptBase {
    public:
        static CCountedPointer<CEntityScriptBase> FABLE_FASTCALL Alloc(CQ_GuildTrainingMeleeScript&, CScriptDataBase*, const CScriptThing&); // 0x00D565C0
        virtual CScriptBase* GetParentScript() const;                    // 0x00D56660
    private:
        virtual void Init();                                             // 0x00D56670
        virtual void Main();                                             // 0x00D58490
    protected:
        CQ_GuildTrainingMeleeScript& ParentClass;                        // +0x14
        CQ_SunnyvaleMasterData* PMasterData;                             // +0x18
    private:
        bool WhisperLate;                                                // +0x1c
        bool WhisperEarly;                                               // +0x1d
        bool HeroStanding;                                               // +0x1e
        bool WhisperStanding;                                            // +0x1f
    };
    class CMeleeOpponent;                                                // Alloc 0x00D566A0
    class CMeleeThunder;                                                 // Alloc 0x00D57FA0
};
}
FABLE_STATIC_ASSERT(sizeof(NScript::CQ_GuildTrainingMeleeScript) == 0x58);
FABLE_STATIC_ASSERT(sizeof(NScript::CQ_GuildTrainingMeleeScript::CTheRealGuildmaster) == 0x20);

#ifdef FABLE_RETAIL_CQ_GUILDTRAININGMELEESCRIPT_MELEEOPPONENT
// Retail opt-in (chunk 828; drafted by chunk 810): nested CMeleeOpponent (Ego_r.pdb, 36 bytes = the retail Alloc new size 0x24):
// CEntityScriptBase (0x14), ParentClass +0x14 (a pointer in the PDB), PMasterData +0x18, BadHit +0x1c, RepeatMelee +0x20. Entity
// vtable 0x012D0328 (stored by the landed Alloc 0x00D566A0): 0 ??_E 0x00D56770, 1 Main 0x00D56790, 2 Init 0x00D56750,
// 3 GetParentScript 0x00D56740. Names: Ego_r.pdb / FableWin.pdb ?<Name>@CMeleeOpponent@CQ_GuildTrainingMeleeScript@NScript@@.
// Checked against retail: Main 0x00D56790 reads ParentClass at [esi+0x14] and its +0x48 / +0x4c / +0x50 / +0x53 / +0x54 / +0x55.
class NScript::CQ_GuildTrainingMeleeScript::CMeleeOpponent : public CEntityScriptBase {
public:
    CMeleeOpponent(CQ_GuildTrainingMeleeScript&, CScriptDataBase*, const CScriptThing&);
    static CCountedPointer<CEntityScriptBase> FABLE_FASTCALL Alloc(CQ_GuildTrainingMeleeScript&, CScriptDataBase*, const CScriptThing&); // 0x00D566A0
    virtual CScriptBase* GetParentScript() const;                        // 0x00D56740
protected:
    CQ_GuildTrainingMeleeScript* ParentClass;                            // +0x14
    CQ_SunnyvaleMasterData* PMasterData;                                 // +0x18
private:
    long BadHit;                                                         // +0x1c
    bool RepeatMelee;                                                    // +0x20
    virtual void Init();                                                 // 0x00D56750
    virtual void Main();                                                 // 0x00D56790
};
FABLE_STATIC_ASSERT(sizeof(NScript::CQ_GuildTrainingMeleeScript::CMeleeOpponent) == 0x24);
#endif // FABLE_RETAIL_CQ_GUILDTRAININGMELEESCRIPT_MELEEOPPONENT
#endif // FABLE_RETAIL_NSCRIPT
#endif // FABLE_ENGINE_CQ_GUILDTRAININGMELEESCRIPT_H
