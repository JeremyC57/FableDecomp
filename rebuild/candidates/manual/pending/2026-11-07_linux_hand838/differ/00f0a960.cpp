// NScript::CQ_AmbushTradersScript::CAT_BanditContact::Main() -- the Ambush Traders bandit contact (the woman): live while alive; wander round her post with the hero's appearance seed recorded and information flagged; each frame: when the hero attacked her retake control and say TEXT_QST_003_CONTACT_ATTACK under a paused speech movie, then resume wandering; once the intro is done leave when Bowerstone Posh unloads; when the hero first talks to her play CS_GIBBETHILL_AMBUSH_PREINTRO with HERO / WOMAN, teleport the hero to BowerstoneJailHSP and remove her
// Grade: DIFFER(2142v1985) -- 181 diff lines: the CIsThingAlive / by-value CScriptThing copy blocker (GOTCHAS chunks 786 / 810 / 835): retail calls the CScriptThing copy 0x004ABE90 inside the inlined CIsThingAlive ctor and at the fifth by-value Me copy (the four set-up copies expanded, the attacked-branch SetScriptingStateGroup copy called) while VC7.1 here expands all of them (ours also caches the CScriptThing vtable in esi), plus the inline-budget tail of that: one seh_hero ctor and two STB / CScriptThing dtors expanded where retail calls them
// Next idea: only the budget answer to the nested-copy blocker (inline_depth(0) reproduces it, forbidden); non-GOTCHAS levers tried: the CIsThingAlive ctor defined after Main (f0_a3, no change), no IMPLICIT_COPY (f0_a2, md317), loop forms (for(;;NSF) md181, for(;!T;NSF) md199)
// Leaf: ?Main@CAT_BanditContact@CQ_AmbushTradersScript@NScript@@EAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00F0A960, 1985 bytes, thiscall, plain `ret` (two epilogues), frame 0x98.
// Identity (proven): Ego_r.pdb ?Main@CAT_BanditContact@CQ_AmbushTradersScript@NScript@@EAEXXZ; slot 1 of the CAT_BanditContact entity
//   vtable 0x012F7EA0 (the address's one .rdata reference); Alloc 0x00F057E0 news 0x1c.
// Callees: CCharString() 0x0099E4B0 / (const char*) 0x0099EBF0 / ~ 0x0099EAE0, CBasicString<char>::Compare 0x00411570, SetPredicate
//   0x00F35B10 with the CIsThingAlive temporary (vtable 0x012C32F0, ctor inline, dtor 0x00E241B0), IsActiveThreadTerminating 0x00F35B30,
//   CScriptThing copy 0x004ABE90 / ~ 0x004AA840, CBaseClass 0x0099A2D0 / ~ 0x0099A2E0, ScriptedThingBase (CBaseObject 0x0099A380 /
//   ~ 0x0099A430, ctor 0x007E72A0, IsValid 0x00CD23B9, Release 0x00CD2770, GetScriptThing 0x007E7490, Speak 0x007E7390,
//   IsPerformingScriptTask 0x007E7450, operator= 0x008ABD10, dtor 0x007E74D0), MovieBase ctor 0x006E7B60 / dtor 0x006E7B80 / PImp dtor
//   0x006E7AB0, map<CCharString, STB> ctor 0x00CDBF70 / dtor 0x00CDBFB0 / operator[] 0x00CD3D2E, RunCutsceneMacro 0x00CBFB7D;
//   CGameScriptInterfaceBase slots 7, 8, 23 MsgOnRegionUnloaded, 70, 72, 108 RemoveThing, 264 GetHealth, 360 SetThingHasInformation,
//   370 StartMovieSequence, 371 FixMovieSequenceCamera, 379 PauseAllNonScriptedEntities, 472 EntityTeleportToThing, 483
//   EntityGetAppearanceSeed, 763 / 766 / 769 SetWanderCentrePoint / Min / MaxDistance, 784 SetScriptingStateGroup; pGlobalScriptDef
//   0x0143E90C AmbushTradersContactWanderDistance +0x9e8.
// Layout: engine/CQ_AmbushTradersScript.h (opt-in BANDITCONTACT): ParentClass +0x14 (ContactAttackedByHero +0x56,
//   BanditContactIntroComplete +0x67, ContactSeed +0x88).
#define FABLE_RETAIL_CWIDESCREENMAGICPAUSEENTITIES_FORCEINLINE_CTOR
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_NSCRIPT
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CLEVELSCRIPTBASE
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CCHARSTRING_FORCEINLINE_TEXT_EQ
#define FABLE_RETAIL_CBASICSTRING_TEXT_EQ_INLINE
#define FABLE_RETAIL_CSCRIPTGAMERESOURCEOBJECTSCRIPTEDTHINGBASE_OUTOFLINE_ASSIGN
#define FABLE_RETAIL_CSCRIPTGAMERESOURCEOBJECTMOVIEBASE_FORCEINLINE_CTOR
#define FABLE_RETAIL_CSCRIPTTHING_IMPLICIT_COPY
#define FABLE_RETAIL_STLP_MAP
#define FABLE_RETAIL_CQ_AMBUSHTRADERSSCRIPT_MAIN
#define FABLE_RETAIL_CQ_AMBUSHTRADERSSCRIPT_BANDITCONTACT
#include "fable_string.h"
#include "engine/CCountedPointer.h"
class CScriptGameResourceObjectMovieBase;
template <> CCountedPointer<CScriptGameResourceObjectMovieBase>::~CCountedPointer();                        // 0x006E7AB0
#include "engine/C3DVector.h"
#include "engine/CScriptThing.h"
#include "engine/CGameScriptInterfaceBase.h"
#include "engine/CQ_SunnyvaleMasterData.h"
#include "engine/CEntityScriptBase.h"
#include "stlport/fable_containers.h"
#include "engine/CQ_AmbushTradersScript.h"
#include "engine/CScriptGameResourceObjectScriptedThingBase.h"
#include "engine/CScriptGameResourceObjectMovieBase.h"
#include "engine/CWideScreenMagicPauseEntities.h"
#include "engine/CIsThingAlive.h"
#include "engine/CScriptDef.h"
#include "engine/CDefPointer.h"
#include "engine/fable_script_cutscene.h"
#include "stlport/fable_map.h"
template <> std::map<CCharString, CScriptGameResourceObjectScriptedThingBase>::map();                       // 0x00CDBF70
template <> std::map<CCharString, CScriptGameResourceObjectScriptedThingBase>::~map();                      // 0x00CDBFB0
enum EScriptAIPriority { SCRIPT_AI_PRIORITY_LOWEST=0, SCRIPT_AI_PRIORITY_LOW=1, SCRIPT_AI_PRIORITY_MEDIUM=2, SCRIPT_AI_PRIORITY_HIGH=3, SCRIPT_AI_PRIORITY_HIGHEST=4 };
enum ETextGroupSelectionMethod { GROUP_SELECT_NONE=0, GROUP_SELECT_RANDOM=1, GROUP_SELECT_RANDOM_NO_REPEAT=2, GROUP_SELECT_SEQUENTIAL=3, GROUP_SELECT_FIRST_ONLY=4 };
enum EScriptingStateGroups { SCRIPTING_STATE_GROUP_NONE=0, SCRIPTING_STATE_GROUP_WANDER=4 };
extern CDefPointer<const CScriptDef> pGlobalScriptDef;                                                      // 0x0143E90C
inline NScript::CIsThingAlive::CIsThingAlive(const CScriptThing& thing) : Thing(thing) { }
inline CScriptThing::CScriptThing() { }
inline CScriptThing::~CScriptThing() { }
template<> inline __declspec(noinline) long __fastcall CBasicString<char>::Compare(const char* a, const char* b)
{
    for (;;) {
        char ca = *a;
        if (ca == 0 && *b == 0)
            return 0;
        char cb = *b;
        if (ca < cb)
            return -1;
        if (ca > cb)
            return 1;
        ++a;
        ++b;
    }
}

void NScript::CQ_AmbushTradersScript::CAT_BanditContact::Main()
{
    CCharString region;
    SetPredicate(CIsThingAlive(Me));
    Game->NewScriptFrame();
    if (IsActiveThreadTerminating())
        return;
    CScriptGameResourceObjectScriptedThingBase seh_me;
    CScriptThing exit_marker;
    Game->SetWanderCentrePoint(Me, Me.GetPos());
    Game->SetWanderMinDistance(Me, 0.0f);
    Game->SetWanderMaxDistance(Me, pGlobalScriptDef->AmbushTradersContactWanderDistance);
    Game->SetScriptingStateGroup(Me, SCRIPTING_STATE_GROUP_WANDER);
    Game->EntityGetAppearanceSeed(Me, ParentClass.ContactSeed);
    Game->SetThingHasInformation(Me, true, false, false);
    for (;; Game->NewScriptFrame()) {
        if (IsActiveThreadTerminating())
            return;
        if (ParentClass.ContactAttackedByHero) {
            if (IsActiveThreadTerminating())
                return;
            if (seh_me.IsValid())
                seh_me.Release();
            while (!Game->StartScriptingEntity(Me, seh_me, SCRIPT_AI_PRIORITY_HIGHEST)) {
                Game->NewScriptFrame();
                if (IsActiveThreadTerminating())
                    return;
            }
            if (IsActiveThreadTerminating())
                return;
            CScriptGameResourceObjectMovieBase speech_movie;
            Game->StartMovieSequence("", speech_movie);
            CWideScreenMagicPauseEntities PauseMyEntitiesPlease(*Game);
            if (Game->GetHealth(seh_me.GetScriptThing()) > 0.0f) {
                seh_me.Speak(Game->GetHero(), "TEXT_QST_003_CONTACT_ATTACK", GROUP_SELECT_RANDOM_NO_REPEAT, false, true, false);
                while (seh_me.IsPerformingScriptTask()) {
                    Game->NewScriptFrame();
                    if (IsActiveThreadTerminating())
                        return;
                }
                if (IsActiveThreadTerminating())
                    return;
            }
            if (seh_me.IsValid())
                seh_me.Release();
            Game->SetScriptingStateGroup(Me, SCRIPTING_STATE_GROUP_WANDER);
            ParentClass.ContactAttackedByHero = false;
        }
        else if (ParentClass.BanditContactIntroComplete && Game->MsgOnRegionUnloaded(region)) {
            if (IsActiveThreadTerminating())
                return;
            if (region == "BowerstonePosh") {
                if (IsActiveThreadTerminating())
                    return;
                Game->RemoveThing(Me, false, true);
            }
        }
        else if (Me.MsgIsTalkedToBy("SCRIPT_NAME_HERO")) {
            if (IsActiveThreadTerminating())
                return;
            if (!ParentClass.BanditContactIntroComplete) {
                if (IsActiveThreadTerminating())
                    return;
                CScriptGameResourceObjectScriptedThingBase seh_hero;
                Game->StartScriptingEntity(Game->GetHero(), seh_hero, SCRIPT_AI_PRIORITY_HIGHEST);
                if (seh_me.IsValid())
                    seh_me.Release();
                while (!Game->StartScriptingEntity(Me, seh_me, SCRIPT_AI_PRIORITY_HIGHEST)) {
                    Game->NewScriptFrame();
                    if (IsActiveThreadTerminating())
                        return;
                }
                if (IsActiveThreadTerminating())
                    return;
                std::map<CCharString, CScriptGameResourceObjectScriptedThingBase> cast;
                cast["HERO"] = seh_hero;
                cast["WOMAN"] = seh_me;
                CScriptGameResourceObjectMovieBase cutscene_movie;
                Game->StartMovieSequence("", cutscene_movie);
                CWideScreenMagicPauseEntities PauseMyEntitiesPlease(*Game);
                Game->FixMovieSequenceCamera(true);
                RunCutsceneMacro("CS_GIBBETHILL_AMBUSH_PREINTRO", cast, 0, 0, false, true);
                Game->FixMovieSequenceCamera(false);
                ParentClass.BanditContactIntroComplete = true;
                CScriptThing hsp = Game->GetThingWithScriptName("BowerstoneJailHSP");
                Game->EntityTeleportToThing(Game->GetHero(), hsp, false);
                Game->RemoveThing(Me, false, true);
            }
        }
    }
}
