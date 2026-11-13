// CAIStateGroup_JackOfBladesStage1::OnActivate() -- register Jack as a combatant against his target, switch his hit locations to the
// plain one (BIG and FORCEFIELD off), kill the live spot-light markers within 50 units, find the shield object within 50 units and keep
// it, copy the arena centre's position into the boss component's phase start position, and reset the speech counter.
// Retail: Fable.exe (TLC, VC7.1) @ 0x00972990, 803 bytes (`ret`); vtable 0x0128D6B4 slot 2. Ego_r.pdb / FableWin.pdb
// ?OnActivate@CAIStateGroup_JackOfBladesStage1@@UAEXXZ.
// Callees: CVectorMap::LowerBound 0x0040F020 (inline GetTC 0x7f / 0xad), CAIStateGroupBase::GetWorld 0x008FCFC0, CBaseIntelligentPointer
// GetPItem 0x00A01B50 / ctor 0x00A01B10 / SetPItem 0x00A01B90 / dtor 0x00A01C10, CCombatManager::RegisterCombatant 0x006EDC50,
// CCharString(const char*, long) 0x0099EBF0 / ~CCharString 0x0099EAE0, CTCHitLocations::GetPHitLocationWithDefName 0x00815820,
// NHitLocations::CHitLocation::SetEnabled 0x00816390, CGameDefinitionManager::Get 0x0044C6B0, CDefinitionManager::GetDefGlobalIndexFromName
// 0x009AD410, CThing::GetPos 0x004C73D0, __ftol2 0x00BFEA70, CAIStateGroupBase::PeekThingSearchTools 0x008FD000,
// CThingSearchTools::GetAllThingsInRoughArea<CThing,vector<CThing*>,CThingFilter_HasDefGlobalIndex> 0x0096BCB0 (declaration only),
// CThingSearchTools::GetPFirstThingInRoughArea<CThingFilter_HasDefGlobalIndex,CThing> 0x007FCEF0 (its REAL body visible in the TU as a
// noinline template, overlay opt-in FABLE_RETAIL_CTHINGSEARCHTOOLS_FIRSTROUGH_BODY), CThing::Kill 0x004C9B80, free 0x00BFEA14 (vector).
// Shape: the first search keeps a named def-index filter (index local first, so the filter store follows the string dtor) and a
// C2DBoxI temporary; the light loop hoists the count with an `if (count > 0)` guard (retail's double `cmp esi,ebx ; jbe`); the shield
// search is one call over temporaries: C2DBoxI(GetPos().ToC2DCoordI(), 50) and CThingFilter_HasDefGlobalIndex(index). With the
// GetPFirstThingInRoughArea body visible VC7.1 knows the out pointer's target and the filter do not escape: the filter temporary
// shares the string temporaries' frame word and the shield is re-read after the intelligent pointer's base ctor, as in retail.
// SetPhaseStartPos is FableWin.pdb's inline setter (3-dword copy at +0x22, packed C3DVector).
// Leaf: ?OnActivate@CAIStateGroup_JackOfBladesStage1@@UAEXXZ
#define FABLE_RETAIL_JACK_STAGE1_ONACTIVATE
#define FABLE_RETAIL_CAISTATEGROUPBASE
#define FABLE_RETAIL_CAISTATEGROUPBASE_MEMBERS
#define FABLE_RETAIL_CAISTATEGROUPBASE_GOTOBED_WORLD
#define FABLE_RETAIL_CAISTATEGROUPBASE_GETCREATURE
#define FABLE_RETAIL_CAISTATEGROUPBASE_PEEK
#define FABLE_RETAIL_CAIBRAIN_GETCREATURE
#define FABLE_RETAIL_CAISTATEGROUP_JACKOFBLADESBASE
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CINTELLIGENTPOINTER_GET_ORIGINAL_CHAIN
#define FABLE_RETAIL_CINTELLIGENTPOINTER_COMPARE_NULL_INT
#define FABLE_RETAIL_CINTELLIGENTPOINTER_DEREFERENCE
#define FABLE_RETAIL_CINTELLIGENTPOINTER_ASSIGN
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CTHINGAICREATURE
#define FABLE_RETAIL_CWORLD
#define FABLE_RETAIL_CWORLD_COMBATMANAGER
#define FABLE_RETAIL_CCOMBATMANAGER
#define FABLE_RETAIL_IS_THING_ALIVE
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_C3DVECTOR_PACKED
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CTCHITLOCATIONS_WITHDEFNAME
#define FABLE_RETAIL_CHITLOCATION_SETENABLED
#define FABLE_RETAIL_CTCBOSS_SETPHASESTARTPOS
#define FABLE_RETAIL_CTHINGSEARCHTOOLS
#define FABLE_RETAIL_CTHINGSEARCHTOOLS_FIRSTROUGH
#define FABLE_RETAIL_CTHINGSEARCHTOOLS_FIRSTROUGH_BODY
#define FABLE_RETAIL_CTHINGSEARCHTOOLS_BESTROUGH_BODY
#define FABLE_RETAIL_CTHINGSEARCHTOOLS_BESTROUGH
#define FABLE_RETAIL_CTHING_TYPEFLAGS
#define FABLE_RETAIL_CTHING_STATTYPEFLAGS
#define FABLE_RETAIL_CTCMAPWHO
#define FABLE_RETAIL_CTCMAPWHO_SEARCHID
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CDEFINITIONMANAGER
#define FABLE_RETAIL_CRESOURCELIST
#define FABLE_RETAIL_CGAMEDEFINITIONMANAGER
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_C2DBOXI
#define FABLE_RETAIL_C2DBOXI_CTORS
#include "engine/CThingCreatureBase.h"
#include "engine/CThingAICreature.h"
#include "engine/CAIStateGroup_JackOfBladesStage1.h"
#include "engine/CAIBrain.h"
#include "engine/CWorld.h"
#include "engine/CCombatManager.h"
#include "engine/ThingAlive.h"
#include "engine/C3DVector.h"
#include "engine/C2DBoxI.h"
#include "engine/CTCHitLocations.h"
#include "engine/NHitLocations_CHitLocation.h"
#include "engine/CTCBoss.h"
#include "engine/CTCMapwho.h"
#include "engine/NThingMap_CThingMap.h"
#include "engine/CThingSearchTools.h"
#include "engine/CThingFilter_HasDefGlobalIndex.h"
#include "engine/CGameDefinitionManager.h"

void CAIStateGroup_JackOfBladesStage1::OnActivate()
{
    CThingAICreature& creature = GetCreature();
    CTCHitLocations* hits;
    creature.GetTC((ETCInterfaceType)0x7f, &hits);
    GetWorld().GetCombatManager().RegisterCombatant(creature, *PTargetThing, true);
    NHitLocations::CHitLocation* location;
    location = hits->GetPHitLocationWithDefName("HIT_LOCATION_JACK_OF_BLADES_BIG");
    location->SetEnabled(false);
    location = hits->GetPHitLocationWithDefName("HIT_LOCATION_JACK_OF_BLADES_FORCEFIELD");
    location->SetEnabled(false);
    location = hits->GetPHitLocationWithDefName("HIT_LOCATION_JACK_OF_BLADES");
    location->SetEnabled(true);
    std::vector<CThing*> lights;
    {
        long index = CGameDefinitionManager::Get().GetDefGlobalIndexFromName("MARKER_SPOT_LIGHT");
        CThingFilter_HasDefGlobalIndex filter(index);
        const C3DVector& pos = GetCreature().GetPos();
        long x = (long)pos.X;
        long y = (long)pos.Y;
        PeekThingSearchTools().GetAllThingsInRoughArea<CThing>(C2DBoxI(x - 50, y - 50, x + 50, y + 50), filter, lights);
    }
    unsigned long count = lights.size();
    if (count > 0)
    for (unsigned long i = 0; i < count; ++i) {
        CThing* light = lights[i];
        if (light != 0 && !light->IsDead())
            light->Kill(true);
    }
    {
        CThing* shield = 0;
        long index = CGameDefinitionManager::Get().GetDefGlobalIndexFromName("OBJECT_JACK_OF_BLADES_SHIELD");
        if (PeekThingSearchTools().GetPFirstThingInRoughArea(C2DBoxI(GetCreature().GetPos().ToC2DCoordI(), 50), CThingFilter_HasDefGlobalIndex(index), &shield))
            PShieldThing = CIntelligentPointer<CThing>(shield);
    }
    if (IS_THING_ALIVE(PArenaCentre)) {
        CTCBoss* boss;
        GetCreature().GetTC((ETCInterfaceType)0xad, &boss);
        boss->SetPhaseStartPos(PArenaCentre->GetPos());
    }
    SpeechCounter = 0;
}