// NScript::CExpression_StealScript::Main() -- the hero "steal" expression: keep the hero conscious, make the quest persistent; unless the targeted thing is stealable and no movie runs (then deactivate the quest), start a movie, show the steal mini-game bar and count down the thing's steal duration scaled by the hero's stealth level (in frames) while a theft deed searches for witnesses; letting go of the expression button, being hit, being witnessed or the thing dying fails; on success and a percentage-weighted roll mark the thing stolen and shift morality; finally deactivate the quest
// Leaf: ?Main@CExpression_StealScript@NScript@@UAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00EEBDF0, 1240 bytes, thiscall, plain `ret` (three epilogues), no EH frame, default flags.
// Identity (proven, two sources): slot 2 (Main) of the script vtable 0x012F5700 (its only reference 0x012F5708; slot 1 = the landed
//   RegisterMain 0x00EEC2D0, which spawns it), stored by the ctor 0x00EEBDC0 / Alloc 0x00EEC390 that CScriptInfoManager::RegisterScripts
//   registers as "Expression_Steal"; Ego_r.pdb ?Main@CExpression_StealScript@NScript@@UAEXXZ and the Ego_d.pdb proc of the same name.
// Source shape: Ego_d.pdb line table (lines 965..1085, egod_lines/) and locals -- function scope `TargetedThing`, `_Fail_Val`; one block
//   {Terminated, Percentage, Finished, speech_movie, DeedHandle, StealTimer, TimerDuration, {Percentile}} -- in the generated quest-script
//   text of the landed Mains (0x00E377D0, 0x00DDAE40): `return _Fail_Val.Return();`, each terminate check on its statement's line.
//   Ego_d line sizes fix the statement forms: 992 / 993 / 1008 / 1009 .. 4-byte bool stores, 994 a 5-byte `fldz ; fstp` (Percentage=0.0f),
//   999 the 11-byte `_ftol` store (StealTimer), 1032 the 5-byte `fld1 ; fstp` (Percentage=1.0f), 1055 the 9-byte decrement, 1058 the
//   5-byte `jmp` closing `while(!Finished)`, 1070 the 17-byte `long Percentile=rand()%100;`.
// Callees: CScriptThing copy 0x004ABE90 / ~CScriptThing 0x004AA840 / IsAlive 0x004AB130 (expanded in the loop: implementation slot 75,
//   called after it -- c2 site frequency), CScriptBase::SetPredicate 0x00CB7920 / IsActiveThreadTerminating 0x00CB7940, the
//   CIsThingConscious ctor / dtor expanded (vtable 0x012C2FE8), CBaseObject() 0x0099A380 (the expanded movie ctor: vtable 0x01260EF4,
//   PImp zeroed) and ~CScriptGameResourceObjectMovieBase 0x006E7B80 (out of line), CCharString(const char*, long) 0x0099EBF0 / ~ 0x0099EAE0,
//   rand 0x00BFEB16, _ftol2 0x00BFEA70; Game slots 7 NewScriptFrame, 70 GetHero, 71 GetHeroTargetedThing, 159
//   ChangeHeroMoralityDueToTheft, 168 / 169 GetHeroStatLevel / GetHeroStatMax, 228 IsDPadButtonHeldForExpression, 281
//   DeactivateQuestLater, 290 SetQuestAsPersistent, 342 / 343 DisplayMiniGameInfo / UpdateMiniGameInfoBar, 346 IsEntityStealable, 349
//   EntitySetAsStolen, 370 StartMovieSequence, 395 IsInMovieSequence, 629 / 630 / 631 the witness-searching deed, 654 GetConstantFPS,
//   655 GetActiveQuestName, 740 GetStealDuration; CScriptThing slot 21 MsgIsHitBy (on the hero reference). Literals "" 0x0122D70E,
//   EXPRESSION_STEAL 0x0124F534; floats 1.0f 0x0122DED8, 100.0f 0x01230A08. Enumerators: MINIGAME_STEAL 5, HERO_STAT_SKILL_STEALTH 5,
//   OPINION_DEED_TYPE_CRIME_THEFT 6.
// Layout: engine/CExpression_StealScript.h (Game +0x40).
// Header contract bodies: inline CScriptThing::IsAlive (the landed IsAlive text), inline CIsThingConscious ctor / dtor. The movie dtor is the
//   header's plain inline body: retail calls 0x006E7B80, and this TU's COMDAT of it is byte-equal (relocations only). The former
//   FABLE_RETAIL_CSCRIPTGAMERESOURCEOBJECTMOVIEBASE_OUTOFLINE_DTOR opt-in is not needed (device ablation, 2026-11-20).
// No `// Inferred:` lines: the first compile of this text matched.
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_NSCRIPT
#define FABLE_RETAIL_BASE_OBJECT
#include <stdlib.h>
#include "fable_string.h"
#include "engine/CCountedPointer.h"
#include "engine/CScriptThing.h"
#include "engine/EHeroTrainableStatType.h"
#include "engine/EOpinionDeedType.h"
#include "engine/EMiniGameType.h"
#include "engine/CGameScriptInterfaceBase.h"
#include "engine/CIsThingConscious.h"
#include "engine/CScriptGameResourceObjectMovieBase.h"
#include "engine/CExpression_StealScript.h"
#include "engine/TRetVoid.h"
inline bool CScriptThing::IsAlive() const { return PImp.Data && PImp.Data->IsAlive(); }
inline NScript::CIsThingConscious::CIsThingConscious(const CScriptThing& thing) : Thing(thing) { }
inline NScript::CIsThingConscious::~CIsThingConscious() { }

void NScript::CExpression_StealScript::Main()
{
    TRetVoid _Fail_Val;
    SetPredicate(CIsThingConscious(Game->GetHero()));Game->NewScriptFrame();if(IsActiveThreadTerminating())return _Fail_Val.Return();
    Game->SetQuestAsPersistent(Game->GetActiveQuestName(),false);
    CScriptThing TargetedThing=Game->GetHeroTargetedThing();
    if(!Game->IsEntityStealable(TargetedThing)){if(IsActiveThreadTerminating())return _Fail_Val.Return();
        Game->DeactivateQuestLater(Game->GetActiveQuestName(),0);
        return _Fail_Val.Return();
    }
    if(Game->IsInMovieSequence()){if(IsActiveThreadTerminating())return _Fail_Val.Return();
        Game->DeactivateQuestLater(Game->GetActiveQuestName(),0);
        return _Fail_Val.Return();
    }
    {
        CScriptGameResourceObjectMovieBase speech_movie;
        Game->StartMovieSequence("",speech_movie);
        Game->DisplayMiniGameInfo(true,MINIGAME_STEAL);
        bool Finished=false;
        bool Terminated=false;
        float Percentage=0.0f;
        float TimerDuration=(float)Game->GetStealDuration(TargetedThing);
        TimerDuration/=(((float)Game->GetHeroStatLevel(HERO_STAT_SKILL_STEALTH)+1.0f)/((float)Game->GetHeroStatMax(HERO_STAT_SKILL_STEALTH)+1.0f));
        TimerDuration=(float)Game->GetConstantFPS()*TimerDuration;
        long StealTimer=(long)TimerDuration;
        long DeedHandle=Game->EntityPostOpinionDeedKeepSearchingForWitnesses(Game->GetHero(),OPINION_DEED_TYPE_CRIME_THEFT,TargetedThing);
        while(!Finished){Game->NewScriptFrame();if(IsActiveThreadTerminating())return _Fail_Val.Return();
            if(!Game->IsDPadButtonHeldForExpression("EXPRESSION_STEAL")){if(IsActiveThreadTerminating())return _Fail_Val.Return();
                Finished=true;
                Terminated=true;
            }
            if(Game->GetHero().MsgIsHitBy("")){if(IsActiveThreadTerminating())return _Fail_Val.Return();
                Finished=true;
                Terminated=true;
            }
            if(Game->IsDeedWitnessed(DeedHandle)){if(IsActiveThreadTerminating())return _Fail_Val.Return();
                Finished=true;
                Terminated=true;
            }
            Percentage=1.0f-StealTimer/TimerDuration;
            if(Percentage>1.0f){if(IsActiveThreadTerminating())return _Fail_Val.Return();
                Percentage=1.0f;
            }
            Game->UpdateMiniGameInfoBar(Percentage);
            if(TargetedThing.IsAlive()){if(IsActiveThreadTerminating())return _Fail_Val.Return();
                if(Percentage>=1.0f){if(IsActiveThreadTerminating())return _Fail_Val.Return();
                    Finished=true;
                }
            }else{if(IsActiveThreadTerminating())return _Fail_Val.Return();
                Terminated=true;
                Finished=true;
            }
            if(StealTimer>0){if(IsActiveThreadTerminating())return _Fail_Val.Return();
                StealTimer--;
            }
        }
        if(IsActiveThreadTerminating())return _Fail_Val.Return();
        Game->RemoveOpinionDeedStillSearchingForWitnesses(Game->GetHero(),DeedHandle);
        Game->DisplayMiniGameInfo(false,MINIGAME_STEAL);
        if(!Terminated){if(IsActiveThreadTerminating())return _Fail_Val.Return();
            if(TargetedThing.IsAlive()){if(IsActiveThreadTerminating())return _Fail_Val.Return();
                long Percentile=rand()%100;
                if(Percentile<Percentage*100.0f){if(IsActiveThreadTerminating())return _Fail_Val.Return();
                    Game->EntitySetAsStolen(TargetedThing);
                    Game->ChangeHeroMoralityDueToTheft();
                }
            }
        }
    }
    Game->DeactivateQuestLater(Game->GetActiveQuestName(),0);
}
