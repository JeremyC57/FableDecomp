// NScript::CV_FishingCompetitionScript::CFishCompOrganiser::AskForFish() -- the organiser checks the hero's catch: question cut-scene (nasty / bonus variants); no fish -> the no-fish line plus the rod state (no-new-prize, champion rod, rod hint once, reminder); otherwise the biggest carried fish's weight, the entry yes/no question unless the competition is already won, the prize band from the script def's weights, then bigger (online + local score, bonus / personal-best / need-bigger / no-prize / prize cut-scenes paying out each new prize, the upgraded rod and the quest objective on the last), same-size, comment-bonus or smaller lines; a new overall best makes the hero the winner
// Leaf: ?AskForFish@CFishCompOrganiser@CV_FishingCompetitionScript@NScript@@AAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00E80F10, 2894 bytes, private thiscall, plain `ret`, no EH frame.
// Identity (proven): Ego_r.pdb ?AskForFish@CFishCompOrganiser@CV_FishingCompetitionScript@NScript@@AAEXXZ; the body calls the class's own
//   PlayCutscene 0x00E80D60, CheckForFish 0x00E81AD0, CheckForRod 0x00E81A60 and GetBiggestFishWeight 0x00E81B40 and reads the organiser's
//   csargs map (+0x3c) and FishingPrizes (+0x58).
// Callees: IsActiveThreadTerminating 0x00F35B30 (before every script statement); CCharString(const char*, long) 0x0099EBF0 / ~ 0x0099EAE0 /
//   copy 0x0099EC30 / operator= 0x0099EFB0 / operator=(const char*) 0x0099EFE0; operator+(CCharString, CCharString) 0x0099F570 and
//   operator+(const char*, CCharString) 0x0099F690; GFIntToCharString 0x0099F830; map<CCharString, CCharString>::operator[] 0x009AC700;
//   CBasicString<char>::Compare 0x00411570 (the inline `prizeName == "OBJECT_FISHING_ROD_UPGRADED"`, null arm as `repe cmpsb`);
//   CV_FishingCompetitionScript::RefreshCompetition 0x00E7FD50; interface slots 7 NewScriptFrame, 39 MsgIsQuestionAnsweredYesOrNo,
//   114 GiveHeroYesNoQuestion, 296 SetQuestCardObjective, 655 GetActiveQuestName, 728 UpdateOnlineScore_FishingCompo,
//   729 UpdateScore_FishingCompetition (ghidra_out/cgsi_slots_aligned.tsv); pGlobalScriptDef 0x0143E90C (FishPrizeDefNames +0x5ac,
//   FishPrizeWeights +0x5d0, engine/CScriptDef.h).
// Layout: CFishCompOrganiser = Ego_r.pdb with retail 12-byte containers (the landed SpeakConversationLine 0x00E80CF0 / CheckForFish
//   0x00E81AD0 model); CV_FishingCompetitionScript fields are Ego_r - 0x14 (CompetitionWon +0x4c, RodOfChampsGiven +0x4d, AttackCounter
//   +0x54, RodHintGiven +0x61, PrizeNumber +0x64, Winner +0x68, BiggestFish +0x6c, BiggestCompetitorFish +0x70).
// Shape: the entry answer defaults to 1 and the won arm only checks termination, so VC7.1 threads the won path straight into the
//   accepted block and keeps the refusal (the `else` of `answer == 1`) as the last block, where the no-fish exit cross-jumps into its
//   GENERIC cut-scene tail; the CheckForRod test is written negated (retail falls through into the no-rod arm); the same-size test
//   is `weight == BiggestFish` (BiggestFish loaded first for fucompp); the prize band loop is VC's four-way unroll of
//   `for (i = FishingPrizes; i >= 0 && prize == 0; --i)`.
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_CCHARSTRING_CONCAT_STRING
#define FABLE_RETAIL_CBASICSTRING_TEXT_EQ_INLINE
#define FABLE_RETAIL_CCHARSTRING_FORCEINLINE_TEXT_EQ
#include "fable_string.h"
CCharString __fastcall GFIntToCharString(long value);       // 0x0099F830
namespace NScript { class CPredicateBase; }
#define FABLE_RETAIL_NSCRIPT
#include "engine/CCountedPointer.h"
#include "engine/CScriptThing.h"
#include "engine/CGameScriptInterface.h"
#include "engine/CEntityScriptBase.h"
#include "stlport/fable_map.h"
#include "engine/CScriptDef.h"
extern CScriptDef* pGlobalScriptDef;                         // 0x0143E90C
namespace NScript {
class CV_FishingCompetitionScript {
public:
    unsigned char _pad_0x00[0x48];
    bool BountyHuntActive;                                   // +0x48
    bool OrganiserRanAway;                                   // +0x49
    bool OrganiserKilled;                                    // +0x4a
    bool CompetitionRunning;                                 // +0x4b
    bool CompetitionWon;                                     // +0x4c
    bool RodOfChampsGiven;                                   // +0x4d
    bool CommentIRanAway;                                    // +0x4e
    bool CompetitorsAttacked;                                // +0x4f
    bool CompetitorsJustGotAttacked;                         // +0x50
    long AttackCounter;                                      // +0x54
    long AttackDayCount;                                     // +0x58
    long HitCount;                                           // +0x5c
    bool DoneIntro;                                          // +0x60
    bool RodHintGiven;                                       // +0x61
    long PrizeNumber;                                        // +0x64
    CCharString Winner;                                      // +0x68
    float BiggestFish;                                       // +0x6c
    float BiggestCompetitorFish;                             // +0x70
    void RefreshCompetition();                               // 0x00E7FD50
    class CFishCompOrganiser : public CEntityScriptBase {
    public:
        CV_FishingCompetitionScript* ParentClass;            // +0x14
        void* PMasterData;                                   // +0x18
        unsigned char seh_Hero[0x10];                        // +0x1c
        unsigned char seh_Organiser[0x10];                   // +0x2c
        std::map<CCharString, CCharString> csargs;           // +0x3c
        bool CommentHitByHero;                               // +0x48
        long Conversation;                                   // +0x4c
        long FishingRodsInGame;                              // +0x50
        long FishTypesInGame;                                // +0x54
        long FishingPrizes;                                  // +0x58
    private:
        void PlayCutscene(CCharString name);                 // 0x00E80D60
        void AskForFish();
        bool CheckForRod();                                  // 0x00E81A60
        bool CheckForFish();                                 // 0x00E81AD0
        float GetBiggestFishWeight();                        // 0x00E81B40
    };
};
}
using namespace NScript;

void CV_FishingCompetitionScript::CFishCompOrganiser::AskForFish()
{
    CCharString question("TEXT_CS_B15_FISH_QUESTION");
    CCharString suffix("");
    if (ParentClass->CompetitionWon) {
        if (IsActiveThreadTerminating()) return;
        suffix = "_BONUS";
    }
    if (ParentClass->AttackCounter > 0) {
        if (IsActiveThreadTerminating()) return;
        question = "TEXT_CS_B15_FISH_QUESTION_NASTY";
    }
    csargs["$ARG1"] = question + suffix;
    PlayCutscene("CS_FISHCOMP_CHECKFISH_QUESTION");
    if (!CheckForFish()) {
        if (IsActiveThreadTerminating()) return;
        if (ParentClass->AttackCounter > 0) {
            if (IsActiveThreadTerminating()) return;
            csargs["$ARG1"] = "TEXT_CS_B15_FISH_NOFISH_NASTY";
        } else {
            if (IsActiveThreadTerminating()) return;
            csargs["$ARG1"] = "TEXT_CS_B15_FISH_NOFISH";
        }
        if (!CheckForRod()) {
            if (IsActiveThreadTerminating()) return;
            if (ParentClass->RodOfChampsGiven) {
                if (IsActiveThreadTerminating()) return;
                csargs["$ARG2"] = "CAM_FC_HEROORG";
                csargs["$ARG3"] = "TEXT_CS_B15_NOROD_10";
                csargs["$ARG4"] = "TEXT_CS_B15_NOROD_CHAMP_10";
            } else if (!ParentClass->RodHintGiven) {
                if (IsActiveThreadTerminating()) return;
                csargs["$ARG2"] = "CAM_FC_HEROORG";
                csargs["$ARG3"] = "TEXT_CS_B15_NOROD_10";
                csargs["$ARG4"] = "TEXT_CS_B15_NOROD_20";
                ParentClass->RodHintGiven = true;
            } else {
                if (IsActiveThreadTerminating()) return;
                csargs["$ARG2"] = "CAM_FC_ORGHERO";
                csargs["$ARG3"] = "TEXT_CS_B15_NOROD_REMINDER";
            }
        } else {
            if (IsActiveThreadTerminating()) return;
            csargs["$ARG2"] = "CAM_FC_ORGHERO";
            csargs["$ARG3"] = "TEXT_CS_B15_FISH_NONEWPRIZE";
        }
        PlayCutscene("CS_FISHCOMP_CHECKFISH_GENERIC");
        return;
    }
    if (IsActiveThreadTerminating()) return;
    float weight = GetBiggestFishWeight();
    long answer = 1;
    if (ParentClass->CompetitionWon) {
        if (IsActiveThreadTerminating()) return;
    } else {
        if (IsActiveThreadTerminating()) return;
        Game->GiveHeroYesNoQuestion("TEXT_QST_B15_FISH_ENTER_QUESTION", "TEXT_OBJECT_HERO_ANSWER_YES", "TEXT_OBJECT_HERO_ANSWER_NO", "", true);
        while ((answer = Game->MsgIsQuestionAnsweredYesOrNo()) < 0) {
            Game->NewScriptFrame();
            if (IsActiveThreadTerminating()) return;
        }
        if (IsActiveThreadTerminating()) return;
    }
    if (answer == 1) {
        if (IsActiveThreadTerminating()) return;
        long prize = 0;
        for (long i = FishingPrizes; i >= 0 && prize == 0; --i)
            if (weight >= pGlobalScriptDef->FishPrizeWeights[i])
                prize = i;
        if (weight > ParentClass->BiggestFish) {
            if (IsActiveThreadTerminating()) return;
            ParentClass->BiggestFish = weight;
            Game->UpdateOnlineScore_FishingCompo(weight);
            Game->UpdateScore_FishingCompetition(weight);
            if (ParentClass->CompetitionWon) {
                if (IsActiveThreadTerminating()) return;
                csargs["$ARG1"] = "TEXT_CS_B15_FISH_BIGGER_BONUS";
                PlayCutscene("CS_FISHCOMP_CHECKFISH_GENERIC");
            } else if (ParentClass->BiggestCompetitorFish > weight) {
                if (IsActiveThreadTerminating()) return;
                csargs["$ARG1"] = "TEXT_CS_B15_FISH_BIGGEST_PERSONAL";
                PlayCutscene("CS_FISHCOMP_CHECKFISH_GIVETIP");
            } else if (prize == ParentClass->PrizeNumber) {
                if (IsActiveThreadTerminating()) return;
                if (prize == 0) {
                    if (IsActiveThreadTerminating()) return;
                    csargs["$ARG1"] = "TEXT_CS_B15_FISH_NEED_BIGGER";
                } else {
                    if (IsActiveThreadTerminating()) return;
                    csargs["$ARG1"] = "TEXT_CS_B15_FISH_BIGGEST_NO_PRIZE";
                }
                PlayCutscene("CS_FISHCOMP_CHECKFISH_GIVETIP");
            } else {
                if (IsActiveThreadTerminating()) return;
                CCharString cutscene("CS_FISHCOMP_CHECKFISH_PRIZE");
                CCharString prizeName("");
                if (ParentClass->PrizeNumber < 0) {
                    if (IsActiveThreadTerminating()) return;
                    ParentClass->PrizeNumber = 0;
                }
                while (ParentClass->PrizeNumber < prize) {
                    Game->NewScriptFrame();
                    if (IsActiveThreadTerminating()) return;
                    ++ParentClass->PrizeNumber;
                    cutscene = cutscene + GFIntToCharString(ParentClass->PrizeNumber);
                    prizeName = pGlobalScriptDef->FishPrizeDefNames[ParentClass->PrizeNumber];
                    csargs["$PRIZE" + GFIntToCharString(ParentClass->PrizeNumber)] = prizeName;
                    if (prizeName == "OBJECT_FISHING_ROD_UPGRADED") {
                        if (IsActiveThreadTerminating()) return;
                        ParentClass->RodOfChampsGiven = true;
                    }
                }
                if (IsActiveThreadTerminating()) return;
                PlayCutscene(cutscene);
                if (ParentClass->PrizeNumber == FishingPrizes) {
                    if (IsActiveThreadTerminating()) return;
                    ParentClass->CompetitionWon = true;
                    Game->SetQuestCardObjective(Game->GetActiveQuestName(), "TEXT_QUEST_FISHING_COMPETITION_OBJECTIVE_02", "", "");
                }
            }
        } else if (weight == ParentClass->BiggestFish) {
            if (IsActiveThreadTerminating()) return;
            csargs["$ARG1"] = "TEXT_CS_B15_FISH_SAMESIZE" + suffix;
            PlayCutscene("CS_FISHCOMP_CHECKFISH_GIVETIP");
        } else if (ParentClass->CompetitionWon) {
            if (IsActiveThreadTerminating()) return;
            csargs["$ARG1"] = "TEXT_CS_B15_FISH_COMMENT_BONUS" + GFIntToCharString(prize);
            PlayCutscene("CS_FISHCOMP_CHECKFISH_GENERIC");
        } else {
            if (IsActiveThreadTerminating()) return;
            csargs["$ARG1"] = "TEXT_CS_B15_FISH_SMALLER";
            PlayCutscene("CS_FISHCOMP_CHECKFISH_GIVETIP");
        }
        if (weight > ParentClass->BiggestCompetitorFish) {
            if (IsActiveThreadTerminating()) return;
            ParentClass->BiggestCompetitorFish = weight;
            ParentClass->Winner = "HERO";
            ParentClass->RefreshCompetition();
        }
    } else {
        if (IsActiveThreadTerminating()) return;
        if (ParentClass->AttackCounter > 0) {
            if (IsActiveThreadTerminating()) return;
            csargs["$ARG1"] = "TEXT_CS_B15_FISH_REFUSE_NASTY";
        } else {
            if (IsActiveThreadTerminating()) return;
            csargs["$ARG1"] = "TEXT_CS_B15_FISH_REFUSE";
        }
        PlayCutscene("CS_FISHCOMP_CHECKFISH_GENERIC");
    }
}