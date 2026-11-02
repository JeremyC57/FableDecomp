// NScript::CV_FisticuffsClubScript::SetCompletedThisClub() -- copy the "completed" flag of the club in the current region into CompletedThisPunchClub (cleared first; set only while the active thread is not terminating)
// Leaf: ?SetCompletedThisClub@CV_FisticuffsClubScript@NScript@@QAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00E827A0, 96 bytes including its 4-entry jump table (0x00E827F0..0x00E827FF; the next
//   function follows), thiscall. Dense switch on RelevantRegion - 1 (`cmp eax,3 ; ja` default), the CompletedThisPunchClub
//   clear scheduled between the bound check and the indirect jump.
// Identity (proven, two sources): (1) FableWin.pdb ?SetCompletedThisClub@CV_FisticuffsClubScript@NScript@@QAEXXZ, the
//   neighbour of SetFightCount; its only caller is CV_FisticuffsClubScript::InitialiseVariables 0x00E82650 (which the landed
//   source declares as the stand-in "InitialiseVariablesCheck" -- NAME_FIXES); (2) every offset touched is an Ego_r.pdb member
//   of CV_FisticuffsClubScript less 0x14 (the retail CScriptBase): RelevantRegion +0x48, CompletedThisPunchClub +0x94,
//   CompletedKnotholePunchClub +0x95, CompletedBowerstonePunchClub +0x96, CompletedBanditPunchClub +0x97,
//   CompletedOakvalePunchClub +0x98; the jump table [+0x96, +0x98, +0x97, +0x95] pairs region 1..4 with Bowerstone, Oakvale,
//   Bandit Camp, Knothole -- the RegionNames order. Catalog label "CScriptBase_InitialiseVariables_Check" is a stand-in.
// Callee: NScript::CScriptBase::IsActiveThreadTerminating() 0x00CB7940 (engine/CScriptBase.h).
// Layout: engine/CLevelScriptBase.h retail branch (FABLE_RETAIL_NSCRIPT); every Ego_r.pdb member restated at its retail offset,
//   class-typed members (CTimer, CScriptThing, CCharString[5]) as opaque 4-byte-aligned storage; sizeof 0xC8 asserted.
// Shape: a switch whose case order follows the source (1, 4, 2, 3) with `default: return`; the terminate test only on the
//   completed path. The jump table entries were verified by objdump -r against retail (vland checks them too).
#define FABLE_RETAIL_NSCRIPT
#include <stddef.h>
#include "engine/CLevelScriptBase.h"                     // CLevelScriptBase<T> : CScriptBase (0x40): Game +0x40, MasterScriptData +0x44

namespace NScript {
class CQ_SunnyvaleMasterData;
class CV_FisticuffsClubScript : public CLevelScriptBase<CQ_SunnyvaleMasterData> {   // vtable 0x012EAF14; retail = Ego_r.pdb - 0x14
public:
    void SetCompletedThisClub();
    long          RelevantRegion;                        // +0x48 (1 Bowerstone, 2 Oakvale, 3 Bandit Camp, 4 Knothole Glade)
    bool          FighterOneInRing;                      // +0x4c
    bool          FighterTwoInRing;                      // +0x4d
    bool          ChallengeAccepted;                     // +0x4e
    bool          FightFinished;                         // +0x4f
    bool          HeroInsideRing;                        // +0x50
    bool          HeroOfferedFisticuffsClubEntry;        // +0x51
    bool          HeroPaidEntryFee;                      // +0x52
    bool          HeroInFisticuffsClub;                  // +0x53
    bool          TylerInFight;                          // +0x54
    bool          HeroBeatTyler;                         // +0x55
    bool          TimeOut;                               // +0x56
    bool          RingCleared;                           // +0x57
    bool          RoundStarted;                          // +0x58
    unsigned long Fighter1Timer_[1];                     // +0x5c CTimer Fighter1Timer
    unsigned long Fighter2Timer_[1];                     // +0x60 CTimer Fighter2Timer
    unsigned long SFighterOne_[3];                       // +0x64 CScriptThing SFighterOne
    unsigned long SFighterTwo_[3];                       // +0x70 CScriptThing SFighterTwo
    long          HitID;                                 // +0x7c
    long          HitCount;                              // +0x80
    long          AttackWarnings;                        // +0x84
    long          FightCount;                            // +0x88
    bool          EverybodyAttack;                       // +0x8c
    bool          LostWhileOutside;                      // +0x8d
    bool          FisticuffsActive;                      // +0x8e
    long          AllowedHits;                           // +0x90
    bool          CompletedThisPunchClub;                // +0x94
    bool          CompletedKnotholePunchClub;            // +0x95
    bool          CompletedBowerstonePunchClub;          // +0x96
    bool          CompletedBanditPunchClub;              // +0x97
    bool          CompletedOakvalePunchClub;             // +0x98
    unsigned long Village_[3];                           // +0x9c CScriptThing Village
    bool          PlayerLost;                            // +0xa8
    float         CheerNumber;                           // +0xac
    float         BooNumber;                             // +0xb0
    unsigned long RegionNames_[5];                       // +0xb4 CCharString RegionNames[5]
};
FABLE_STATIC_ASSERT(sizeof(CV_FisticuffsClubScript) == 0xc8);
FABLE_STATIC_ASSERT(offsetof(CV_FisticuffsClubScript, RelevantRegion) == 0x48);
FABLE_STATIC_ASSERT(offsetof(CV_FisticuffsClubScript, Fighter1Timer_) == 0x5c);
FABLE_STATIC_ASSERT(offsetof(CV_FisticuffsClubScript, CompletedThisPunchClub) == 0x94);
FABLE_STATIC_ASSERT(offsetof(CV_FisticuffsClubScript, Village_) == 0x9c);
FABLE_STATIC_ASSERT(offsetof(CV_FisticuffsClubScript, RegionNames_) == 0xb4);

void CV_FisticuffsClubScript::SetCompletedThisClub()
{
    CompletedThisPunchClub = false;
    bool completed;
    switch (RelevantRegion) {
    case 1:
        completed = CompletedBowerstonePunchClub;
        break;
    case 4:
        completed = CompletedKnotholePunchClub;
        break;
    case 2:
        completed = CompletedOakvalePunchClub;
        break;
    case 3:
        completed = CompletedBanditPunchClub;
        break;
    default:
        return;
    }
    if (completed) {
        if (IsActiveThreadTerminating())
            return;
        CompletedThisPunchClub = true;
    }
}
} // namespace NScript