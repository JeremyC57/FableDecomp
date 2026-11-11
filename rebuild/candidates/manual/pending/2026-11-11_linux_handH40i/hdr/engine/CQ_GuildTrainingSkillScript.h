#ifndef FABLE_ENGINE_CQ_GUILDTRAININGSKILLSCRIPT_H
#define FABLE_ENGINE_CQ_GUILDTRAININGSKILLSCRIPT_H
// Retail opt-in (chunk 816): NScript::CQ_GuildTrainingSkillScript, new header (the landed sibling rows keep their local models).
// Layout: Ego_r.pdb CQ_GuildTrainingSkillScript (struct_layouts_egor.tsv, 108 bytes) at retail offsets = PDB - 0x14 (retail
// CLevelScriptBase is 0x48): TargetsHit +0x48, TutorialState +0x4c, GenericTutorialCounter +0x50, TotalTrainingDummiesCounter +0x54,
// sizeof 0x58 (CTheRealGuildmaster::Main 0x00D5AE70 reads and writes TutorialState +0x4c).
// Nested CTheRealGuildmaster (Ego_r.pdb, 32 bytes, the record whose ParentClass is CQ_GuildTrainingSkillScript*; the landed Alloc
// 0x00D5ABE0 allocates 0x20): CEntityScriptBase (0x14), ParentClass +0x14, PMasterData +0x18, PlayerNotWarned +0x1c.
// Entity vtable 0x012D09D8 (stored by Alloc 0x00D5ABE0): 0 scalar deleting dtor 0x00D5ACA0, 1 Main 0x00D5AE70, 2 Init 0x00D5AC90,
// 3 GetParentScript 0x00D5AC80 (4 / 5 the CEntityScriptBase defaults 0x00CDEBC0 / 0x00CDEBD0). Spellings: Ego_r.pdb / FableWin.pdb
// ?<Name>@CTheRealGuildmaster@CQ_GuildTrainingSkillScript@NScript@@ (Main / Init EAE, GetParentScript UBE, Alloc SI).
#ifdef FABLE_RETAIL_NSCRIPT
#include "rebuild_abi.h"
#include "fable_string.h"
#include "engine/CCountedPointer.h"
#include "engine/CScriptThing.h"
#include "engine/CLevelScriptBase.h"
#include "engine/CEntityScriptBase.h"
namespace NScript {
class CQ_SunnyvaleMasterData;
class CQ_GuildTrainingSkillScript : public CLevelScriptBase<CQ_SunnyvaleMasterData> {
public:
    long TargetsHit;                                                     // +0x48
    long TutorialState;                                                  // +0x4c
    long GenericTutorialCounter;                                         // +0x50
    long TotalTrainingDummiesCounter;                                    // +0x54
    class CTheRealGuildmaster : public CEntityScriptBase {
    public:
        static CCountedPointer<CEntityScriptBase> FABLE_FASTCALL Alloc(CQ_GuildTrainingSkillScript&, CScriptDataBase*, const CScriptThing&); // 0x00D5ABE0
        virtual CScriptBase* GetParentScript() const;                    // 0x00D5AC80
    private:
        virtual void Init();                                             // 0x00D5AC90
        virtual void Main();                                             // 0x00D5AE70
    protected:
        CQ_GuildTrainingSkillScript& ParentClass;                        // +0x14
        CQ_SunnyvaleMasterData* PMasterData;                             // +0x18
    private:
        bool PlayerNotWarned;                                            // +0x1c
    };
};
}
FABLE_STATIC_ASSERT(sizeof(NScript::CQ_GuildTrainingSkillScript) == 0x58);
FABLE_STATIC_ASSERT(sizeof(NScript::CQ_GuildTrainingSkillScript::CTheRealGuildmaster) == 0x20);

#endif // FABLE_RETAIL_NSCRIPT
#endif // FABLE_ENGINE_CQ_GUILDTRAININGSKILLSCRIPT_H
