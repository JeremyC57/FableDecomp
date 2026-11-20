// NScript::CV_BookCollectingScript::CBS_Teacher::Main() -- the Bowerstone slums schoolteacher of the book collection: keep her persistent and set the gossip stage from the books donated; until enough books are accepted, show the information marker only in school hours, wait while the hero-dolls script borrows her, and when the hero talks to her play the intro (and give the quest card and objective) or ask again for the last requested book / look for books / say it is the wrong time; then complete the quest and say the completion line whenever talked to
// Inline-budget: header-contract inline devices, no flag and no pragma (ablation/, each removed in turn) -- FABLE_RETAIL_CWIDESCREENMAGICPAUSEENTITIES_FORCEINLINE_CTOR (retail expands both pause ctors; plain inline: DIFFER(3473v3534)), the row-local `inline CScriptThing::~CScriptThing() { }` (retail expands the GetScriptThing temporary's dtor at one site and calls 0x004AA840 at the others; without it DIFFER(3486v3534)) and the out-of-line `template <> CCountedPointer<CScriptGameResourceObjectMovieBase>::~CCountedPointer()` declaration (retail calls 0x006E7AB0 inside the expanded movie dtors and the whole dtor 0x006E7B80 elsewhere; without it DIFFER(3600v3534)). No per-row flag was needed (the row matches at default flags).
// Leaf: ?Main@CBS_Teacher@CV_BookCollectingScript@NScript@@EAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00E54E90, 3534 bytes, thiscall, plain `ret`, no EH frame.
// Identity (two sources): Ego_r.pdb / FableWin.pdb ?Main@CBS_Teacher@CV_BookCollectingScript@NScript@@EAEXXZ and the Ego_d.pdb proc of the same
//   name; slot 1 of the CBS_Teacher entity vtable 0x012E5F40 (slot 0 scalar deleting dtor 0x00E546A0, 2 Init 0x00E54630, 3 GetParentScript 0x00E54620).
// Callees: IsActiveThreadTerminating 0x00F35B30, CCharString 0x0099EBF0 / ~ 0x0099EAE0, ~CScriptThing 0x004AA840, ScriptedThingBase IsValid
//   0x00CD23B9 / Release 0x00CD2770, movie ctor 0x006E7B60 / dtor 0x006E7B80 / ~CCountedPointer<MovieBase> 0x006E7AB0, CBaseObject 0x0099A380 /
//   ~ 0x0099A430, ~CBaseClass 0x0099A2E0, operator delete 0x00BFE9BC, CV_BookCollectingScript::AddGossip 0x00E55C60, CBS_Teacher::AskForBook
//   0x00E55CE0 / LookForBook 0x00E57530; CGameScriptInterfaceBase slots 7, 8, 70, 184 IsObjectInThingsPossession, 264 GetHealth, 288
//   SetQuestAsCompleted, 295 GiveHeroQuestCardDirectly, 296 SetQuestCardObjective, 360 / 361 Set / ClearThingHasInformation, 370, 379, 477, 647
//   GetTimeOfDay, 655 GetActiveQuestName, 719 RemoveRumourCategory; ScriptedThingBase slots 12 GetScriptThing, 13 Speak, 26 IsPerformingScriptTask;
//   CScriptThing slot 27 MsgIsTalkedToBy.
// Source shape: the parked chunk 845 draft's statement order (its bytes already matched apart from the entry store), written in the generated
//   quest-script text of the landed Mains: `TRetVoid _Fail_Val;` and `return _Fail_Val.Return();` at every exit (a plain `return;` moves the shared
//   exit blocks: the draft's pad-order residue), the seh macro and each speak-and-wait macro on one line, each terminate check on its statement's
//   line. Names from Ego_d.pdb's locals: function scope `_Fail_Val`; blocks {speech_movie, PauseMyEntitiesPlease, {Given}} and {speech_movie,
//   PauseMyEntitiesPlease}; seh_me is the class member (+0x34). Ego_d is a later revision of this script (its line table does not fit the retail
//   statement order in the talk arm), so its line table fixes names and scopes here, not the order.
// Inferred (fitted statement; coordinator ruling 2026-11-20 under the owner's delegation, as the landed 0x00E72630): retail zeroes a
//   conditional-temporary flag at entry (the dead store `mov dword ptr [esp+34h],0` 0x00E54E9C) and never tests it. The bytes prove that a
//   conditional temporary existed in dead code but constrain neither its content nor its place. It is written as one extra operand of the dead
//   post-loop check (Ego_d line 751), with only calls and literals this function already uses. Ego_d's prolog line (603, 36 bytes = the flag-free
//   siblings; 46 with a flag) shows its later revision had no such temporary. Without it DIFFER(3534v3534), 37 diff lines: no entry store, the moved loop pad.
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
#include "engine/TRetVoid.h"
enum EScriptAIPriority { SCRIPT_AI_PRIORITY_LOWEST=0, SCRIPT_AI_PRIORITY_LOW=1, SCRIPT_AI_PRIORITY_MEDIUM=2, SCRIPT_AI_PRIORITY_HIGH=3, SCRIPT_AI_PRIORITY_HIGHEST=4 };
enum ETextGroupSelectionMethod { GROUP_SELECT_NONE=0, GROUP_SELECT_RANDOM=1, GROUP_SELECT_RANDOM_NO_REPEAT=2, GROUP_SELECT_SEQUENTIAL=3, GROUP_SELECT_FIRST_ONLY=4 };
void NScript::CV_BookCollectingScript::CBS_Teacher::Main()
{
    TRetVoid _Fail_Val;
    Game->SetThingPersistent(Me, true);
    if (ParentClass->BooksDonated < 4 && ParentClass->GossipState == 0) {if(IsActiveThreadTerminating())return _Fail_Val.Return();
        ParentClass->AddGossip("Book Collection - Need Books", "TEXT_AI_GOSSIP_BOOK_COLLECTION_NEED", "VILLAGE_BOWERSTONE_SLUMS", "FACTION_VILLAGERS");
        ParentClass->GossipState = 1;
    } else if (ParentClass->BooksDonated > 3 && ParentClass->GossipState == 1) {if(IsActiveThreadTerminating())return _Fail_Val.Return();
        Game->RemoveRumourCategory("Book Collection - Need Books");
        ParentClass->GossipState = 2;
    } else if (ParentClass->BooksDonated > 9 && ParentClass->GossipState < 3) {if(IsActiveThreadTerminating())return _Fail_Val.Return();
        ParentClass->AddGossip("Book Collection - Given Books", "TEXT_AI_GOSSIP_BOOK_COLLECTION_GIVEN", "VILLAGE_BOWERSTONE_SLUMS", "FACTION_VILLAGERS");
        ParentClass->GossipState = 3;
    }
    while (ParentClass->BooksDonated < BooksAccepted) {
        Game->NewScriptFrame();
        if(IsActiveThreadTerminating())return _Fail_Val.Return();
        if (ParentClass->HasInformation && (Game->GetTimeOfDay() > 1615 || Game->GetTimeOfDay() < 845)) {if(IsActiveThreadTerminating())return _Fail_Val.Return();
            Game->ClearThingHasInformation(Me);
            ParentClass->HasInformation = false;
        } else if (!ParentClass->HasInformation && Game->GetTimeOfDay() > 845 && Game->GetTimeOfDay() < 1615) {if(IsActiveThreadTerminating())return _Fail_Val.Return();
            Game->SetThingHasInformation(Me, false, false, true);
            ParentClass->HasInformation = true;
        }
        if (PMasterData->HeroDollsScriptUsingTeacher) {if(IsActiveThreadTerminating())return _Fail_Val.Return();
            while (PMasterData->HeroDollsScriptUsingTeacher) {
                Game->NewScriptFrame();
                if(IsActiveThreadTerminating())return _Fail_Val.Return();
            }
            if(IsActiveThreadTerminating())return _Fail_Val.Return();
            ParentClass->HasInformation = false;
        }
        if (Me.MsgIsTalkedToBy("SCRIPT_NAME_HERO")) {if(IsActiveThreadTerminating())return _Fail_Val.Return();
            CScriptGameResourceObjectMovieBase speech_movie;Game->StartMovieSequence("",speech_movie);CWideScreenMagicPauseEntities PauseMyEntitiesPlease(*Game);
            if(seh_me.IsValid())seh_me.Release();while(!Game->StartScriptingEntity(Me,seh_me,SCRIPT_AI_PRIORITY_HIGHEST)){Game->NewScriptFrame();if(IsActiveThreadTerminating())return _Fail_Val.Return();}if(IsActiveThreadTerminating())return _Fail_Val.Return();
            if (!ParentClass->DoneIntro) {if(IsActiveThreadTerminating())return _Fail_Val.Return();
                if (PMasterData->HeroExposedLadyGrey) {if(IsActiveThreadTerminating())return _Fail_Val.Return();
                    if(Game->GetHealth(seh_me.GetScriptThing())>0.0f){seh_me.Speak(Game->GetHero(),"TEXT_QST_B16_INTRO_B",GROUP_SELECT_NONE,false,true,false);while(seh_me.IsPerformingScriptTask()){Game->NewScriptFrame();if(IsActiveThreadTerminating())return _Fail_Val.Return();}if(IsActiveThreadTerminating())return _Fail_Val.Return();}
                } else {if(IsActiveThreadTerminating())return _Fail_Val.Return();
                    if(Game->GetHealth(seh_me.GetScriptThing())>0.0f){seh_me.Speak(Game->GetHero(),"TEXT_QST_B16_INTRO",GROUP_SELECT_NONE,false,true,false);while(seh_me.IsPerformingScriptTask()){Game->NewScriptFrame();if(IsActiveThreadTerminating())return _Fail_Val.Return();}if(IsActiveThreadTerminating())return _Fail_Val.Return();}
                }
                Game->GiveHeroQuestCardDirectly("OBJECT_QUEST_CARD_BOOK_COLLECTION", Game->GetActiveQuestName(), false);
                Game->SetQuestCardObjective(Game->GetActiveQuestName(), "TEXT_QUEST_BOOK_COLLECTION_OBJECTIVE_01", "", "");
                ParentClass->DoneIntro = true;
            } else if (Game->GetTimeOfDay() >= 845 && Game->GetTimeOfDay() <= 1615) {
                if (ParentClass->LastBookRequested >= 0) {if(IsActiveThreadTerminating())return _Fail_Val.Return();
                    if(Game->GetHealth(seh_me.GetScriptThing())>0.0f){seh_me.Speak(Game->GetHero(),"TEXT_QST_B16_BOOK_REQUEST_AGAIN",GROUP_SELECT_RANDOM_NO_REPEAT,false,true,false);while(seh_me.IsPerformingScriptTask()){Game->NewScriptFrame();if(IsActiveThreadTerminating())return _Fail_Val.Return();}if(IsActiveThreadTerminating())return _Fail_Val.Return();}
                    if (Game->IsObjectInThingsPossession(ParentClass->BookDef[ParentClass->LastBookRequested], Game->GetHero())) {if(IsActiveThreadTerminating())return _Fail_Val.Return();
                        bool Given=AskForBook(ParentClass->LastBookRequested,"TEXT_QST_B16_BOOK_REFUSE_AGAIN");
                        if (!Given) {if(IsActiveThreadTerminating())return _Fail_Val.Return();
                            LookForBook();
                        } else {if(IsActiveThreadTerminating())return _Fail_Val.Return();
                            ParentClass->LastBookRequested = -1;
                        }
                    } else {if(IsActiveThreadTerminating())return _Fail_Val.Return();
                        if(Game->GetHealth(seh_me.GetScriptThing())>0.0f){seh_me.Speak(Game->GetHero(),"TEXT_QST_B16_BOOK_LOST",GROUP_SELECT_NONE,false,true,false);while(seh_me.IsPerformingScriptTask()){Game->NewScriptFrame();if(IsActiveThreadTerminating())return _Fail_Val.Return();}if(IsActiveThreadTerminating())return _Fail_Val.Return();}
                        LookForBook();
                    }
                } else {if(IsActiveThreadTerminating())return _Fail_Val.Return();
                    if(Game->GetHealth(seh_me.GetScriptThing())>0.0f){seh_me.Speak(Game->GetHero(),"TEXT_QST_B16_BOOK_LOOK",GROUP_SELECT_RANDOM_NO_REPEAT,false,true,false);while(seh_me.IsPerformingScriptTask()){Game->NewScriptFrame();if(IsActiveThreadTerminating())return _Fail_Val.Return();}if(IsActiveThreadTerminating())return _Fail_Val.Return();}
                    LookForBook();
                }
            } else {if(IsActiveThreadTerminating())return _Fail_Val.Return();
                if(Game->GetHealth(seh_me.GetScriptThing())>0.0f){seh_me.Speak(Game->GetHero(),"TEXT_QST_B16_BOOK_WRONG_TIME",GROUP_SELECT_RANDOM_NO_REPEAT,false,true,false);while(seh_me.IsPerformingScriptTask()){Game->NewScriptFrame();if(IsActiveThreadTerminating())return _Fail_Val.Return();}if(IsActiveThreadTerminating())return _Fail_Val.Return();}
            }
            if (seh_me.IsValid())
                seh_me.Release();
        }
    }
    if(IsActiveThreadTerminating())return _Fail_Val.Return();
    Game->ClearThingHasInformation(Me);
    Game->SetQuestAsCompleted(Game->GetActiveQuestName(), false, false, false);
    if(IsActiveThreadTerminating())return _Fail_Val.Return();
    for (;;) {
        if (PMasterData->HeroDollsScriptUsingTeacher) {if(IsActiveThreadTerminating())return _Fail_Val.Return();
            while (PMasterData->HeroDollsScriptUsingTeacher) {
                Game->NewScriptFrame();
                if(IsActiveThreadTerminating())return _Fail_Val.Return();
            }
            if(IsActiveThreadTerminating())return _Fail_Val.Return();
        } else if (Me.MsgIsTalkedToBy("SCRIPT_NAME_HERO")) {if(IsActiveThreadTerminating())return _Fail_Val.Return();
            CScriptGameResourceObjectMovieBase speech_movie;Game->StartMovieSequence("",speech_movie);CWideScreenMagicPauseEntities PauseMyEntitiesPlease(*Game);
            if(seh_me.IsValid())seh_me.Release();while(!Game->StartScriptingEntity(Me,seh_me,SCRIPT_AI_PRIORITY_HIGHEST)){Game->NewScriptFrame();if(IsActiveThreadTerminating())return _Fail_Val.Return();}if(IsActiveThreadTerminating())return _Fail_Val.Return();
            if(Game->GetHealth(seh_me.GetScriptThing())>0.0f){seh_me.Speak(Game->GetHero(),"TEXT_QST_B16_COMPLETE",GROUP_SELECT_RANDOM_NO_REPEAT,false,true,false);while(seh_me.IsPerformingScriptTask()){Game->NewScriptFrame();if(IsActiveThreadTerminating())return _Fail_Val.Return();}if(IsActiveThreadTerminating())return _Fail_Val.Return();}
            if (seh_me.IsValid())
                seh_me.Release();
        }
        Game->NewScriptFrame();
        if(IsActiveThreadTerminating())return _Fail_Val.Return();
    }
    // Inferred: the `|| Me.MsgIsTalkedToBy("SCRIPT_NAME_HERO")` operand of this dead post-loop check (see the header: a conditional temporary existed; its content and place are not constrained)
    if (IsActiveThreadTerminating() || Me.MsgIsTalkedToBy("SCRIPT_NAME_HERO")) return _Fail_Val.Return();
}