#ifndef FABLE_ENGINE_CQ_GUILDTRAININGWILLSCRIPT_H
#define FABLE_ENGINE_CQ_GUILDTRAININGWILLSCRIPT_H
// Retail opt-in (chunk 819): NScript::CQ_GuildTrainingWillScript, new header (the landed sibling rows keep their local models).
// Layout: Ego_r.pdb CQ_GuildTrainingWillScript (struct_layouts_egor.tsv, 132 bytes) at retail offsets = PDB - 0x14 (retail
// CLevelScriptBase is 0x48), then - 4 for the STLport vector: TargetsHit +0x48, TutorialState +0x4c, TotalTrainingDummiesCounter
// +0x50, GenericTutorialCounter +0x54, HeroHit +0x58, TestFinished +0x5c, BanditsDefeated +0x5d, DummyWizardsVector +0x60
// (STLport vector, 0xc), sizeof 0x6c -- the 108 bytes the script Alloc 0x00D61BB0 news and the landed ctor 0x00D5DC50 builds; the
// offsets CTheRealGuildmaster::Main 0x00D5E0C0 reads (TutorialState +0x4c, TestFinished +0x5c, BanditsDefeated +0x5d).
// Nested CTheRealGuildmaster (Ego_r.pdb, 28 bytes, the record whose ParentClass is CQ_GuildTrainingWillScript*; the landed Alloc
// 0x00D5DDE0 allocates 0x1C): CEntityScriptBase (0x14), ParentClass +0x14, PMasterData +0x18.
// Entity vtable 0x012D1070 (stored by Alloc 0x00D5DDE0): 0 scalar deleting dtor 0x00D5DEA0, 1 Main 0x00D5E0C0, 2 Init 0x00D5DE90,
// 3 GetParentScript 0x00D5DE80. Spellings: Ego_r.pdb / FableWin.pdb ?<Name>@CTheRealGuildmaster@CQ_GuildTrainingWillScript@NScript@@
// (Main / Init EAE, GetParentScript UBE, Alloc SI).
#ifdef FABLE_RETAIL_NSCRIPT
#include "rebuild_abi.h"
#include "fable_string.h"
#include "engine/CCountedPointer.h"
#include "engine/CScriptThing.h"
#include "stlport/fable_containers.h"
#include "engine/CLevelScriptBase.h"
#include "engine/CEntityScriptBase.h"
namespace NScript {
class CQ_SunnyvaleMasterData;
class CQ_GuildTrainingWillScript : public CLevelScriptBase<CQ_SunnyvaleMasterData> {
public:
    long TargetsHit;                                                     // +0x48
    long TutorialState;                                                  // +0x4c
    long TotalTrainingDummiesCounter;                                    // +0x50
    long GenericTutorialCounter;                                         // +0x54
    long HeroHit;                                                        // +0x58
    bool TestFinished;                                                   // +0x5c
    bool BanditsDefeated;                                                // +0x5d
    std::vector<CScriptThing> DummyWizardsVector;                        // +0x60
    class CTheRealGuildmaster : public CEntityScriptBase {
    public:
        static CCountedPointer<CEntityScriptBase> FABLE_FASTCALL Alloc(CQ_GuildTrainingWillScript&, CScriptDataBase*, const CScriptThing&); // 0x00D5DDE0
        virtual CScriptBase* GetParentScript() const;                    // 0x00D5DE80
    private:
        virtual void Init();                                             // 0x00D5DE90
        virtual void Main();                                             // 0x00D5E0C0
    protected:
        CQ_GuildTrainingWillScript& ParentClass;                         // +0x14
        CQ_SunnyvaleMasterData* PMasterData;                             // +0x18
    };
};
}
FABLE_STATIC_ASSERT(sizeof(NScript::CQ_GuildTrainingWillScript) == 0x6c);
FABLE_STATIC_ASSERT(sizeof(NScript::CQ_GuildTrainingWillScript::CTheRealGuildmaster) == 0x1c);

#endif // FABLE_RETAIL_NSCRIPT
#endif // FABLE_ENGINE_CQ_GUILDTRAININGWILLSCRIPT_H
