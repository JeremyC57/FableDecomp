#ifndef FABLE_ENGINE_CQ_ENDGAMEBOSSBATTLESCRIPT_H
#define FABLE_ENGINE_CQ_ENDGAMEBOSSBATTLESCRIPT_H
// Retail opt-in (chunk 836): NScript::CQ_EndGameBossBattleScript, new header (no includer before chunk 836; the landed siblings
// CEndGameJackOfBladesBoss::Main 0x00D2FB20 and CEndGameSister::Alloc 0x00D2FC00 carry local models).
// Layout: Ego_r.pdb CQ_EndGameBossBattleScript (struct_layouts_egor.tsv, 128 bytes) at retail offsets = PDB - 0x14 (retail
// CLevelScriptBase is 0x48): IntroFinished +0x48 (the landed Jack Main reads it as its "JackReadyToFight" byte), FinalDecision +0x49,
// FinalDecisionMade +0x4a, SisterNeededForCutscene +0x4b, GuildmasterChat +0x4c, FinalCutscenePlayed +0x4d, seh_Jack +0x50 (0x10),
// Jack +0x60 (0xc), sizeof 0x6c. CEndGameSister::Main 0x00D328C0 reads +0x48 / +0x49 / +0x4a / +0x4b and writes +0x4a / +0x4d.
// Methods (Ego_r.pdb): RegisterMain, Main, JackOfBladesBossFightCutscene, JackOfBladesPhaseWatcher, JackOfBladesBossBattle (all public).
// Nested entity scripts (Ego_r.pdb ?Alloc@<Class>@CQ_EndGameBossBattleScript@NScript@@SI...): CEndGameJackOfBladesBoss, CEndGameSister,
// CSwordVoid, CTheRealGuildmaster, forward-declared only.
// Base: CLevelScriptBase<CQ_SunnyvaleMasterData>, the one CLevelScriptBase instance in the Ego_r.pdb name set.
#ifndef FABLE_RETAIL_NSCRIPT
#error engine/CQ_EndGameBossBattleScript.h is the retail NScript model: define FABLE_RETAIL_NSCRIPT before including it
#else
#include "rebuild_abi.h"
#include "fable_string.h"
#include "engine/CCountedPointer.h"
#include "engine/CScriptThing.h"
#include "engine/CLevelScriptBase.h"
#include "engine/CEntityScriptBase.h"
#include "engine/CScriptGameResourceObjectScriptedThingBase.h"
namespace NScript {
class CQ_SunnyvaleMasterData;
class CQ_EndGameBossBattleScript : public CLevelScriptBase<CQ_SunnyvaleMasterData> {
public:
    virtual void RegisterMain();
    virtual void Main();
    void JackOfBladesBossFightCutscene();
    void JackOfBladesPhaseWatcher();
    void JackOfBladesBossBattle();
    bool IntroFinished;                                                  // +0x48
    bool FinalDecision;                                                  // +0x49
    bool FinalDecisionMade;                                              // +0x4a
    bool SisterNeededForCutscene;                                        // +0x4b
    bool GuildmasterChat;                                                // +0x4c
    bool FinalCutscenePlayed;                                            // +0x4d
    CScriptGameResourceObjectScriptedThingBase seh_Jack;                 // +0x50
    CScriptThing Jack;                                                   // +0x60
    class CEndGameJackOfBladesBoss;                                      // Alloc 0x00D2FA40
    class CEndGameSister;                                                // Alloc 0x00D2FC00
    class CSwordVoid;                                                    // Alloc 0x00D2FCE0
    class CTheRealGuildmaster;
};
#ifdef FABLE_RETAIL_CQ_ENDGAMEBOSSBATTLESCRIPT_CENDGAMESISTER
// Retail opt-in (chunk 836): the nested entity script CEndGameSister (Ego_r.pdb, 0x1c: ParentClass +0x14, PMasterData +0x18,
// struct_layouts_egor.tsv; the landed Alloc 0x00D2FC00 allocates 0x1c). Methods: Ego_r.pdb ?Main@CEndGameSister@...@EAEXXZ (private
// virtual, vtable slot 1, 0x00D328C0), GetParentScript 0x00D2FCA0, Init 0x00D2FCB0, Alloc 0x00D2FC00.
class CQ_EndGameBossBattleScript::CEndGameSister : public CEntityScriptBase {
public:
    typedef CQ_EndGameBossBattleScript TParentClass;
    CEndGameSister(CQ_EndGameBossBattleScript&, CScriptDataBase*, const CScriptThing&);
    static CCountedPointer<CEntityScriptBase> FABLE_FASTCALL Alloc(CQ_EndGameBossBattleScript&, CScriptDataBase*, const CScriptThing&); // 0x00D2FC00
    virtual CScriptBase* GetParentScript() const;                        // 0x00D2FCA0
    CQ_SunnyvaleMasterData* GetMasterData();
protected:
    CQ_EndGameBossBattleScript& ParentClass;                             // +0x14
    CQ_SunnyvaleMasterData* PMasterData;                                 // +0x18
private:
    virtual void Init();                                                 // 0x00D2FCB0
    virtual void Main();                                                 // 0x00D328C0
};
#endif
}
FABLE_STATIC_ASSERT(sizeof(NScript::CQ_EndGameBossBattleScript) == 0x6c);
#ifdef FABLE_RETAIL_CQ_ENDGAMEBOSSBATTLESCRIPT_CENDGAMESISTER
FABLE_STATIC_ASSERT(sizeof(NScript::CQ_EndGameBossBattleScript::CEndGameSister) == 0x1c);
#endif

#endif // FABLE_RETAIL_NSCRIPT
#endif // FABLE_ENGINE_CQ_ENDGAMEBOSSBATTLESCRIPT_H
