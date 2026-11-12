#ifndef FABLE_ENGINE_CQ_ORCHARDFARMRAIDSCRIPT_H
#define FABLE_ENGINE_CQ_ORCHARDFARMRAIDSCRIPT_H
// NEW shared header (w22 agent2027, row 0x00DCD350 NScript::CQ_OrchardFarmRaidScript::CTeamSpawn::Main): the retail
// NScript::CQ_OrchardFarmRaidScript (no includer before this chunk; the landed rows of the class use local models).
// Layout: Ego_r.pdb CQ_OrchardFarmRaidScript (struct_layouts_egor.tsv, 292 bytes) at retail offsets = PDB - 0x14 (retail
// CLevelScriptBase is 0x48), and - 0x18 past the 12-byte retail STLport CrateList vector (PDB 16): CrateCount +0x48,
// CommentTimer +0x4c, DoneIntroduction +0x50, HeroAtWrongEntrance +0x51, WhisperSpawned +0x52, CrateList +0x54, MissionSucceeded
// +0x60, MissionFailed +0x64, FailReasons +0x68, HeroTeam +0x78, BanditWavesSpawned +0x7c, RemindHeroOfObjectivesTimer +0x80,
// TextSystemScriptCode +0x84, the four bools +0x88..+0x8b, Teams[2] +0x8c (CCrateTeamManager, 0x40 each), sizeof 0x10c.
// Retail evidence: CTeamSpawn::Main reads DoneIntroduction +0x50, WhisperSpawned +0x52, HeroTeam +0x78, BanditWavesSpawned +0x7c,
// Teams[TeamID] (shl 6) MemberCount +0xa4 / TeamReinforcementsTimer +0xa8 and stores MissionFailed +0x64; the landed
// MakeTeamMemberComment 0x00DCDA80 reads CommentTimer +0x4c / TextSystemScriptCode +0x84, the landed Main 0x00DCC770 HeroTeam +0x78 /
// Teams +0x8c (CrateDropPos +0x24), the landed CTeamSpawn::Init 0x00DCD1D0 HeroTeam +0x78.
// MissionFailed's PDB type is an unnamed 4-byte type (`<unnamed-type-MissionFailed>`, no enumerator names in the name sets): modelled
// as long (Main stores 3 into it, `mov [ecx+64h],edi` with edi = 3, 0x00DCDA5F).
// Method set: Ego_r.pdb / FableWin.pdb ?..@CQ_OrchardFarmRaidScript@NScript@@ (declarations only). ERaidComment is the nested enum
// of MakeTeamMemberComment's third parameter (`W4ERaidComment@12@`); its enumerator names are in neither name set.
// CCrateTeamManager (Ego_r.pdb, 64 bytes, the same in retail: no container): StateCounter[6] +0, MemberCount +0x18,
// TeamReinforcementsTimer +0x1c, MaxTeamMembers +0x20, CrateDropPos +0x24, TeamCrateCarrier +0x30, EnemyTeam +0x3c.
// CTeamSpawn (Ego_r.pdb, 64 bytes, opt-in FABLE_RETAIL_CQ_ORCHARDFARMRAIDSCRIPT_CTEAMSPAWN_MAIN): CEntityScriptBase (0x14), ParentClass
// +0x14 (a pointer in the PDB layout; Main reads `mov eax,[esi+14h] ; mov cl,[eax+50h]`), PMasterData +0x18, OtherSpawnPoint +0x1c,
// TeamID +0x28, TeamMemberLimit +0x2c, TeamRespawnTime +0x30, TeamMemberName +0x34, TeamMemberDefName +0x38, BanditsLeftID +0x3c (the
// landed ctor 0x00DCD160 / Init 0x00DCD1D0 agree). Spellings: Ego_r.pdb / FableWin.pdb ?<Name>@CTeamSpawn@CQ_OrchardFarmRaidScript@NScript@@
// (Alloc SI 0x00DCD0E0, ctor QAE 0x00DCD160, Init / Main EAE (private virtual), GetParentScript UBE).
#ifndef FABLE_RETAIL_NSCRIPT
#error engine/CQ_OrchardFarmRaidScript.h is the retail NScript model: define FABLE_RETAIL_NSCRIPT before including it
#else
#include "rebuild_abi.h"
#include "fable_string.h"
#include "engine/CCountedPointer.h"
#include "engine/CScriptThing.h"
#include "engine/CLevelScriptBase.h"
#include "engine/CEntityScriptBase.h"
#include "engine/CTimer.h"
#include "stlport/fable_containers.h"
class CPersistContext;
class CScriptDataBase;
namespace NScript {
class CQ_SunnyvaleMasterData;
class CQ_OrchardFarmRaidScript : public CLevelScriptBase<CQ_SunnyvaleMasterData> {
public:
    enum ERaidComment { };                                               // enumerators unproven (W4ERaidComment@12@)
    class CCrateTeamManager {
    public:
        CCrateTeamManager();
        ~CCrateTeamManager();
        long StateCounter[6];                                            // +0x00
        long MemberCount;                                                // +0x18
        CTimer TeamReinforcementsTimer;                                  // +0x1c
        long MaxTeamMembers;                                             // +0x20
        CScriptThing CrateDropPos;                                       // +0x24
        CScriptThing TeamCrateCarrier;                                   // +0x30
        CCrateTeamManager* EnemyTeam;                                    // +0x3c
    };
    static CScriptBase* FABLE_FASTCALL Alloc(CScriptDataBase*, CGameScriptInterfaceBase&);
    virtual void RegisterMain();
    virtual void Main();                                                 // 0x00DCC770
    virtual void Init();                                                 // 0x00DCC140
    virtual void OnPersist(CPersistContext&);                            // 0x00DCC720
    void WatchForExternalScriptDeactivation();                           // 0x00DCCF30
    void ReplaceQuestCards();
    bool MakeTeamMemberComment(const CCharString&, CScriptThing&, ERaidComment);   // 0x00DCDA80
    void DoMultiplierCutscene();
    void DoCutsceneIfRequired();                                         // 0x00DCFA60
    void ProcessGameRulesEvil();                                         // 0x00DD03D0
    void ProcessGameRulesGood();                                         // 0x00DD0F60
    long CrateCount;                                                     // +0x48
    CTimer CommentTimer;                                                 // +0x4c
    bool DoneIntroduction;                                               // +0x50
    bool HeroAtWrongEntrance;                                            // +0x51
    bool WhisperSpawned;                                                 // +0x52
    std::vector<CScriptThing,std::allocator<CScriptThing> > CrateList;  // +0x54
    bool MissionSucceeded;                                               // +0x60
    long MissionFailed;                                                  // +0x64 (PDB: <unnamed-type-MissionFailed>, 4 bytes)
    CWideString FailReasons[4];                                          // +0x68
    long HeroTeam;                                                       // +0x78
    long BanditWavesSpawned;                                             // +0x7c
    CTimer RemindHeroOfObjectivesTimer;                                  // +0x80
    CCharString TextSystemScriptCode;                                    // +0x84
    bool HeroMetWhisperBeforeFarm;                                       // +0x88
    bool ShownCombatMultiplierTutorial;                                  // +0x89
    bool ShownCombatFlourishTutorial;                                    // +0x8a
    bool WhisperInCutscene;                                              // +0x8b
    CCrateTeamManager Teams[2];                                          // +0x8c
    class CTeamSpawn;                                                    // Alloc 0x00DCD0E0
    class CArtefact;                                                     // Alloc 0x00DCDB80
    class COrchardFarmWhisper;                                           // Alloc 0x00DCEF50
    class CCrateTeamMember;                                              // Alloc 0x00DCDE20
    class CFarmRearEntrance;                                             // Alloc 0x00DCF3D0
    class CM_WhisperFarmRaidIntro;                                       // Alloc 0x00DCF540
    class CMK_OFI_GWLL_WHIS2;                                            // Alloc 0x00DCF610
#ifdef FABLE_RETAIL_CQ_ORCHARDFARMRAIDSCRIPT_CTEAMSPAWN_MAIN
    class CTeamSpawn : public CEntityScriptBase {
    public:
        static CCountedPointer<CEntityScriptBase> FABLE_FASTCALL Alloc(CQ_OrchardFarmRaidScript&, CScriptDataBase*, const CScriptThing&); // 0x00DCD0E0
        CTeamSpawn(CQ_OrchardFarmRaidScript&, CScriptDataBase*, const CScriptThing&);   // 0x00DCD160
        virtual CScriptBase* GetParentScript() const;
    private:
        virtual void Main();                                             // 0x00DCD350
        virtual void Init();                                             // 0x00DCD1D0
    protected:
        CQ_OrchardFarmRaidScript* ParentClass;                           // +0x14
        CQ_SunnyvaleMasterData* PMasterData;                             // +0x18
    private:
        CScriptThing OtherSpawnPoint;                                    // +0x1c
        long TeamID;                                                     // +0x28
        long TeamMemberLimit;                                            // +0x2c
        long TeamRespawnTime;                                            // +0x30
        CCharString TeamMemberName;                                      // +0x34
        CCharString TeamMemberDefName;                                   // +0x38
        long BanditsLeftID;                                              // +0x3c
    };
#endif
};
}
FABLE_STATIC_ASSERT(sizeof(NScript::CQ_OrchardFarmRaidScript) == 0x10c);
#ifdef FABLE_RETAIL_CQ_ORCHARDFARMRAIDSCRIPT_CTEAMSPAWN_MAIN
FABLE_STATIC_ASSERT(sizeof(NScript::CQ_OrchardFarmRaidScript::CTeamSpawn) == 0x40);
#endif

#endif // FABLE_RETAIL_NSCRIPT
#endif // FABLE_ENGINE_CQ_ORCHARDFARMRAIDSCRIPT_H
