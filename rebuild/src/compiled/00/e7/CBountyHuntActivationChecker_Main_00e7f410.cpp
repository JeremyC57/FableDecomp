// CBountyHuntActivationChecker::Main() -- fishing-competition state check: while Q_BountyHunt is active, flag it, remove the fishermen and post the "bandits" sign text; when the hunt has just ended, respawn the fishermen and refresh the competition; otherwise refresh the sign / respawn the organiser / generate an entry depending on the competition state; finally catch the attack counter up with the elapsed days
// Leaf: Main
// Retail: Fable.exe (TLC, VC7.1) @ 0x00E7F410, 786 bytes (800 with padding); entity-script vtable slot 1 (Main).
// Callees: CEntityScriptBase::IsActiveThreadTerminating 0x00F35B30, CCharString(const char*, long) 0x0099EBF0 /
//   ~CCharString 0x0099EAE0, ~CScriptThing 0x004AA840, RemoveFishermen 0x00E7FB80 / RespawnFishermen 0x00E7F7F0 (this),
//   CV_FishingCompetitionScript GenerateCompetitionEntry 0x00E7F730 and its sign/competition refresh 0x00E7FD50
//   (reads "FishCompSign"; manifest label "FishingCompetition_Initialize"); CScriptThing slot 6 GetPos;
//   CGameScriptInterface slots 72 GetThingWithScriptName, 91 CreateCreature, 284 IsQuestActive, 652 GetDayCount,
//   714 SetReadableObjectTextTag.
//   Literals "FishCompSign" 0x012EA6D0, "Q_BountyHunt" 0x012C4D4C, "TEXT_QST_B15_FISH_SIGN_INFO_BANDITS" 0x012EA6AC,
//   "TEXT_QST_B15_FISH_SIGN_INFO_RANAWAY" 0x012EA688, "FishCompOrganiser" 0x012EA554, "MK_FC_ORG" 0x012EA67C,
//   "CREATURE_FISHERMAN" 0x012EA668.
// Layout: Ego_r.pdb CBountyHuntActivationChecker (28 bytes): CEntityScriptBase {vptr, Game +4, Me +8}, ParentClass +0x14
//   (CV_FishingCompetitionScript, retail = PDB - 0x14), PMasterData +0x18.
class C3DVector { public: float X, Y, Z; };
class CCharString { public:
    void* PStringData;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    ~CCharString();                                    // 0x0099EAE0
};
class CScriptThing { public:                                  // vtable 0x01238C8C, 12 bytes
    virtual void t000(); virtual void t001(); virtual void t002(); virtual void t003(); virtual void t004(); virtual void t005();
    virtual const C3DVector& GetPos() const;   // slot 6 (+0x18)
    void* PImp_Data;                                   // +0x04
    void* PImp_Info;                                   // +0x08
    CScriptThing(const CScriptThing& other);           // 0x004ABE90
    virtual ~CScriptThing();                                   // 0x004AA840
};
enum EScriptEntityMoveType { SCRIPT_ENTITY_MOVE_TYPE_WALK };
enum EScriptAIPriority { SCRIPT_AI_PRIORITY_NONE };
enum ETextGroupSelectionMethod { TEXT_GROUP_SELECTION_RANDOM };
#define FABLE_RETAIL_BASE_OBJECT
#include "engine/CBaseObject.h"                       // : CBaseClass, vtable 0x0129A7D8 (~ slot 0, Validate slot 1)
template <class T> class CCountedPointer { public:
    void* Data;
    void* Info;
    CCountedPointer() : Data(0), Info(0) { }
};
#include "engine/CScriptGameResourceObjectBase.h"
#include "engine/CScriptGameResourceObjectScriptedThingBase.h"
// CGameScriptInterface is the retail game-script interface from the shared headers (FABLE_RETAIL_NSCRIPT opt-in:
//   CGameScriptInterface : CGameScriptInterfaceBase : CBaseClassNonCopyable : CBaseClass, vtable 0x01260F0C, 918 slots).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterface.h"
class CEntityScriptBase { public:                             // retail 0x14 bytes
    virtual void e00();
    CGameScriptInterface* Game;                        // +0x04
    CScriptThing          Me;                          // +0x08
    void SetPredicate(void* predicate);       // 0x00F35B10
    bool IsActiveThreadTerminating() const;            // 0x00F35B30
};
struct CV_FishingCompetitionScript {
    unsigned char _pad[0x48];
    bool BountyHuntActive;                             // +0x48
    bool OrganiserRanAway;                             // +0x49
    bool OrganiserKilled;                              // +0x4a
    bool CompetitionRunning;                           // +0x4b
    bool CompetitionWon;                               // +0x4c
    bool RodOfChampsGiven;                             // +0x4d
    bool CommentIRanAway;                              // +0x4e
    bool CompetitorsAttacked;                          // +0x4f
    bool CompetitorsJustGotAttacked;                   // +0x50
    long AttackCounter;                                // +0x54
    long AttackDayCount;                               // +0x58
    long HitCount;                                     // +0x5c
    void GenerateCompetitionEntry();                   // 0x00E7F730
    void RefreshCompetition();                         // 0x00E7FD50
};
struct CBountyHuntActivationChecker : CEntityScriptBase {
    CV_FishingCompetitionScript* ParentClass;          // +0x14
    void*                        PMasterData;          // +0x18 CQ_SunnyvaleMasterData*
    void Main();
    void RespawnFishermen();                           // 0x00E7F7F0
    void RemoveFishermen();                            // 0x00E7FB80
};

void CBountyHuntActivationChecker::Main()
{
    CScriptThing sign = Game->GetThingWithScriptName("FishCompSign");
    if (Game->IsQuestActive("Q_BountyHunt")) {
        if (IsActiveThreadTerminating())
            return;
        ParentClass->BountyHuntActive = true;
        RemoveFishermen();
        if (!ParentClass->CompetitionWon) {
            if (IsActiveThreadTerminating())
                return;
            Game->SetReadableObjectTextTag(sign, "TEXT_QST_B15_FISH_SIGN_INFO_BANDITS");
        }
    } else {
        bool huntEnded = !Game->IsQuestActive("Q_BountyHunt") && ParentClass->BountyHuntActive;
        if (huntEnded) {
            if (IsActiveThreadTerminating())
                return;
            ParentClass->BountyHuntActive = false;
            RespawnFishermen();
            if (!ParentClass->CompetitionWon) {
                if (IsActiveThreadTerminating())
                    return;
                ParentClass->RefreshCompetition();
            }
        } else if (ParentClass->CompetitionWon) {
            if (IsActiveThreadTerminating())
                return;
            ParentClass->RefreshCompetition();
            return;
        } else if (!ParentClass->CompetitionRunning) {
            if (IsActiveThreadTerminating())
                return;
            Game->SetReadableObjectTextTag(sign, "TEXT_QST_B15_FISH_SIGN_INFO_RANAWAY");
        } else if (ParentClass->OrganiserRanAway) {
            if (IsActiveThreadTerminating())
                return;
            Game->CreateCreature("CREATURE_FISHERMAN", Game->GetThingWithScriptName("MK_FC_ORG").GetPos(), "FishCompOrganiser", false);
            ParentClass->RefreshCompetition();
            ParentClass->OrganiserRanAway = false;
            ParentClass->CommentIRanAway = true;
        } else {
            if (IsActiveThreadTerminating())
                return;
            ParentClass->GenerateCompetitionEntry();
            ParentClass->RefreshCompetition();
        }
    }
    if (ParentClass->AttackCounter > 0) {
        if (IsActiveThreadTerminating())
            return;
        long day = Game->GetDayCount();
        while (ParentClass->AttackCounter > 0 && day > ParentClass->AttackDayCount) {
            ParentClass->AttackDayCount++;
            ParentClass->AttackCounter--;
            ParentClass->HitCount--;
        }
    }
}