// NScript::CV_ArcheryCompetitionScript::OnPersist(CPersistContext& context) -- save/load the archery contest's flags: quest activated/finished, prize and welcome state, the two high-score prizes (given and added to the owner), the attack warning and the pickpocket flag, each under its member name with a false default
// Leaf: ?OnPersist@CV_ArcheryCompetitionScript@NScript@@UAEXAAVCPersistContext@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00E37520, 269 bytes, thiscall, `ret 4`.
// Identity (proven): the row is referenced once, from slot 4 (OnPersist) of the RTTI-less level-script vtable 0x012E2EE4,
//   which the landed NScript::CV_ArcheryCompetitionScript ctor 0x00E32350 stores (slot 1 the landed RegisterMain
//   0x00E324A0, slot 3 the landed Init 0x00E323B0); Ego_r.pdb ?OnPersist@CV_ArcheryCompetitionScript@NScript@@UAEXAAVCPersistContext@@@Z.
//   Every persisted name string is the Ego_r.pdb member at that retail offset + 0x14. Catalog label sub_00e37520.
// Callees: CPersistContext::Transfer<bool> 0x004045C0 (Ego_r ??$Transfer@_N@CPersistContext@@QAEXPBDAA_NAB_N@Z), ten times.
// Strings (paired by hand): "QuestActivated" 0x012E2778, "PrizeAlreadyGot" 0x012E35A0, "QuestFinished" 0x012C6FBC,
//   "WelcomeComplete" 0x012E3590, "GivenReallyCoolPrize" 0x012E3578, "GivenFirstHighScorePrize" 0x012E355C,
//   "GiveAttackWarning" 0x012E3548, "AddedReallyCoolPrizeToOwner" 0x012E352C, "AddedFirstHighScorePrizeToOwner"
//   0x012E350C, "Pickpocketed" 0x012E34FC.
// Layout: the landed CV_ArcheryCompetitionScript::Init 0x00E323B0 model (Ego_r.pdb - 0x14).
// Shape: the landed CV_ChapelOfEvilScript::OnPersist 0x00E690F0 model: one Transfer per member in retail's order; each
//   false default is a temporary in the dead argument slot (bl = 0).
class CCharString { public: void* PData; };
class CGameScriptInterfaceBase;
class CPersistContext {
public:
    template <class T> void Transfer(const char* name, T& value, const T& defaultValue);   // 0x004045C0 <bool>
};
#define FABLE_RETAIL_NSCRIPT
// CTimer: the retail model from engine/CTimer.h (Ego_r.pdb CTimer {long TimerIndex}, 0x4; the setter is the retail
//   operator=(long), Ego_r 0x00CCE720). Bodies the row expands inline follow the include.
#include "engine/CTimer.h"
// CScriptBase / CLevelScriptBase<T>: the retail NScript model from the shared headers (FABLE_RETAIL_NSCRIPT: CScriptBase :
//   CBaseClassNonCopyable : CBaseClass, vtable 0x012C1648, 5 slots, 0x40; CLevelScriptBase<T> Game +0x40, MasterScriptData +0x44).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CLevelScriptBase.h"

namespace NScript {
class CQ_SunnyvaleMasterData;
class CV_ArcheryCompetitionScript : public CLevelScriptBase<CQ_SunnyvaleMasterData> {
public:
    long Score;                                         // +0x48
    bool IsOwnerDead;                                   // +0x4c
    bool PrizeAlreadyGot;                               // +0x4d
    bool QuestActivated;                                // +0x4e
    bool AttackedByHero;                                // +0x4f
    bool GiveAttackWarning;                             // +0x50
    bool QuestFinished;                                 // +0x51
    bool WelcomeComplete;                               // +0x52
    bool Pickpocketed;                                  // +0x53
    long DummyCounter;                                  // +0x54
    long ArcheryHighScoreInfoID;                        // +0x58
    long ArcheryGameScoreInfoID;                        // +0x5c
    long ArcheryGameTimerID;                            // +0x60
    bool SteppedOut;                                    // +0x64
    bool UsedMagic;                                     // +0x65
    bool ShotOwner;                                     // +0x66
    bool GivenReallyCoolPrize;                          // +0x67
    bool GivenFirstHighScorePrize;                      // +0x68
    bool AddedReallyCoolPrizeToOwner;                   // +0x69
    bool AddedFirstHighScorePrizeToOwner;               // +0x6a
    CTimer GameTimeLeft;                                // +0x6c
    CTimer RandomSpeechTimer;                           // +0x70
    CCharString RandomSpeech[10];                       // +0x74
    enum { SCORE_POWER_LEVEL_0 } ScorePowerLevel;       // +0x9c (unnamed enum in the PDB)
    enum { CONTEST_STATE_0 } ContestState;              // +0xa0 (unnamed enum in the PDB)
    virtual void OnPersist(CPersistContext& context);
};

void CV_ArcheryCompetitionScript::OnPersist(CPersistContext& context)
{
    context.Transfer("QuestActivated", QuestActivated, false);
    context.Transfer("PrizeAlreadyGot", PrizeAlreadyGot, false);
    context.Transfer("QuestFinished", QuestFinished, false);
    context.Transfer("WelcomeComplete", WelcomeComplete, false);
    context.Transfer("GivenReallyCoolPrize", GivenReallyCoolPrize, false);
    context.Transfer("GivenFirstHighScorePrize", GivenFirstHighScorePrize, false);
    context.Transfer("GiveAttackWarning", GiveAttackWarning, false);
    context.Transfer("AddedReallyCoolPrizeToOwner", AddedReallyCoolPrizeToOwner, false);
    context.Transfer("AddedFirstHighScorePrizeToOwner", AddedFirstHighScorePrizeToOwner, false);
    context.Transfer("Pickpocketed", Pickpocketed, false);
}
}