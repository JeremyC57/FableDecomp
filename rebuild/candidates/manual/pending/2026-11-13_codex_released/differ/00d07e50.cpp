// NScript::CQ_BanditCampBossBattleScript::CCampHostage::Main() -- bandit-camp boss battle, the first hostage: hold an alive filter over Me, take Me under scripted control; until the hostages are rescued or one is killed, shout for help at the hero (four alternating lines, every 10 seconds within 8 units), answer the hero's talk with TEXT_QST_009_HOSTAGE_HELP and the guard-too-close event with TEXT_QST_009_HOSTAGE_CLOSE under a paused speech movie; finally release Me
// Grade: DIFFER(2016v2068) -- 25 lines: retail expands the CScriptGameResourceObjectMovieBase dtor (PImp release 0x006E7AB0, GROBase vtable 0x0126008C store, ~CBaseObject 0x0099A430) at three exit pads (HELP block after its closing terminate test; CLOSE block speech-loop exit and closing test) where VC7.1 calls the COMDAT; everything else matches (the documented per-site MovieBase dtor residue)
// Next idea: find a scope / temporary arrangement that changes VC7.1's per-site choice only for those three pads (MOVIEBASE_FORCEINLINE expands all and adds vtable stores; do-while(0), extra braces, the FORCEINLINE_CTOR and STB OUTOFLINE_DTOR opt-ins do not help; T80 inline meter diagnostic halves the residue, so compare with a matched Main of the same TU)
// Leaf: ?Main@CCampHostage@CQ_BanditCampBossBattleScript@NScript@@EAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00D07E50, 2068 bytes incl. the 4-entry switch table at 0x00D08654; plain `ret`, frame 0x70.
// Identity: Ego_r.pdb / FableWin.pdb ?Main@CCampHostage@CQ_BanditCampBossBattleScript@NScript@@EAEXXZ (private virtual); the single
//   .rdata reference is slot 1 of the CCampHostage entity vtable 0x012C8F8C (slot 0 the landed scalar deleting dtor 0x00D07E30, slot 2
//   the landed Init 0x00D07DD0, slot 3 GetParentScript 0x00D07DC0); was the placeholder Quest009_Gamesmaster_Hostage_Setup.
// Callees: CIsThingAlive (vtable 0x012C32F0) + CScriptThing copy 0x004ABE90 / ~ 0x004AA840, SetPredicate 0x00F35B10,
//   IsActiveThreadTerminating 0x00F35B30, CBaseObject 0x0099A380 / ~ 0x0099A430, ScriptedThingBase IsValid 0x00CD23B9 / Release
//   0x00CD2770 / PlayAnimation 0x007E73D0 / GetScriptThing 0x007E7490 / Speak 0x007E7390 / IsPerformingScriptTask 0x007E7450 / dtor
//   0x007E74D0, CCountedPointer<MovieBase> dtor 0x006E7AB0, ~CTimer 0x00CD4470, IsDistanceBetweenThingsUnder 0x00CBE2FF,
//   CCharString 0x0099EBF0 / ~ 0x0099EAE0; Game slots 7, 8, 70, 264, 364, 365, 366, 370, 379, 475; CScriptThing slot 27
//   MsgIsTalkedToBy; pDSTGame 0x0143E8F8 timer slots 87..90.
// Layout: engine/CQ_BanditCampBossBattleScript.h (FABLE_RETAIL_CQ_BANDITCAMPBOSSBATTLESCRIPT_HOSTAGE): HostagesRescued +0x4e,
//   HostageKilled +0x4f, PlayGuardTooCloseCutscene +0x63.
#define FABLE_RETAIL_NSCRIPT
#define FABLE_RETAIL_CLEVELSCRIPTBASE
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CWIDESCREENMAGICPAUSEENTITIES_FORCEINLINE_CTOR
#define FABLE_RETAIL_CQ_BANDITCAMPBOSSBATTLESCRIPT_HOSTAGE
#include "fable_string.h"
#include "engine/CQ_BanditCampBossBattleScript.h"
#include "engine/CIsThingAlive.h"
#include "engine/CScriptGameResourceObjectScriptedThingBase.h"
template<> CCountedPointer<CScriptGameResourceObjectMovieBase>::~CCountedPointer();   // 0x006E7AB0
#include "engine/CScriptGameResourceObjectMovieBase.h"
#include "engine/CGameScriptInterfaceBase.h"
#include "engine/CWideScreenMagicPauseEntities.h"
#include "engine/CTimer.h"
#include "engine/fable_script_distance.h"
#include "engine/fable_script_animation_defaults.h"
enum EScriptAIPriority { SCRIPT_AI_PRIORITY_LOWEST=0, SCRIPT_AI_PRIORITY_LOW=1, SCRIPT_AI_PRIORITY_MEDIUM=2, SCRIPT_AI_PRIORITY_HIGH=3, SCRIPT_AI_PRIORITY_HIGHEST=4 };
enum ETextGroupSelectionMethod { GROUP_SELECT_NONE=0, GROUP_SELECT_RANDOM=1, GROUP_SELECT_RANDOM_NO_REPEAT=2, GROUP_SELECT_SEQUENTIAL=3, GROUP_SELECT_FIRST_ONLY=4 };
inline NScript::CIsThingAlive::CIsThingAlive(const CScriptThing& thing) : Thing(thing) { }
inline NScript::CIsThingAlive::~CIsThingAlive() { }
extern CGameScriptInterfaceBase* pDSTGame;   // 0x0143E8F8 ?pDSTGame@@3PAVCGameScriptInterfaceBase@@A
inline CTimer::CTimer() : TimerIndex(pDSTGame->RegisterTimer()) { }
inline CTimer::~CTimer() { pDSTGame->DeregisterTimer(TimerIndex); }
__forceinline CTimer& CTimer::operator=(long time) { pDSTGame->SetTimer(TimerIndex, time); return *this; }
__forceinline long CTimer::GetTime() const { return pDSTGame->GetTimer(TimerIndex); }

void NScript::CQ_BanditCampBossBattleScript::CCampHostage::Main()
{
    {
        CIsThingAlive filter(Me);
        SetPredicate(filter);
    }
    Game->NewScriptFrame();
    if (IsActiveThreadTerminating()) return;
    CScriptGameResourceObjectScriptedThingBase seh_me;
    if (seh_me.IsValid()) seh_me.Release();
    while (!Game->StartScriptingEntity(Me, seh_me, SCRIPT_AI_PRIORITY_HIGHEST)) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating()) return;
    }
    if (IsActiveThreadTerminating()) return;
    CTimer shout_timer;
    shout_timer = 0;
    long shout = 0;
    while (!ParentClass.HostagesRescued && !ParentClass.HostageKilled) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating()) return;
        if (shout_timer.GetTime() <= 0 && IsDistanceBetweenThingsUnder(Me, Game->GetHero(), 8.0f)) {
            if (IsActiveThreadTerminating()) return;
            Game->EntitySetFacingAngleTowardsThing(Me, Game->GetHero(), false);
            long conversation = Game->AddNewConversation(Me, false, false);
            Game->AddPersonToConversation(conversation, Game->GetHero());
            seh_me.PlayAnimation("CS_SHOUT_FOR_HELP", false, false, false, true, NGlobalConsole::GDefaultToUsingPhysicsInCutsceneAnimations, false, false);
            switch (shout) {
            case 0:
                Game->AddLineToConversation(conversation, "TEXT_QST_009_HOSTAGE_SHOUT_FIRST", false, Me, Game->GetHero());
                shout = 1;
                break;
            case 1:
                Game->AddLineToConversation(conversation, "TEXT_QST_009_HOSTAGE_SHOUT_SECOND", false, Me, Game->GetHero());
                shout = 2;
                break;
            case 2:
                Game->AddLineToConversation(conversation, "TEXT_QST_009_HOSTAGE_SHOUT_THIRD", false, Me, Game->GetHero());
                shout = 3;
                break;
            case 3:
                Game->AddLineToConversation(conversation, "TEXT_QST_009_HOSTAGE_SHOUT_FOURTH", false, Me, Game->GetHero());
                shout = 2;
                break;
            }
            shout_timer = 10;
        }
        if (Me.MsgIsTalkedToBy("SCRIPT_NAME_HERO")) {
            if (IsActiveThreadTerminating()) return;
            if (seh_me.IsValid()) seh_me.Release();
            while (!Game->StartScriptingEntity(Me, seh_me, SCRIPT_AI_PRIORITY_HIGHEST)) {
                Game->NewScriptFrame();
                if (IsActiveThreadTerminating()) return;
            }
            if (IsActiveThreadTerminating()) return;
            CScriptGameResourceObjectMovieBase speech_movie;
            Game->StartMovieSequence("", speech_movie);
            CWideScreenMagicPauseEntities PauseMyEntitiesPlease(*Game);
            if (Game->GetHealth(seh_me.GetScriptThing()) > 0.0f) {
                seh_me.Speak(Game->GetHero(), "TEXT_QST_009_HOSTAGE_HELP", GROUP_SELECT_NONE, false, true, false);
                while (seh_me.IsPerformingScriptTask()) {
                    Game->NewScriptFrame();
                    if (IsActiveThreadTerminating()) return;
                }
                if (IsActiveThreadTerminating()) return;
            }
        }
        if (ParentClass.PlayGuardTooCloseCutscene) {
            if (IsActiveThreadTerminating()) return;
            if (seh_me.IsValid()) seh_me.Release();
            while (!Game->StartScriptingEntity(Me, seh_me, SCRIPT_AI_PRIORITY_HIGHEST)) {
                Game->NewScriptFrame();
                if (IsActiveThreadTerminating()) return;
            }
            if (IsActiveThreadTerminating()) return;
            CScriptGameResourceObjectMovieBase speech_movie;
            Game->StartMovieSequence("", speech_movie);
            CWideScreenMagicPauseEntities PauseMyEntitiesPlease(*Game);
            if (Game->GetHealth(seh_me.GetScriptThing()) > 0.0f) {
                seh_me.Speak(Game->GetHero(), "TEXT_QST_009_HOSTAGE_CLOSE", GROUP_SELECT_NONE, false, true, false);
                while (seh_me.IsPerformingScriptTask()) {
                    Game->NewScriptFrame();
                    if (IsActiveThreadTerminating()) return;
                }
                if (IsActiveThreadTerminating()) return;
            }
            ParentClass.PlayGuardTooCloseCutscene = false;
        }
    }
    if (!IsActiveThreadTerminating())
        seh_me.Release();
}
