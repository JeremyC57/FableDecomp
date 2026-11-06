#ifndef FABLE_ENGINE_CQ_BANDITCAMPBOSSBATTLESCRIPT_H
#define FABLE_ENGINE_CQ_BANDITCAMPBOSSBATTLESCRIPT_H
// Retail opt-in (chunk 783): NScript::CQ_BanditCampBossBattleScript, new header (no includer before chunk 783).
// Layout: Ego_r.pdb CQ_BanditCampBossBattleScript (struct_layouts_egor.tsv, 132 bytes) at retail offsets = PDB - 0x14 (retail
// CLevelScriptBase is 0x48): the flags +0x48..+0x54, KingHealth +0x58, flags +0x5c..+0x65, AngryBanditNeeded +0x68,
// BanditsNeededForCutscene +0x6c, PubGameChatted +0x6d -- the offsets the landed rows read (Init 0x00D03610 writes the flags,
// CCampHostage2::Main 0x00D08790 reads HostagesRescued +0x4e / HostageKilled +0x4f, CGate3Guard::Main 0x00D07490 reads
// Gate3Open +0x4d). Retail methods: ctor 0x00D035E0, Init 0x00D03610, RegisterMain 0x00D03820, WatchForTermination 0x00D04260.
// Nested entity script: only CGate3Guard so far (Ego_r.pdb CGate3Guard, 28 bytes: CEntityScriptBase 0x14, ParentClass +0x14,
// PMasterData +0x18; vtable 0x012C8E9C: 1 Main 0x00D07490, 2 Init 0x00D07410, 3 GetParentScript 0x00D073E0; Alloc 0x00D07340).
// Base: CLevelScriptBase<CQ_SunnyvaleMasterData>, as the Sunnyvale quest scripts (the master-data type reaches no mangled name
// used here). ParentClass is a reference as in the other quest headers (engine/CV_DemonDoorsScript.h, CQ_ArenaScript.h).
#ifndef FABLE_RETAIL_NSCRIPT
#error engine/CQ_BanditCampBossBattleScript.h is the retail NScript model: define FABLE_RETAIL_NSCRIPT before including it
#else
#include "rebuild_abi.h"
#include "fable_string.h"
#include "engine/CCountedPointer.h"
#include "engine/CScriptThing.h"
#include "engine/CLevelScriptBase.h"
#include "engine/CEntityScriptBase.h"
namespace NScript {
class CQ_SunnyvaleMasterData;
class CQ_BanditCampBossBattleScript : public CLevelScriptBase<CQ_SunnyvaleMasterData> {
public:
    virtual void RegisterMain();                                         // 0x00D03820
    virtual void Main();
    virtual void Init();                                                 // 0x00D03610
    void WatchForTermination();                                          // 0x00D04260
    bool AttackedOuterGateGuards;                                        // +0x48
    bool AttackedGate2OuterGuard;                                        // +0x49
    bool AttackedGate3OuterGuard;                                        // +0x4a
    bool Gate1Open;                                                      // +0x4b
    bool Gate2Open;                                                      // +0x4c
    bool Gate3Open;                                                      // +0x4d
    bool HostagesRescued;                                                // +0x4e
    bool HostageKilled;                                                  // +0x4f
    bool ItsAllOver;                                                     // +0x50
    bool TheresaFrescoShown;                                             // +0x51
    bool BanditKingFightStarted;                                         // +0x52
    bool BanditKingFightEnded;                                           // +0x53
    bool BanditKingCrowdSpawn;                                           // +0x54
    long KingHealth;                                                     // +0x58 (PDB int)
    bool SpokenToSecondGuard;                                            // +0x5c
    bool MissionSucceeded;                                               // +0x5d
    bool MissionFailed;                                                  // +0x5e
    bool Gate1Passed;                                                    // +0x5f
    bool AssassinsUnderAttack;                                           // +0x60
    bool AssassinCutsceneTriggered;                                      // +0x61
    bool TalkedToAssassin;                                               // +0x62
    bool PlayGuardTooCloseCutscene;                                      // +0x63
    bool TwinBladeKilled;                                                // +0x64
    bool TwinBladeAttacked;                                              // +0x65
    long AngryBanditNeeded;                                              // +0x68
    bool BanditsNeededForCutscene;                                       // +0x6c
    bool PubGameChatted;                                                 // +0x6d
    class CGate3Guard : public CEntityScriptBase {
    public:
        static CCountedPointer<CEntityScriptBase> FABLE_FASTCALL Alloc(CQ_BanditCampBossBattleScript&, CScriptDataBase*, const CScriptThing&);   // 0x00D07340
        virtual CScriptBase* GetParentScript() const;                    // slot 3 0x00D073E0
    private:
        virtual void Main();                                             // slot 1 0x00D07490
        virtual void Init();                                             // slot 2 0x00D07410
    protected:
        NScript::CQ_BanditCampBossBattleScript & ParentClass;            // +0x14
    protected:
        NScript::CQ_SunnyvaleMasterData * PMasterData;                   // +0x18
    };
};
}
FABLE_STATIC_ASSERT(sizeof(NScript::CQ_BanditCampBossBattleScript) == 0x70);
FABLE_STATIC_ASSERT(sizeof(NScript::CQ_BanditCampBossBattleScript::CGate3Guard) == 0x1c);

#endif // FABLE_RETAIL_NSCRIPT
#endif // FABLE_ENGINE_CQ_BANDITCAMPBOSSBATTLESCRIPT_H
