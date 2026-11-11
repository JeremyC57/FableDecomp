// NScript::CV_BodyGuardScript::CBodyGuard::SetHenchmanAsHired(bool hired) -- hire a henchman (follow and ally the hero, hero faction, join the henchman list and the wage bill, a health bar per henchman type) or dismiss him (stop following, bodyguard faction, leave the list and the wage bill; when his home level is not loaded, warp him out with an effect, mark his type dead with a zero respawn timer and fade him out)
// Leaf: ?SetHenchmanAsHired@CBodyGuard@CV_BodyGuardScript@NScript@@AAEX_N@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00E621F0, 1340 bytes = 1308 bytes of code + `mov edi,edi` + two 4-entry jump tables
//   (0x00E6270C health-bar switch: case 0 -> 0x00E62380 BAD_01, 1 -> 0x00E623D5 BAD_02, 2 -> 0x00E622D6 GOOD_01, 3 -> 0x00E6232B
//   GOOD_02; 0x00E6271C the inlined GetGoldCost: 0/2 -> cheap 0x00E62500, 1/3 -> expensive 0x00E6250E; both pairings checked by
//   hand against the object's $L labels). Thiscall, `ret 4`. The catalog reservation (1384) also covered the next function
//   0x00E62730 (an Alloc: `new 0x94` + the CV_BodyGuardScript ctor 0x00E5D140), not part of this row.
// Identity (two sources): Ego_r.pdb 0x00EB7FA0 and FableWin.pdb 0x039A84C0 ?SetHenchmanAsHired@CBodyGuard@CV_BodyGuardScript@NScript@@AAEX_N@Z
//   (private, bool argument); called by the sibling CBodyGuard::Main 0x00E5FD40 (draft hand-backs H40d/H40g) in the same TU.
// Callees: CEntityScriptBase::IsActiveThreadTerminating 0x00F35B30; Game slots 452 ClearThingBestEnemyTarget, 70 GetHero, 480
//   SetEntityAsRegionFollowing, 229 EntityFollowThing, 599 EntitySetThingAsAllyOfThing, 601 EntitySetInFaction, 321
//   DisplayQuestInfo, 325 AddQuestInfoBarHealth, 235 EntitySetAsAllowedToFollowHero, 338 RemoveQuestInfoElement, 230
//   EntityStopFollowing, 11 IsLevelLoaded, 516 EntitySetAsDamageable, 469 EntitySetTargetable, 844 SetIsPushableByHero (CScriptThing
//   by value: the inline copy built in the argument slot), 745 SetIsThingForcePushable, 8 StartScriptingEntity, 7 NewScriptFrame, 100
//   CreateEffect; CScriptThing slots 6 GetPos / 9 GetHomeMapName / 78 IsEqualTo (inline operator==); vector<CScriptThing>
//   push_back 0x008ADF90 / erase 0x00CD3152; CBodyGuard::GetGoldCost 0x00E5FC70 (called on the hire path, expanded on the dismiss
//   path); pDSTGame 0x0143E8F8 slot 89 SetTimer (inline CTimer::operator=); FadeOutAndKillEntity 0x00CBE716; the scripted-thing
//   handle (CBaseObject ctor 0x0099A380, IsValid 0x00CD23B9, Release 0x00CD2770, dtor 0x007E74D0); ~CScriptThing 0x004AA840.
// Literals: "FACTION_HERO" 0x0126C3A4, "HUD_QUEST_ICON_BODYGUARD_GOOD_01/_02" 0x012E72F4 / 0x012E72D0, "..._BAD_01/_02" 0x012E72B0 /
//   0x012E7290, "FACTION_BODYGUARD" 0x012D649C, "HenchmanWarp" 0x012E7280, "HENCHMAN_WARP" 0x012E7270, 1.0f 0x0122DED8.
// Shape (each measured against retail):
//   - GetGoldCost is an inline member defined in the TU (retail emits its COMDAT 0x00E5FC70 right after its first caller
//     OnPredicateFail 0x00E5FB80): c2 then does not know which registers it clobbers, so ParentClass stays in ebp across the call
//     (0x00E622A9); an out-of-line same-TU body puts it in edx. The call / expand split (hire path called, dismiss path after the
//     list loop expanded) is VC7.1's own per-site decision.
//   - vector<CScriptThing>::erase is called (FABLE_RETAIL_STLP_VECTOR_ERASE_OUTOFLINE), push_back is the shared COMDAT call.
//   - the health-bar cases are written in retail's layout order (2, 3, 0, 1); each string temporary keeps its own frame word.
//   - the effect position is `Me.GetPos() + C3DVector(0, 0, 1)` inside the call: GetPos runs after both string temporaries
//     (0x00E62662), X and Y are copied (the +0.0f adds fold) and only Z is added; a named GetPos reference calls it first.
//   - the level-loaded arm holds only the terminate check, so its call has no test (0x00E6254D).
#define FABLE_RETAIL_NSCRIPT
#define FABLE_RETAIL_CLEVELSCRIPTBASE
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_STLP_VECTOR
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CSCRIPTGAMERESOURCEOBJECTSCRIPTEDTHINGBASE_OUTOFLINE_DTOR
#define FABLE_RETAIL_CV_BODYGUARDSCRIPT_CBODYGUARD_MAIN
#define FABLE_RETAIL_STLP_VECTOR_ERASE_OUTOFLINE
#include "fable_string.h"
#include "engine/C3DVector.h"
#include "engine/CCountedPointer.h"
#include "engine/CScriptThing.h"
#include "stlport/fable_containers.h"
#include "engine/CQ_SunnyvaleMasterData.h"
#include "engine/CV_BodyGuardScript.h"
#include "engine/CScriptGameResourceObjectScriptedThingBase.h"
#include "engine/CGameScriptInterfaceBase.h"
#include "engine/CTimer.h"
#include "engine/CRGBColour.h"
#include "engine/CScriptDef.h"
#include "engine/CDefPointer.h"
#include "engine/fable_script_fade.h"
enum EScriptAIPriority { SCRIPT_AI_PRIORITY_LOWEST=0, SCRIPT_AI_PRIORITY_LOW=1, SCRIPT_AI_PRIORITY_MEDIUM=2, SCRIPT_AI_PRIORITY_HIGH=3, SCRIPT_AI_PRIORITY_HIGHEST=4 };
extern CGameScriptInterfaceBase* pDSTGame;   // 0x0143E8F8
extern CDefPointer<const CScriptDef> pGlobalScriptDef;   // 0x0143E90C
FABLE_FORCEINLINE bool CScriptThing::operator==(const CScriptThing& other) const { return PImp->IsEqualTo(*other.PImp); }
inline CScriptThing::CScriptThing(const CScriptThing& other) : CBaseClass(other), PImp(other.PImp) { }
FABLE_FORCEINLINE CTimer& CTimer::operator=(long time) { pDSTGame->SetTimer(TimerIndex, time); return *this; }

inline long NScript::CV_BodyGuardScript::CBodyGuard::GetGoldCost()
{
    switch (HenchManType) {
    case 0:
    case 2:
        return pGlobalScriptDef->HenchmanCheapGoldCost;
    case 1:
    case 3:
        return pGlobalScriptDef->HenchmanExpensiveGoldCost;
    }
    return 0;
}

void NScript::CV_BodyGuardScript::CBodyGuard::SetHenchmanAsHired(bool hired)
{
    if (hired) {
        if (IsActiveThreadTerminating()) return;
        BrainState = 2;
        Game->ClearThingBestEnemyTarget(Me);
        Game->SetEntityAsRegionFollowing(Game->GetHero(), Me, true);
        Game->EntityFollowThing(Me, Game->GetHero(), 1.0f, true);
        Game->EntitySetThingAsAllyOfThing(Me, Game->GetHero());
        Game->EntitySetInFaction(Me, "FACTION_HERO");
        ParentClass.HenchmanList.push_back(Me);
        ParentClass.TotalWages += GetGoldCost();
        Game->DisplayQuestInfo(true);
        switch (HenchManType) {
        case 2:
            MyHealthBarID = Game->AddQuestInfoBarHealth(Me, CRGBColour(0, 255, 0, 255), "HUD_QUEST_ICON_BODYGUARD_GOOD_01", 1.0f);
            break;
        case 3:
            MyHealthBarID = Game->AddQuestInfoBarHealth(Me, CRGBColour(0, 255, 0, 255), "HUD_QUEST_ICON_BODYGUARD_GOOD_02", 1.0f);
            break;
        case 0:
            MyHealthBarID = Game->AddQuestInfoBarHealth(Me, CRGBColour(0, 255, 0, 255), "HUD_QUEST_ICON_BODYGUARD_BAD_01", 1.0f);
            break;
        case 1:
            MyHealthBarID = Game->AddQuestInfoBarHealth(Me, CRGBColour(0, 255, 0, 255), "HUD_QUEST_ICON_BODYGUARD_BAD_02", 1.0f);
            break;
        }
    } else {
        if (IsActiveThreadTerminating()) return;
        Game->EntitySetAsAllowedToFollowHero(Me, false);
        Game->RemoveQuestInfoElement(MyHealthBarID);
        BrainState = 1;
        Game->ClearThingBestEnemyTarget(Me);
        Game->SetEntityAsRegionFollowing(Game->GetHero(), Me, false);
        Game->EntityStopFollowing(Me);
        Game->EntitySetInFaction(Me, "FACTION_BODYGUARD");
        for (CScriptThing* it = ParentClass.HenchmanList.begin(); it != ParentClass.HenchmanList.end(); ++it) {
            if (Me == *it) {
                ParentClass.HenchmanList.erase(it);
                break;
            }
        }
        ParentClass.TotalWages -= GetGoldCost();
        if (Game->IsLevelLoaded(Me.GetHomeMapName())) {
            if (IsActiveThreadTerminating()) return;
        } else {
            if (IsActiveThreadTerminating()) return;
            Game->EntitySetAsDamageable(Me, false);
            Game->EntitySetTargetable(Me, false);
            Game->SetIsPushableByHero(Me, false);
            Game->SetIsThingForcePushable(Me, false);
            CScriptGameResourceObjectScriptedThingBase seh_me;
            if (seh_me.IsValid()) seh_me.Release();
            while (!Game->StartScriptingEntity(Me, seh_me, SCRIPT_AI_PRIORITY_HIGHEST)) {
                Game->NewScriptFrame();
                if (IsActiveThreadTerminating()) return;
            }
            if (IsActiveThreadTerminating()) return;
            Game->CreateEffect("HENCHMAN_WARP", Me.GetPos() + C3DVector(0.0f, 0.0f, 1.0f), "HenchmanWarp", 0.0f, false, false);
            ParentClass.HenchmanGlobalStatus[HenchManType] = 1;
            ParentClass.HenchmanDeadRespawnTimer[HenchManType] = 0;
            FadeOutAndKillEntity(Me, 3.0f, true, true);
        }
    }
}
