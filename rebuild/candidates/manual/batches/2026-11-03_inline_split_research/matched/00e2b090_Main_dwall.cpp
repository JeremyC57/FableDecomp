// NScript::CV_AmbushScamScript::Main() -- the Ambush Scam village script: bind the three entity scripts, wait to enter Witchwood2 from Witchwood1, activate the good or evil variant by hero morality, run the trickster approach and the ambush, award the ending, then wait to leave the region and close the quest
// Leaf: ?Main@CV_AmbushScamScript@NScript@@UAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00E2B090, 2357 bytes, plain `ret`; slot 2 of the script vtable 0x012E2428.
// Grade: DIFFER(2183v2357) -- mixed ~CScriptThing: retail expands the dtor at the exits of the two rotated loops and the inner wait (four merged copies) and calls 0x004AA840 at the shared late exit; plain inline is all-or-nothing and the lane forbids inline_depth regions (INLINE_DEPTH_REVISIT family); the zero/vtable register choice follows
// Callees: operator new 0x00BFEA1A, CCharString (const char*, long) 0x0099EBF0 / copy 0x0099EC30 / () 0x0099E4B0 / ~ 0x0099EAE0,
//   AddEntityScriptBinding 0x00CB8230, PostAddScriptedEntities 0x00CB8930, CBaseClass() 0x0099A2D0 / ~ 0x0099A2E0,
//   IsActiveThreadTerminating 0x00CB7940, Compare 0x00411570, operator delete 0x00BFE9BC, ~CScriptThing 0x004AA840,
//   CSpawnedFuncBase(const CCharString&, unsigned long) 0x00CDD450, AddSpawnedFunction 0x00CB7E50, TricksterApproach 0x00E2EAC0,
//   SpringAmbush 0x00E2F170, AssignMoralityEtcForEvilEnding 0x00E2BB10, AssignMoralityEtcForGoodEnding 0x00E2BAC0; the entity
//   script allocators CAS_Ambusher::Alloc 0x00E2BB60, CAS_Trickster::Alloc 0x00E2BC90, CAS_Buddy::Alloc 0x00E2BEC0; the spawned
//   WatchForQuestFinished 0x00E2BA50 / WatchForRegionTransition 0x00E2B9D0; interface slots 7, 22, 64, 70, 81, 158, 276, 281,
//   558, 655; pGlobalScriptDef 0x0143E90C (AmbushScamTricksterApproachProximityLow +0x280).
#define FABLE_RETAIL_NSCRIPT
#define FABLE_RETAIL_CLEVELSCRIPTBASE
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_SUSPENDABLE_PROCESS
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_CSCRIPTTHING_IMPLICIT_ASSIGNMENT
#define FABLE_RETAIL_CENTITYSCRIPTBINDING_FORCEINLINE_CTOR
#include "stlport/fable_containers.h"
#include "engine/CV_AmbushScamScript.h"
#include "engine/CScriptThing.h"
#include "engine/CGameScriptInterfaceBase.h"
#include "engine/CEntityScriptBinding.h"
#include "engine/CSpawnedFunc.h"
#include "engine/fable_script_def_global.h"
template<class T> inline bool CBasicString<T>::operator==(const T* text) const { return Compare(PData,text)==0; }
inline CScriptThing::CScriptThing() { }
inline CScriptThing::~CScriptThing() { }
inline CCharString CScriptThing::GetDataString() const { if (!PImp.Data) return NullString; return PImp.Data->GetDataString(); }

void NScript::CV_AmbushScamScript::Main()
{
    AddEntityScriptBinding(new CEntityScriptBinding<CV_AmbushScamScript>("AS_Ambusher", this, &CAS_Ambusher::Alloc, 1));
    AddEntityScriptBinding(new CEntityScriptBinding<CV_AmbushScamScript>("AS_Trickster", this, &CAS_Trickster::Alloc, 1));
    AddEntityScriptBinding(new CEntityScriptBinding<CV_AmbushScamScript>("AS_Buddy", this, &CAS_Buddy::Alloc, 1));
    PostAddScriptedEntities();
    Game->PostAddScriptedEntities();
    CCharString region_name;
    CScriptThing entrance_point;
    Game->SetCreatureGeneratorsEnabled("Witchwood2", false);
    for (;;) {
        do { if (IsActiveThreadTerminating()) return; } while (0);
        while (!Game->MsgOnRegionLoaded(region_name)) {
            Game->NewScriptFrame();
            do { if (IsActiveThreadTerminating()) return; } while (0);
        }
        do { if (IsActiveThreadTerminating()) return; } while (0);
        if (QuestActivated && region_name == "Witchwood2") {
            do { if (IsActiveThreadTerminating()) return; } while (0);
            break;
        }
        entrance_point = Game->GetNearestWithDefName(Game->GetHero(), "REGION_ENTRANCE_POINT");
        if (region_name == "Witchwood2" && entrance_point.GetDataString() == "FromWitchwood1") {
            do { if (IsActiveThreadTerminating()) return; } while (0);
            break;
        }
        Game->NewScriptFrame();
    }
    do { if (IsActiveThreadTerminating()) return; } while (0);
    if (!QuestActivated) {
        do { if (IsActiveThreadTerminating()) return; } while (0);
        if (Game->GetHeroMoralityCategory() != 2 && Game->GetHeroMoralityCategory() != 1 && Game->GetHeroMoralityCategory() != 0) {
            do { if (IsActiveThreadTerminating()) return; } while (0);
            Game->ActivateQuest("V_AmbushScamGood");
            MoralitySwitch = true;
        } else {
            do { if (IsActiveThreadTerminating()) return; } while (0);
            Game->ActivateQuest("V_AmbushScamEvil");
            MoralitySwitch = false;
        }
        QuestActivated = true;
    }
    TricksterApproachProximity = pGlobalScriptDef->AmbushScamTricksterApproachProximityLow;
    AddSpawnedFunction(new CSpawnedFunc<CV_AmbushScamScript>("WatchForQuestFinished", 0, this, &CV_AmbushScamScript::WatchForQuestFinished), CCharString(""));
    AddSpawnedFunction(new CSpawnedFunc<CV_AmbushScamScript>("WatchForRegionTransition", 0, this, &CV_AmbushScamScript::WatchForRegionTransition), CCharString(""));
    for (;;) {
        do { if (IsActiveThreadTerminating()) return; } while (0);
        if ((TricksterApproachComplete && AmbushSpringComplete) || QuestFinished)
            break;
        if (!TricksterApproachComplete) {
            do { if (IsActiveThreadTerminating()) return; } while (0);
            TricksterApproach();
            TricksterApproachComplete = true;
        }
        if (!AmbushSpringComplete) {
            do { if (IsActiveThreadTerminating()) return; } while (0);
            SpringAmbush();
            AmbushSpringComplete = true;
        }
        Game->NewScriptFrame();
    }
    do { if (IsActiveThreadTerminating()) return; } while (0);
    do { if (IsActiveThreadTerminating()) return; } while (0);
    while (!QuestFinished && (!LeftRegion || TricksterApproachComplete)) {
        Game->NewScriptFrame();
        do { if (IsActiveThreadTerminating()) return; } while (0);
    }
    do { if (IsActiveThreadTerminating()) return; } while (0);
    if (AmbushSpringComplete) {
        do { if (IsActiveThreadTerminating()) return; } while (0);
        if (HeroAttackOnTricksterAfterAmbush || HeroAttackOnTricksterKillingBlow || TricksterKilledAfterBeingSpared) {
            do { if (IsActiveThreadTerminating()) return; } while (0);
            AssignMoralityEtcForEvilEnding();
        } else if (HeroTalkToTrickster) {
            do { if (IsActiveThreadTerminating()) return; } while (0);
            AssignMoralityEtcForGoodEnding();
        }
    }
    while (!Game->MsgOnRegionLoaded(region_name) && !LeftRegion) {
        Game->NewScriptFrame();
        do { if (IsActiveThreadTerminating()) return; } while (0);
    }
    do { if (IsActiveThreadTerminating()) return; } while (0);
    Game->SetCreatureGeneratorsEnabled("Witchwood2", true);
    if (MoralitySwitch) {
        do { if (IsActiveThreadTerminating()) return; } while (0);
        Game->DeactivateQuestLater("V_AmbushScamGood", 0);
    } else {
        do { if (IsActiveThreadTerminating()) return; } while (0);
        Game->DeactivateQuestLater("V_AmbushScamEvil", 0);
    }
    Game->DeactivateQuestLater(Game->GetActiveQuestName(), 0);
}
