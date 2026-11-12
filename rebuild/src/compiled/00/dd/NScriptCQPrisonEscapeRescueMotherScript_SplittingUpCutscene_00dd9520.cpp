// NScript::CQ_PrisonEscapeRescueMotherScript::SplittingUpCutscene() -- once the mother follows and the graveyard is loaded: teleport the hero to the splitting-up outro marker, play the prison video and the CS_MOTHER_SPLITTING_UP cut-scene, then complete and retire the prison-escape quests
// Lever (chunk Q13): the inline GetAngleXY slot wrapper returns its call parenthesised, `return (PImp.Data->GetAngleXY());`: the PAREN tuple moves the fstp of the EntityTeleportToPosition angle temporary one scheduling slot, after the inline GetPos() null re-test (`mov ecx,[esp+20h] ; cmp ecx,edi ; fstp [esp+0Ch]`, 0x00DD9631..0x00DD9637), as retail. Graded RELOCATION_MATCH with FABLE_HDR_OVERLAY=agentQ13/hdr (no overlay header used).
// Leaf: ?SplittingUpCutscene@CQ_PrisonEscapeRescueMotherScript@NScript@@QAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00DD9520, 986 bytes (+ int3 padding), thiscall, plain `ret`.
// Identity: Ego_r.pdb / FableWin.pdb ?SplittingUpCutscene@CQ_PrisonEscapeRescueMotherScript@NScript@@QAEXXZ (engine/CQ_PrisonEscapeRescueMotherScript.h);
//   the body plays "CS_MOTHER_SPLITTING_UP" at the "OUTRO_SPLITTING_UP" marker; spawned from Main 0x00DD2CE0 (CSpawnedFunc vtable 0x012DB954).
// Callees: CScriptBase::IsActiveThreadTerminating 0x00CB7940, CCharString 0x0099EBF0 / 0x0099EAE0, CBaseObject() 0x0099A380, the
//   scripted-thing handle's Release 0x00CD2770 / operator= 0x008ABD10 / dtor 0x007E74D0, the movie handle ctor 0x006E7B60 / dtor
//   0x006E7B80, map<CCharString, CScriptGameResourceObjectScriptedThingBase> ctor 0x00CDBF70 / dtor 0x00CDBFB0 / operator[] 0x00CD3D2E,
//   RunCutsceneMacro 0x00CBFB7D (__fastcall), ~CScriptThing 0x004AA840; CScriptThing slots 6 GetPos, 10 GetAngleXY and 75 IsAlive
//   (inline through PImp, NullCoord 0x0143E8E0); Game slots 7, 8, 12 IsRegionLoaded, 70 GetHero, 72 GetThingWithScriptName, 108
//   RemoveThing, 128 GiveHeroExperience, 134 SetGuardsIgnoreCrimes, 151 ResetPlayerCreatureCombatMultiplier, 281 DeactivateQuestLater,
//   288 SetQuestAsCompleted, 315 / 318 the death resets, 369 PlayAVIMovie, 370 StartMovieSequence, 473 EntityTeleportToPosition,
//   565 SetTeleportingAsActive, 655 GetActiveQuestName; pGlobalScriptDef 0x0143E90C PrisonEscapeExperienceFromMother +0xDE4.
#include "fable_compiler.h"
#define FABLE_RETAIL_NSCRIPT
#define FABLE_RETAIL_CLEVELSCRIPTBASE
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_STLP_MAP
#define FABLE_RETAIL_CSCRIPTGAMERESOURCEOBJECTMOVIEBASE_OUTOFLINE_CTOR
#define FABLE_RETAIL_CSCRIPTGAMERESOURCEOBJECTMOVIEBASE_OUTOFLINE_DTOR
#define FABLE_RETAIL_CSCRIPTGAMERESOURCEOBJECTSCRIPTEDTHINGBASE_OUTOFLINE_DTOR
#define FABLE_RETAIL_CSCRIPTGAMERESOURCEOBJECTSCRIPTEDTHINGBASE_OUTOFLINE_ASSIGN
#include "fable_string.h"
#include "engine/C3DVector.h"
#include "engine/CCountedPointer.h"
#include "engine/CScriptThing.h"
#include "stlport/fable_containers.h"
#include "engine/CQ_SunnyvaleMasterData.h"
#include "engine/CQ_PrisonEscapeRescueMotherScript.h"
#include "engine/CScriptGameResourceObjectScriptedThingBase.h"
class CScriptGameResourceObjectMovieBase;
template <> CCountedPointer<CScriptGameResourceObjectMovieBase>::~CCountedPointer();
#include "engine/CScriptGameResourceObjectMovieBase.h"
#include "engine/CGameScriptInterfaceBase.h"
#include "engine/CScriptDef.h"
#include "engine/CDefPointer.h"
#include "stlport/fable_map.h"
enum EScriptAIPriority { SCRIPT_AI_PRIORITY_HIGHEST = 4 };
extern CDefPointer<const CScriptDef> pGlobalScriptDef;     // 0x0143E90C
void FABLE_FASTCALL RunCutsceneMacro(const CCharString&, std::map<CCharString,CScriptGameResourceObjectScriptedThingBase>&, std::map<CCharString,bool>*, std::map<CCharString,CCharString>*, bool, bool);   // 0x00CBFB7D
// Retail expands these at this row's sites (CScriptThing 0x004AA980 / 0x004AAA40 / 0x004AB130): defined inline after the headers.
inline const C3DVector& CScriptThing::GetPos() const { if (!PImp.Data) return NullCoord; return PImp.Data->GetPos(); }
// GetAngleXY returns the forwarded call parenthesised: the PAREN tuple puts the angle argument's fstp after the GetPos null re-test (0x00DD9637).
inline float CScriptThing::GetAngleXY() const { if (!PImp.Data) return 0.0f; return (PImp.Data->GetAngleXY()); }
inline bool CScriptThing::IsAlive() const { if (!PImp.Data) return false; return PImp.Data->IsAlive(); }

void NScript::CQ_PrisonEscapeRescueMotherScript::SplittingUpCutscene()
{
    while (!MotherFollowing) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
    }
    if (IsActiveThreadTerminating())
        return;
    while (!Game->IsRegionLoaded("Graveyard")) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
    }
    if (IsActiveThreadTerminating())
        return;
    CScriptThing outro = Game->GetThingWithScriptName("OUTRO_SPLITTING_UP");
    Game->EntityTeleportToPosition(Game->GetHero(), outro.GetPos(), outro.GetAngleXY(), true, false);
    Game->PlayAVIMovie("Data\\Video\\8_prison_2_comp.xmv");
    CScriptGameResourceObjectScriptedThingBase hero;
    Game->StartScriptingEntity(Game->GetHero(), hero, SCRIPT_AI_PRIORITY_HIGHEST);
    CScriptGameResourceObjectScriptedThingBase motherHandle;   // declared and never used (its ctor / dtor stay: 0x00DD96CC, 0x00DD98D8)
    CScriptThing mother = Game->GetThingWithScriptName("MotherTorture");
    if (mother.IsAlive()) {
        if (IsActiveThreadTerminating())
            return;
        Game->RemoveThing(mother, false, true);
    }
    CScriptGameResourceObjectMovieBase movie;
    Game->StartMovieSequence("", movie);
    std::map<CCharString, CScriptGameResourceObjectScriptedThingBase> cast;
    cast["Hero"] = hero;
    RunCutsceneMacro("CS_MOTHER_SPLITTING_UP", cast, 0, 0, false, true);
    hero.Release();
    Game->ResetPlayerCreatureCombatMultiplier();
    Game->GiveHeroExperience(pGlobalScriptDef->PrisonEscapeExperienceFromMother);
    Game->SetQuestAsCompleted(Game->GetActiveQuestName(), true, false, false);
    Game->SetGuardsIgnoreCrimes(false);
    Game->SetTeleportingAsActive(true);
    Game->ResetWhetherToFailQuestOnDeathToDefault();
    Game->ResetDeathRecoveryMarkerNameToDefault();
    Game->DeactivateQuestLater(Game->GetActiveQuestName(), 0);
    Game->DeactivateQuestLater("Q_PrisonEscapeFromCell", 0);
}