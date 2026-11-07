// NScript::CV_BookCollectingScript::CBS_Teacher::Main() -- the Bowerstone slums schoolteacher of the book collection: keep her persistent and set the gossip stage from the books donated; until enough books are accepted, show the information marker only in school hours, wait while the hero-dolls script borrows her, and when the hero talks to her play the intro (and give the quest card and objective) or ask again for the last requested book / look for books / say it is the wrong time; then complete the quest and say the completion line whenever talked to
// Grade: DIFFER(3534v3534) -- same size; retail has an entry store of 0 to [esp+34h] (a conditional-temporary flag home sharing the slot of the SetQuestAsCompleted GetActiveQuestName temporary) whose source construct is not found, and places the expanded-movie-dtor return pad of the first TalkedTo block before the called-dtor pads (ours places it last)
// Next idea: find the threaded-away conditional temporary that owns the flag (a final-loop && put the store at the right slot but grew the frame by 4); the pad order probably follows from it
// Leaf: ?Main@CBS_Teacher@CV_BookCollectingScript@NScript@@EAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00E54E90, 3534 bytes; entity vtable 0x012E5F40 slot 1.
#define FABLE_RETAIL_CWIDESCREENMAGICPAUSEENTITIES_FORCEINLINE_CTOR
#define FABLE_RETAIL_NSCRIPT
#define FABLE_RETAIL_CLEVELSCRIPTBASE
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_DEF_CLASS_BASE
#include "fable_string.h"
#include "engine/CCountedPointer.h"
#include "engine/C3DVector.h"
#include "engine/CScriptThing.h"
inline CScriptThing::~CScriptThing() { }
#include "engine/CQ_SunnyvaleMasterData.h"
#include "engine/CV_BookCollectingScript.h"
#include "engine/CGameScriptInterfaceBase.h"
#include "engine/CScriptGameResourceObjectScriptedThingBase.h"
class CScriptGameResourceObjectMovieBase;
template <> CCountedPointer<CScriptGameResourceObjectMovieBase>::~CCountedPointer();
#include "engine/CScriptGameResourceObjectMovieBase.h"
#include "engine/CWideScreenMagicPauseEntities.h"
enum EScriptAIPriority { SCRIPT_AI_PRIORITY_LOWEST=0, SCRIPT_AI_PRIORITY_LOW=1, SCRIPT_AI_PRIORITY_MEDIUM=2, SCRIPT_AI_PRIORITY_HIGH=3, SCRIPT_AI_PRIORITY_HIGHEST=4 };
enum ETextGroupSelectionMethod { GROUP_SELECT_NONE=0, GROUP_SELECT_RANDOM=1, GROUP_SELECT_RANDOM_NO_REPEAT=2, GROUP_SELECT_SEQUENTIAL=3, GROUP_SELECT_FIRST_ONLY=4 };
#define BT_T if (IsActiveThreadTerminating()) return
#define BT_SAY(key, method) \
    if (Game->GetHealth(seh_me.GetScriptThing()) > 0.0f) { \
        seh_me.Speak(Game->GetHero(), key, method, false, true, false); \
        while (seh_me.IsPerformingScriptTask()) { \
            Game->NewScriptFrame(); \
            BT_T; \
        } \
        BT_T; \
    }
#define BT_START_SCRIPTING \
    if (seh_me.IsValid()) \
        seh_me.Release(); \
    while (!Game->StartScriptingEntity(Me, seh_me, SCRIPT_AI_PRIORITY_HIGHEST)) { \
        Game->NewScriptFrame(); \
        BT_T; \
    } \
    BT_T

void NScript::CV_BookCollectingScript::CBS_Teacher::Main()
{
    Game->SetThingPersistent(Me, true);
    if (ParentClass->BooksDonated < 4 && ParentClass->GossipState == 0) {
        BT_T;
        ParentClass->AddGossip("Book Collection - Need Books", "TEXT_AI_GOSSIP_BOOK_COLLECTION_NEED", "VILLAGE_BOWERSTONE_SLUMS", "FACTION_VILLAGERS");
        ParentClass->GossipState = 1;
    } else if (ParentClass->BooksDonated > 3 && ParentClass->GossipState == 1) {
        BT_T;
        Game->RemoveRumourCategory("Book Collection - Need Books");
        ParentClass->GossipState = 2;
    } else if (ParentClass->BooksDonated > 9 && ParentClass->GossipState < 3) {
        BT_T;
        ParentClass->AddGossip("Book Collection - Given Books", "TEXT_AI_GOSSIP_BOOK_COLLECTION_GIVEN", "VILLAGE_BOWERSTONE_SLUMS", "FACTION_VILLAGERS");
        ParentClass->GossipState = 3;
    }
    while (ParentClass->BooksDonated < BooksAccepted) {
        Game->NewScriptFrame();
        BT_T;
        if (ParentClass->HasInformation && (Game->GetTimeOfDay() > 1615 || Game->GetTimeOfDay() < 845)) {
            BT_T;
            Game->ClearThingHasInformation(Me);
            ParentClass->HasInformation = false;
        } else if (!ParentClass->HasInformation && Game->GetTimeOfDay() > 845 && Game->GetTimeOfDay() < 1615) {
            BT_T;
            Game->SetThingHasInformation(Me, false, false, true);
            ParentClass->HasInformation = true;
        }
        if (PMasterData->HeroDollsScriptUsingTeacher) {
            BT_T;
            while (PMasterData->HeroDollsScriptUsingTeacher) {
                Game->NewScriptFrame();
                BT_T;
            }
            BT_T;
            ParentClass->HasInformation = false;
        }
        if (Me.MsgIsTalkedToBy("SCRIPT_NAME_HERO")) {
            BT_T;
            CScriptGameResourceObjectMovieBase movie;
            Game->StartMovieSequence("", movie);
            CWideScreenMagicPauseEntities PauseMyEntitiesPlease(*Game);
            BT_START_SCRIPTING;
            if (!ParentClass->DoneIntro) {
                BT_T;
                if (PMasterData->HeroExposedLadyGrey) {
                    BT_T;
                    BT_SAY("TEXT_QST_B16_INTRO_B", GROUP_SELECT_NONE);
                } else {
                    BT_T;
                    BT_SAY("TEXT_QST_B16_INTRO", GROUP_SELECT_NONE);
                }
                Game->GiveHeroQuestCardDirectly("OBJECT_QUEST_CARD_BOOK_COLLECTION", Game->GetActiveQuestName(), false);
                Game->SetQuestCardObjective(Game->GetActiveQuestName(), "TEXT_QUEST_BOOK_COLLECTION_OBJECTIVE_01", "", "");
                ParentClass->DoneIntro = true;
            } else if (Game->GetTimeOfDay() >= 845 && Game->GetTimeOfDay() <= 1615) {
                if (ParentClass->LastBookRequested >= 0) {
                    BT_T;
                    BT_SAY("TEXT_QST_B16_BOOK_REQUEST_AGAIN", GROUP_SELECT_RANDOM_NO_REPEAT);
                    if (Game->IsObjectInThingsPossession(ParentClass->BookDef[ParentClass->LastBookRequested], Game->GetHero())) {
                        BT_T;
                        if (!AskForBook(ParentClass->LastBookRequested, "TEXT_QST_B16_BOOK_REFUSE_AGAIN")) {
                            BT_T;
                            LookForBook();
                        } else {
                            BT_T;
                            ParentClass->LastBookRequested = -1;
                        }
                    } else {
                        BT_T;
                        BT_SAY("TEXT_QST_B16_BOOK_LOST", GROUP_SELECT_NONE);
                        LookForBook();
                    }
                } else {
                    BT_T;
                    BT_SAY("TEXT_QST_B16_BOOK_LOOK", GROUP_SELECT_RANDOM_NO_REPEAT);
                    LookForBook();
                }
            } else {
                BT_T;
                BT_SAY("TEXT_QST_B16_BOOK_WRONG_TIME", GROUP_SELECT_RANDOM_NO_REPEAT);
            }
            if (seh_me.IsValid())
                seh_me.Release();
        }
    }
    BT_T;
    Game->ClearThingHasInformation(Me);
    Game->SetQuestAsCompleted(Game->GetActiveQuestName(), false, false, false);
    BT_T;
    for (;;) {
        if (PMasterData->HeroDollsScriptUsingTeacher) {
            BT_T;
            while (PMasterData->HeroDollsScriptUsingTeacher) {
                Game->NewScriptFrame();
                BT_T;
            }
            BT_T;
        } else if (Me.MsgIsTalkedToBy("SCRIPT_NAME_HERO")) {
            BT_T;
            CScriptGameResourceObjectMovieBase movie;
            Game->StartMovieSequence("", movie);
            CWideScreenMagicPauseEntities PauseMyEntitiesPlease(*Game);
            BT_START_SCRIPTING;
            BT_SAY("TEXT_QST_B16_COMPLETE", GROUP_SELECT_RANDOM_NO_REPEAT);
            if (seh_me.IsValid())
                seh_me.Release();
        }
        Game->NewScriptFrame();
        BT_T;
    }
}
