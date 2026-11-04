// NScript::CQ_EndGameScript::CHookCoastTeleporter::Main() -- Hook Coast teleporter: unless the undead bar starts full, give Mother's two instructions and show the bar; drain the bar by EndGame_BarDecrementAmount every EndGame_BarDecrementInterval until it reaches EndGame_UpperBarLimit; remove the bar and Me's mini-map marker; if it was not full at the start, play CS_GATEWAY_OPEN with the hero, fade out, activate Q_EndGame_Barriers, teleport the hero to MG_EndGameTeleport and add log-book entry 240; then idle until terminated
// Grade: DIFFER(1274v1319) -- dl=74: c2's static block frequency (INLINE_DEPTH_REVISIT) puts our cut-scene sites colder and our final exit hotter than retail's: retail expands the movie ctor/dtor (0x0099A380 + vptr + zero PImp; 0x006E7AB0 + vptr + 0x0099A430) and calls ~CTimer 0x00CD4470 at the final idle-loop exit; ours calls the movie ctor/dtor and expands the final ~CTimer (cross-jumped with the post-loop exit, so the three main-loop return tails move and TimerIndex lives in ebp, retail edi)
// Leaf: ?Main@CHookCoastTeleporter@CQ_EndGameScript@NScript@@EAEXXZ
// Flags: none
// Retail: Fable.exe (TLC, VC7.1) @ 0x00D29880, 1319 bytes, thiscall, plain `ret`, no EH frame. Literals "" 0x0122D70E,
//   "TEXT_QST_025_MOTHER_INSTRUCTIONS_10" 0x012CBD4C, "..._20" 0x012CBD28, "HERO" 0x01255174, "CS_GATEWAY_OPEN" 0x012CBD18,
//   "Q_EndGame_Barriers" 0x012CBD04, "MG_EndGameTeleport" 0x012CBCF0.
// Identity (two sources): vtable 0x012CBC3C slot 1 (slot 2 Init 0x00D29020, slot 3 GetParentScript 0x00D29010; Alloc 0x00D28F70,
//   registration "HookCoastTeleporter" in 0x00D27460) + Ego_r.pdb / FableWin.pdb ?Main@CHookCoastTeleporter@CQ_EndGameScript@NScript@@EAEXXZ.
// Layout: engine/CQ_EndGameScript.h (ParentClass is a reference: UndeadBar +0x48, GuiCounter +0x4c, ShowUndeadBar +0x54);
//   pGlobalScriptDef 0x0143E90C EndGame_UpperBarLimit +0xbe4 / EndGame_BarDecrementAmount +0xbe8 / EndGame_BarDecrementInterval +0xbec.
// Callees: IsActiveThreadTerminating 0x00F35B30; CCharString 0x0099EBF0 / ~ 0x0099EAE0; CTimer slots 87 RegisterTimer / 88
//   DeregisterTimer / 89 SetTimer / 90 GetTimer through pDSTGame 0x0143E8F8, ~CTimer 0x00CD4470; __ftol2 0x00BFEA70; scripted-thing
//   handle ctor 0x007E72A0 / dtor 0x007E74D0 / IsValid 0x00CD23B9 / Release 0x00CD2770; CBaseObject operator= 0x0099A3B0 and the
//   handle's CCountedPointer operator= 0x008AB1E0; cast map ctor 0x00CDBF70 / operator[] 0x00CD3D2E / dtor 0x00CDBFB0; movie PImp dtor
//   0x006E7AB0; RunCutsceneMacro 0x00CBFB7D and AddLogbookStoryEntry 0x00CBE87F (__fastcall); ~CScriptThing 0x004AA840 (temporary);
//   CGameScriptInterfaceBase slots 7, 8, 70, 72, 274, 278, 332, 338, 352, 370, 373, 376, 379, 472.
// Shape: the EndMission 0x00E13310 cut-scene prelude (auto_inline(off) movie CCountedPointer dtor, __forceinline STB operator=,
//   FABLE_RETAIL_STLP_MAP) + the header's out-of-line STB ctor/dtor switches and the pause FORCEINLINE_CTOR switch; plain inline CTimer.
// Tries (10): a base 1258v1319; b pause ctor forced + STB op= forced 1274 (this); c/d final loop as do-while / while+break (same);
//   e every terminate check do-while(0) (1265, dl 105: everything hot); f/i/j the cut-scene block as do { } while (0) 1322v1319 dl 96
//   (movie expanded like retail, final ~CTimer still expanded); g do-while(0) after the wait loop only (no effect); h else-branch first.
//   /d2inlT meter: our final exit scores ~55-60 (needs > 74); one more main-loop exit share would push it past 74 (diagnostic only).
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_NSCRIPT
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_STLP_MAP
#define FABLE_RETAIL_CWIDESCREENMAGICPAUSEENTITIES_FORCEINLINE_CTOR
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_CSCRIPTGAMERESOURCEOBJECTSCRIPTEDTHINGBASE_OUTOFLINE_ASSIGN
#define FABLE_RETAIL_CSCRIPTGAMERESOURCEOBJECTSCRIPTEDTHINGBASE_OUTOFLINE_CTOR
#define FABLE_RETAIL_CSCRIPTGAMERESOURCEOBJECTSCRIPTEDTHINGBASE_OUTOFLINE_DTOR
enum EScriptAIPriority { SCRIPT_AI_PRIORITY_LOWEST,SCRIPT_AI_PRIORITY_LOW,SCRIPT_AI_PRIORITY_MEDIUM,SCRIPT_AI_PRIORITY_HIGH,SCRIPT_AI_PRIORITY_HIGHEST };
#include "fable_string.h"
#include "engine/CCountedPointer.h"
#include "engine/CScriptThing.h"
#include "engine/CScriptGameResourceObjectScriptedThingBase.h"
#pragma auto_inline(off)
template<> CCountedPointer<CScriptGameResourceObjectMovieBase>::~CCountedPointer()
{
 if(Info){--Info->RefCount;if(Info->RefCount==0){Info->DeleteFunc(Info->Data);delete Info;}}
 Data=0;Info=0;
}
template<> CCountedPointer<CScriptGameResourceObjectScriptedThingBase>& CCountedPointer<CScriptGameResourceObjectScriptedThingBase>::operator=(const CCountedPointer<CScriptGameResourceObjectScriptedThingBase>& other);
#pragma auto_inline(on)
#include "engine/CScriptGameResourceObjectMovieBase.h"
#include "engine/CRGBColour.h"
#include "engine/CGameScriptInterfaceBase.h"
#include "engine/CWideScreenMagicPauseEntities.h"
#include "engine/CQ_EndGameScript.h"
#include "engine/CTimer.h"
#include "stlport/fable_map.h"
#include "engine/fable_script_cutscene.h"
#include "engine/fable_script_def_global.h"
void FABLE_FASTCALL AddLogbookStoryEntry(long);                      // 0x00CBE87F
extern CGameScriptInterfaceBase* pDSTGame;
inline CTimer::CTimer() { TimerIndex = pDSTGame->RegisterTimer(); }
inline CTimer::~CTimer() { pDSTGame->DeregisterTimer(TimerIndex); }
inline CTimer& CTimer::operator=(long time) { pDSTGame->SetTimer(TimerIndex, time); return *this; }
inline long CTimer::GetTime() const { return pDSTGame->GetTimer(TimerIndex); }
__forceinline CScriptGameResourceObjectScriptedThingBase& CScriptGameResourceObjectScriptedThingBase::operator=(const CScriptGameResourceObjectScriptedThingBase& other) { CScriptGameResourceObjectBase::operator=(other); PImp=other.PImp; return *this; }
void NScript::CQ_EndGameScript::CHookCoastTeleporter::Main()
{
    bool bar_full = ParentClass.UndeadBar >= pGlobalScriptDef->EndGame_UpperBarLimit;
    if (ParentClass.UndeadBar < pGlobalScriptDef->EndGame_BarDecrementAmount) {
        if (IsActiveThreadTerminating()) return;
        Game->HeroReceiveMessageFromGuildMaster("TEXT_QST_025_MOTHER_INSTRUCTIONS_10", "", true, true);
        Game->Pause(3.0f);
        Game->HeroReceiveMessageFromGuildMaster("TEXT_QST_025_MOTHER_INSTRUCTIONS_20", "", true, true);
        Game->Pause(4.0f);
        ParentClass.ShowUndeadBar = true;
    }
    CTimer decrement_timer;
    decrement_timer = 1;
    while (ParentClass.UndeadBar < pGlobalScriptDef->EndGame_UpperBarLimit) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating()) return;
        if (decrement_timer.GetTime() == 0) {
            if (IsActiveThreadTerminating()) return;
            if (ParentClass.UndeadBar < pGlobalScriptDef->EndGame_BarDecrementAmount) {
                if (IsActiveThreadTerminating()) return;
                ParentClass.UndeadBar = 0.0f;
            } else {
                if (IsActiveThreadTerminating()) return;
                ParentClass.UndeadBar -= pGlobalScriptDef->EndGame_BarDecrementAmount;
            }
            decrement_timer = (long)pGlobalScriptDef->EndGame_BarDecrementInterval;
        }
        Game->UpdateQuestInfoBar(ParentClass.GuiCounter, ParentClass.UndeadBar, -1.0f, -1.0f);
    }
    if (IsActiveThreadTerminating()) return;
    Game->RemoveQuestInfoElement(ParentClass.GuiCounter);
    Game->MiniMapRemoveMarker(Me);
    if (!bar_full) {
        if (IsActiveThreadTerminating()) return;
        {
            CScriptGameResourceObjectScriptedThingBase seh_hero;
            if (seh_hero.IsValid()) seh_hero.Release();
            while (!Game->StartScriptingEntity(Game->GetHero(), seh_hero, SCRIPT_AI_PRIORITY_HIGHEST)) {
                Game->NewScriptFrame();
                if (IsActiveThreadTerminating()) return;
            }
            if (IsActiveThreadTerminating()) return;
            std::map<CCharString,CScriptGameResourceObjectScriptedThingBase> cast;
            cast["HERO"] = seh_hero;
            CScriptGameResourceObjectMovieBase movie;
            Game->StartMovieSequence("", movie);
            CWideScreenMagicPauseEntities pause(*Game);
            RunCutsceneMacro("CS_GATEWAY_OPEN", cast, 0, 0, false, true);
        }
        Game->FadeScreenOut(0.5f, 0.0f, CRGBColour(0,0,0,255));
        Game->ActivateQuestWithoutLoadingResources("Q_EndGame_Barriers");
        Game->EntityTeleportToThing(Game->GetHero(), Game->GetThingWithScriptName("MG_EndGameTeleport"), false);
        AddLogbookStoryEntry(240);
    }
    for (;;) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating()) return;
    }
}
