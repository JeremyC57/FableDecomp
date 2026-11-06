// NScript::CV_BookCollectingScript::CBS_Teacher::AskForBook(long book, CCharString refusal) -- offer to take the book (yes/no question); on yes mark it read and donated, take it from the hero, thank (good or grudging line, morality and reputation for a wanted book), play its CS_SCHOOLBOOK_<n> cut-scene with the hero and teacher cast and the book's arguments, then award the good / evil wizard hat or the silver key when earned and spawn the parent's BookReaction(book); on no remember the request and say the refusal (or never mind); returns whether the book was accepted
// Leaf: ?AskForBook@CBS_Teacher@CV_BookCollectingScript@NScript@@AAE_NJVCCharString@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00E55CE0, 2529 bytes, private thiscall, `ret 8`, no EH frame.
// Identity (proven): Ego_r.pdb ?AskForBook@CBS_Teacher@CV_BookCollectingScript@NScript@@AAE_NJVCCharString@@@Z (the landed LookForBook
//   0x00E57530 calls it with "TEXT_QST_B16_BOOK_REFUSED"); it calls the class's AdjustHeroReputation 0x00E56D10 and GetBookSpecificArgs
//   0x00E57020 (Ego_r.pdb spellings) and spawns ?BookReaction@CV_BookCollectingScript@NScript@@QAEXJ@Z 0x00E566F0.
// Callees: IsActiveThreadTerminating 0x00F35B30; CCharString 0x0099EBF0 / ~ 0x0099EAE0 / operator=(const char*) 0x0099EFE0 /
//   operator const char* 0x0099E4C0, operator+(const char*, CCharString) 0x0099F690 / (CCharString, CCharString) 0x0099F570,
//   GFIntToCharString 0x0099F830; CScriptGameResourceObjectScriptedThingBase ctor 0x007E72A0 / ~ 0x007E74D0 / operator= 0x008ABD10
//   and its slots 12 GetScriptThing, 13 Speak(const char*), 26 IsPerformingScriptTask; ~CScriptThing 0x004AA840; the cast map
//   map<CCharString, CScriptGameResourceObjectScriptedThingBase> ctor 0x00CDBF70 / ~ 0x00CDBFB0 / operator[] 0x00CD3D2E and the
//   argument map map<CCharString, CCharString> ctor 0x009AC2D0 / ~ 0x009AC310 / clear 0x009AACE0 / operator[] 0x009AC700 (all out of
//   line in this TU: explicit-specialisation declarations, the landed 0x00DC1770 spelling); RunCutsceneMacro 0x00CBFB7D; operator new
//   0x00BFEA1A, CSpawnedFuncBase(const CCharString&, unsigned long) 0x00CDD450 (CSpawnedFunc1<CV_BookCollectingScript, long> vtable
//   0x012E6228), CScriptBase::AddSpawnedFunction 0x00CB7E50; interface slots 7 NewScriptFrame, 8 StartScriptingEntity, 39
//   MsgIsQuestionAnsweredYesOrNo, 70 GetHero, 114 GiveHeroYesNoQuestion, 125 TakeObjectFromHero, 156 GiveHeroMorality, 264 GetHealth,
//   371 FixMovieSequenceCamera, 374 FadeScreenIn.
// Layout: CBS_Teacher = Ego_r.pdb (BadBooksForHat +0x1c, GoodBooksForHat +0x20, BooksWanted +0x24, BooksAccepted +0x28, MoralityReward
//   +0x30, seh_me +0x34); CV_BookCollectingScript = Ego_r.pdb with retail STLport containers (ReadingBook +0x48, HatRewarded +0x79,
//   KeyRewarded +0x7a, BooksDonated +0x84, GoodBooksDonated +0x88, LastBookRequested +0x8c, BookDef +0x94, BookDonated +0xa0 as bytes:
//   the landed LookForBook model).
// Shape: one function-scope result byte; `X; if (answer == 1) {X; ...} else {X; ...}` (VC7.1 hoists the arms' first termination
//   check above the compare); the cut-scene objects (hero handle, cast map, argument map) live in an inner block that closes before
//   the spawn, so the spawn's name temporaries reuse their frame; the region name is an explicit CCharString("") argument (built
//   after the new-expression, conditional name temporaries with flags 7 in ebx); `book` rides in ebp and the question string takes
//   its argument slot.
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCHARSTRING_CONCAT_STRING
#define FABLE_RETAIL_NSCRIPT
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CSCRIPTGAMERESOURCEOBJECTSCRIPTEDTHINGBASE_OUTOFLINE_CTOR
#define FABLE_RETAIL_CSCRIPTGAMERESOURCEOBJECTSCRIPTEDTHINGBASE_OUTOFLINE_DTOR
#define FABLE_RETAIL_CSCRIPTGAMERESOURCEOBJECTSCRIPTEDTHINGBASE_OUTOFLINE_ASSIGN
#define FABLE_RETAIL_STLP_MAP
#define FABLE_RETAIL_SUSPENDABLE_PROCESS
#include "fable_string.h"
CCharString __fastcall GFIntToCharString(long value);       // 0x0099F830
#include "engine/CCountedPointer.h"
#include "engine/CScriptThing.h"
#include "engine/CGameScriptInterface.h"
#include "engine/CEntityScriptBase.h"
#include "engine/CScriptBase.h"
#include "engine/CScriptGameResourceObjectScriptedThingBase.h"
#include "engine/fable_script_cutscene.h"
#include "engine/CSpawnedFuncBase.h"
#include "engine/CSpawnedFunc1.h"
template<> std::map<CCharString,CScriptGameResourceObjectScriptedThingBase>::map();      // 0x00CDBF70
template<> std::map<CCharString,CScriptGameResourceObjectScriptedThingBase>::~map();     // 0x00CDBFB0
template<> std::map<CCharString,CCharString>::map();                                     // 0x009AC2D0
template<> std::map<CCharString,CCharString>::~map();                                    // 0x009AC310
template<> void std::map<CCharString,CCharString>::clear();                              // 0x009AACE0
class CQ_SunnyvaleMasterData;
namespace NScript {
class CV_BookCollectingScript : public CScriptBase {
public:
    CGameScriptInterface* Game;                              // +0x40
    CQ_SunnyvaleMasterData* PMasterData;                     // +0x44
    bool ReadingBook;                                        // +0x48
    unsigned char BookReactions[0xc];                        // +0x4c vector<CConversation>
    CScriptGameResourceObjectScriptedThingBase seh_Boy;      // +0x58
    CScriptGameResourceObjectScriptedThingBase seh_Girl;     // +0x68
    bool DoneIntro;                                          // +0x78
    bool HatRewarded;                                        // +0x79
    bool KeyRewarded;                                        // +0x7a
    long GossipState;                                        // +0x7c
    bool HasInformation;                                     // +0x80
    long BooksDonated;                                       // +0x84
    long GoodBooksDonated;                                   // +0x88
    long LastBookRequested;                                  // +0x8c
    long BooksInGame;                                        // +0x90
    CCharString* BookDef;                                    // +0x94 vector<CCharString> _M_start
    void* BookDefRest[2];
    bool* BookDonated;                                       // +0xa0
    void* BookDonatedRest[2];
    bool* BookOwned;                                         // +0xac
    void BookReaction(long book);                            // 0x00E566F0
    class CBS_Teacher : public CEntityScriptBase {
    public:
        CV_BookCollectingScript* ParentClass;                // +0x14
        CQ_SunnyvaleMasterData* PMasterData;                 // +0x18
        long BadBooksForHat;                                 // +0x1c
        long GoodBooksForHat;                                // +0x20
        long BooksWanted;                                    // +0x24
        long BooksAccepted;                                  // +0x28
        long BooksComment;                                   // +0x2c
        long MoralityReward;                                 // +0x30
        CScriptGameResourceObjectScriptedThingBase seh_me;   // +0x34
    private:
        bool AskForBook(long book, CCharString refusal);
        void AdjustHeroReputation(EOpinionDeedType deed);    // 0x00E56D10
        void GetBookSpecificArgs(long book, std::map<CCharString,CCharString>& args);   // 0x00E57020
    };
};
}
using namespace NScript;

bool CV_BookCollectingScript::CBS_Teacher::AskForBook(long book, CCharString refusal)
{
    bool result = false;
    CCharString question = "TEXT_QST_B16_OFFER_BOOK_" + GFIntToCharString(book);
    Game->GiveHeroYesNoQuestion(question, "TEXT_OBJECT_HERO_ANSWER_YES", "TEXT_OBJECT_HERO_ANSWER_NO", "", true);
    long answer;
    while ((answer = Game->MsgIsQuestionAnsweredYesOrNo()) < 0) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating()) return false;
    }
    if (IsActiveThreadTerminating()) return false;
    if (answer == 1) {
        if (IsActiveThreadTerminating()) return false;
        ParentClass->ReadingBook = true;
        Game->TakeObjectFromHero(ParentClass->BookDef[book]);
        ParentClass->BookDonated[book] = true;
        ++ParentClass->BooksDonated;
        result = true;
        if (book < BooksWanted) {
            if (IsActiveThreadTerminating()) return false;
            ++ParentClass->GoodBooksDonated;
            if (Game->GetHealth(seh_me.GetScriptThing()) > 0.0f) {
                seh_me.Speak(Game->GetHero(), "TEXT_QST_B16_BOOK_ACCEPTED", (ETextGroupSelectionMethod)2, false, true, false);
                while (seh_me.IsPerformingScriptTask()) {
                    Game->NewScriptFrame();
                    if (IsActiveThreadTerminating()) return false;
                }
                if (IsActiveThreadTerminating()) return false;
            }
            Game->GiveHeroMorality((float)MoralityReward);
            AdjustHeroReputation((EOpinionDeedType)0x2c);
        } else {
            if (IsActiveThreadTerminating()) return false;
            if (Game->GetHealth(seh_me.GetScriptThing()) > 0.0f) {
                seh_me.Speak(Game->GetHero(), "TEXT_QST_B16_BOOK_ACCEPTED_GRUDGINGLY", (ETextGroupSelectionMethod)2, false, true, false);
                while (seh_me.IsPerformingScriptTask()) {
                    Game->NewScriptFrame();
                    if (IsActiveThreadTerminating()) return false;
                }
                if (IsActiveThreadTerminating()) return false;
            }
        }
        CCharString cutscene = "CS_SCHOOLBOOK_" + GFIntToCharString(book);
        {
            CScriptGameResourceObjectScriptedThingBase hero;
            Game->StartScriptingEntity(Game->GetHero(), hero, (EScriptAIPriority)4);
            std::map<CCharString, CScriptGameResourceObjectScriptedThingBase> cast;
            cast["Hero"] = hero;
            cast["Teacher"] = seh_me;
            std::map<CCharString, CCharString> args;
            GetBookSpecificArgs(book, args);
            Game->FixMovieSequenceCamera(true);
            RunCutsceneMacro(cutscene, cast, 0, &args, false, true);
            if (!ParentClass->HatRewarded) {
                if (IsActiveThreadTerminating()) return false;
                if (ParentClass->GoodBooksDonated >= GoodBooksForHat) {
                    if (IsActiveThreadTerminating()) return false;
                    args.clear();
                    args["$PRIZE"] = "OBJECT_HERO_HAT_WIZARD_GOOD";
                    cast["Hero"] = hero;
                    cast["Teacher"] = seh_me;
                    RunCutsceneMacro("CS_SCHOOLBOOK_TEACHER_GIVES_HAT", cast, 0, &args, false, true);
                    ParentClass->HatRewarded = true;
                } else if (ParentClass->BooksDonated - ParentClass->GoodBooksDonated >= BadBooksForHat) {
                    if (IsActiveThreadTerminating()) return false;
                    args.clear();
                    args["$PRIZE"] = "OBJECT_HERO_HAT_WIZARD_EVIL";
                    cast["Hero"] = hero;
                    cast["Teacher"] = seh_me;
                    RunCutsceneMacro("CS_SCHOOLBOOK_TEACHER_GIVES_HAT", cast, 0, &args, false, true);
                    ParentClass->HatRewarded = true;
                }
            } else if (!ParentClass->KeyRewarded) {
                if (IsActiveThreadTerminating()) return false;
                if (ParentClass->BooksDonated == BooksAccepted) {
                    if (IsActiveThreadTerminating()) return false;
                    args.clear();
                    args["$PRIZE"] = "OBJECT_SILVER_KEY";
                    cast["Hero"] = hero;
                    cast["Teacher"] = seh_me;
                    RunCutsceneMacro("CS_SCHOOLBOOK_TEACHER_GIVES_KEY", cast, 0, &args, false, true);
                    ParentClass->KeyRewarded = true;
                }
            }
            Game->FadeScreenIn();
            Game->FixMovieSequenceCamera(false);
        }
        ParentClass->ReadingBook = false;
        ParentClass->AddSpawnedFunction(new CSpawnedFunc1<CV_BookCollectingScript, long>(CCharString("ParentClass.") + CCharString("BookReaction"), 0, ParentClass, &CV_BookCollectingScript::BookReaction, book), CCharString(""));
    } else {
        if (IsActiveThreadTerminating()) return false;
        if (book < BooksWanted) {
            if (IsActiveThreadTerminating()) return false;
            ParentClass->LastBookRequested = book;
            if (Game->GetHealth(seh_me.GetScriptThing()) > 0.0f) {
                seh_me.Speak(Game->GetHero(), refusal, (ETextGroupSelectionMethod)2, false, true, false);
                while (seh_me.IsPerformingScriptTask()) {
                    Game->NewScriptFrame();
                    if (IsActiveThreadTerminating()) return false;
                }
                if (IsActiveThreadTerminating()) return false;
            }
        } else {
            if (IsActiveThreadTerminating()) return false;
            if (Game->GetHealth(seh_me.GetScriptThing()) > 0.0f) {
                seh_me.Speak(Game->GetHero(), "TEXT_QST_B16_BOOK_REFUSED_NEVER_MIND", (ETextGroupSelectionMethod)2, false, true, false);
                while (seh_me.IsPerformingScriptTask()) {
                    Game->NewScriptFrame();
                    if (IsActiveThreadTerminating()) return false;
                }
                if (IsActiveThreadTerminating()) return false;
            }
        }
    }
    return result;
}