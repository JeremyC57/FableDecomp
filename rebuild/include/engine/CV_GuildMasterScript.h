#ifndef FABLE_ENGINE_CV_GUILDMASTERSCRIPT_H
#define FABLE_ENGINE_CV_GUILDMASTERSCRIPT_H
// Retail opt-in (chunk 808): NScript::CV_GuildMasterScript, new header (no includer before chunk 808; the landed sibling rows keep
// their local models).
// Layout: Ego_r.pdb CV_GuildMasterScript (struct_layouts_egor.tsv, 104 bytes) at retail offsets = PDB - 0x14 (retail
// CLevelScriptBase is 0x48; CCharString is one pointer in retail): GuildMasterDialogue bool[5] +0x48, LastDialogueSaid +0x50,
// sizeof 0x54 (the landed Alloc 0x00E92900 news 0x54; the landed GetGuildMasterSpeech 0x00E91F20 reads both members there).
// Nested CGuildMasterGameFlow (Ego_r.pdb, 28 bytes): CEntityScriptBase (0x14), ParentClass +0x14, PMasterData +0x18 (the landed
// Alloc 0x00E908D0 news 0x1C). Entity vtable 0x012EC184: 0 ??_G 0x00E90980, 1 Main 0x00E90BE0, 2 Init 0x00E909A0, 3 GetParentScript
// 0x00E90970 (all but Main landed); Main 0x00E90BE0 reads PMasterData->PostSavePosition +0x04 and calls GetGuildMasterSpeech on ParentClass.
// Methods: the Ego_r.pdb / FableWin.pdb name sets ?<Name>@CV_GuildMasterScript@NScript@@... and ?<Name>@CGuildMasterGameFlow@CV_GuildMasterScript@NScript@@...
// Base: CLevelScriptBase<CQ_SunnyvaleMasterData> (the entity scripts' PMasterData is CQ_SunnyvaleMasterData*).
#ifdef FABLE_RETAIL_NSCRIPT   // the retail NScript model only; no #error branch (VC7.1 under Wine spins on #error)
#include "rebuild_abi.h"
#include "fable_string.h"
#include "engine/CCountedPointer.h"
#include "engine/CScriptThing.h"
#include "engine/CLevelScriptBase.h"
#include "engine/CEntityScriptBase.h"
class CScriptDataBase;
namespace NScript {
class CQ_SunnyvaleMasterData;
class CV_GuildMasterScript : public CLevelScriptBase<CQ_SunnyvaleMasterData> {
public:
    virtual void RegisterMain();                                         // 0x00E90790 (slot 1)
    virtual void Main();                                                 // 0x00E90830 (slot 2)
    virtual void Init();                                                 // 0x00E90780 (slot 3)
    CCharString GetGuildMasterSpeech();                                  // 0x00E91F20
    bool GuildMasterDialogue[5];                                         // +0x48
    CCharString LastDialogueSaid;                                        // +0x50
    class CGuildMasterGameFlow : public CEntityScriptBase {
    public:
        static CCountedPointer<CEntityScriptBase> FABLE_FASTCALL Alloc(CV_GuildMasterScript&, CScriptDataBase*, const CScriptThing&); // 0x00E908D0
        virtual CScriptBase* GetParentScript() const;                    // 0x00E90970
    protected:
        CV_GuildMasterScript& ParentClass;                               // +0x14
        CQ_SunnyvaleMasterData* PMasterData;                             // +0x18
    private:
        virtual void Init();                                             // 0x00E909A0
        virtual void Main();                                             // 0x00E90BE0
    };
};
}
FABLE_STATIC_ASSERT(sizeof(NScript::CV_GuildMasterScript) == 0x54);
FABLE_STATIC_ASSERT(sizeof(NScript::CV_GuildMasterScript::CGuildMasterGameFlow) == 0x1C);

#endif // FABLE_RETAIL_NSCRIPT
#endif // FABLE_ENGINE_CV_GUILDMASTERSCRIPT_H
