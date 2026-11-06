#ifndef FABLE_ENGINE_CQ_MINIONCAMPSCRIPT_H
#define FABLE_ENGINE_CQ_MINIONCAMPSCRIPT_H
// Retail opt-in (chunk 809): NScript::CQ_MinionCampScript, new header (the landed sibling rows keep their local models).
// Layout: Ego_r.pdb CQ_MinionCampScript (struct_layouts_egor.tsv, 208 bytes) at retail offsets = PDB - 0x14 (retail CLevelScriptBase
// is 0x48) up to CurrentStage +0x74; the two STLport lists are 4 bytes each in retail (the PDB's 12), so ShouldSpawn +0x98,
// SpawnPointToSpawn +0x9c, BriarState +0xa8, sizeof 0xAC -- the offsets the landed siblings use (AddNewHeroAttacker 0x00DA0B30,
// GetRandomSpawnPoint 0x00DA0E40: HeroAttackerOne / Two +0x78 / +0x84, LeftSpawnPoints / RightSpawnPoints +0x90 / +0x94) and
// CBriarRose::Main 0x00DA0EF0 reads (IntroDone +0x49, BriarRoseAtAltar +0x4a, MinionsSummoned +0x4b, PortalSealed +0x4c,
// MissionFailed +0x4d, ChantTimer +0x60, ChantTimerBar +0x64, ChantTimerPaused +0x68, CurrentStage +0x74, BriarState +0xa8).
// The lists are opaque here (no std::list model in rebuild/include; header debt): one pointer word each.
// MissionStage / BriarRoseState: the PDB names the member types but no enumerators (as the landed siblings declare them).
// Nested CBriarRose (Ego_r.pdb, 80 bytes): CEntityScriptBase (0x14), ParentClass +0x14, PMasterData +0x18, seh_me +0x1c,
// MyDestination +0x2c, Target +0x38, NumHitsBetweenForcePushes +0x44, SpeechTimer +0x48, IsPlayingChantAnim +0x4c. Entity vtable
// 0x012D6D0C (stored by the ctor 0x00D9E930 that Alloc 0x00D9E8B0 calls): 0 scalar deleting dtor 0x00D9FEB0, 1 Main 0x00DA0EF0,
// 2 Init 0x00D9E9E0, 3 GetParentScript 0x00D9E9C0, 4 0x00CDEBC0, 5 OnPredicateFail 0x00D9E9D0. Spellings: Ego_r.pdb / FableWin.pdb
// ?<Name>@CBriarRose@CQ_MinionCampScript@NScript@@ (Main / Init / OnPredicateFail EAE, the helpers AAE, GetParentScript UBE).
// DoChant / DoOnHeroHitSpeech: in both PDB name sets, no out-of-line copy in retail (Main expands them).
#ifdef FABLE_RETAIL_NSCRIPT
#include "rebuild_abi.h"
#include "fable_string.h"
#include "engine/CCountedPointer.h"
#include "engine/CScriptThing.h"
#include "engine/CLevelScriptBase.h"
#include "engine/CEntityScriptBase.h"
#include "engine/CScriptGameResourceObjectScriptedThingBase.h"
#include "engine/CTimer.h"
namespace NScript {
class CQ_SunnyvaleMasterData;
class CQ_MinionCampScript : public CLevelScriptBase<CQ_SunnyvaleMasterData> {
public:
    enum MissionStage { };
    enum BriarRoseState { };
    virtual void RegisterMain();                                         // 0x00D9E520
    virtual void Main();                                                 // 0x00D9E5E0
    virtual void Init();                                                 // 0x00D9E2E0
    void DoMission();                                                    // 0x00DA0230
    void PhaseOne();                                                     // 0x00DA05D0
    void RunFirstWaveControl();                                          // 0x00DA0820
    void RunSpawnPointControl();                                         // 0x00DA0C40
    void AddNewHeroAttacker(const CScriptThing&);                        // 0x00DA0B30
    bool IsAssignedAsHeroAttacker(const CScriptThing&);                  // 0x00D9EC50
    void StartMinionSpawnTimer();                                        // 0x00D9F200
    void PauseChantTimer();                                              // 0x00DA2490
    void RunChantTimer();                                                // 0x00DA24F0
    void CheckBriarHit(CScriptThing);                                    // 0x00DA2860
    bool QuestStartScreenShown;                                          // +0x48
    bool IntroDone;                                                      // +0x49
    bool BriarRoseAtAltar;                                               // +0x4a
    bool MinionsSummoned;                                                // +0x4b
    bool PortalSealed;                                                   // +0x4c
    bool MissionFailed;                                                  // +0x4d
    long NumberOfMinions;                                                // +0x50
    long MaxNumberOfMinions;                                             // +0x54
    float MinMinionSpawnPeriod;                                          // +0x58
    float MaxMinionSpawnPeriod;                                          // +0x5c
    CTimer ChantTimer;                                                   // +0x60
    long ChantTimerBar;                                                  // +0x64
    bool ChantTimerPaused;                                               // +0x68
    long PausedTime;                                                     // +0x6c
    CTimer MinionSpawnTimer;                                             // +0x70
    MissionStage CurrentStage;                                           // +0x74
    CScriptThing HeroAttackerOne;                                        // +0x78
    CScriptThing HeroAttackerTwo;                                        // +0x84
    void* LeftSpawnPoints;                                               // +0x90 std::list<CScriptThing> (opaque)
    void* RightSpawnPoints;                                              // +0x94 std::list<CScriptThing> (opaque)
    bool ShouldSpawn;                                                    // +0x98
    CScriptThing SpawnPointToSpawn;                                      // +0x9c
    BriarRoseState BriarState;                                           // +0xa8
    class CBriarRose : public CEntityScriptBase {
    public:
        static CCountedPointer<CEntityScriptBase> FABLE_FASTCALL Alloc(CQ_MinionCampScript&, CScriptDataBase*, const CScriptThing&); // 0x00D9E8B0
        virtual CScriptBase* GetParentScript() const;                    // 0x00D9E9C0
    private:
        virtual void Main();                                             // 0x00DA0EF0
        virtual void Init();                                             // 0x00D9E9E0
        virtual void OnPredicateFail();                                  // 0x00D9E9D0
        void DoChant();                                                  // (expanded by Main)
        void DoOnHeroHitSpeech();                                        // (expanded by Main)
        void DoOnHitSpeech();                                            // 0x00DA26B0
        void DoOnSpeakToSpeech();                                        // 0x00DA27B0
        void DoSpeech(const CCharString&);                               // 0x00DA2610
        void PlayCutscene(CCharString);                                  // 0x00DA2CA0
        bool UpdateTarget();                                             // 0x00DA2A20
    protected:
        CQ_MinionCampScript& ParentClass;                                // +0x14
        CQ_SunnyvaleMasterData* PMasterData;                             // +0x18
        CScriptGameResourceObjectScriptedThingBase seh_me;               // +0x1c
        CScriptThing MyDestination;                                      // +0x2c
        CScriptThing Target;                                             // +0x38
        long NumHitsBetweenForcePushes;                                  // +0x44
        CTimer SpeechTimer;                                              // +0x48
        bool IsPlayingChantAnim;                                         // +0x4c
    };
    class CMCMinionGuard;                                                // Alloc 0x00D9EA80
    class CMCSpawnPoint;                                                 // Alloc 0x00D9ED50
    class CMCMinion;                                                     // Alloc 0x00D9F0B0
    class CFadeinSummoner;                                               // Alloc 0x00D9F5B0
};
}
FABLE_STATIC_ASSERT(sizeof(NScript::CQ_MinionCampScript) == 0xAC);
FABLE_STATIC_ASSERT(sizeof(NScript::CQ_MinionCampScript::CBriarRose) == 0x50);

#endif // FABLE_RETAIL_NSCRIPT
#endif // FABLE_ENGINE_CQ_MINIONCAMPSCRIPT_H
