// CTCCreatureGenerator::InitialiseCreatureGenerator(bool) -- set up the generator once its world is loaded: unless restoring,
// find the exclusion area (the shape manager of the first BAD_CREATURE_GENERATION_AREA marker), gather the open and hidden
// spawn positions round the generator, run the pending nav check against the hero and fire a trigger-on-activate generator
// Leaf: ?InitialiseCreatureGenerator@CTCCreatureGenerator@@QAEX_N@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x0074B770, 395 bytes, `ret 4`. Ego_r.pdb / FableWin.pdb ?InitialiseCreatureGenerator@CTCCreatureGenerator@@QAEX_N@Z.
// Callees: CCharString(const char*, long) 0x0099EBF0 / ~CCharString 0x0099EAE0; CMainGameComponent::PeekDefinitionManager 0x00415FBC on
//   CMainGameComponent::PMainGameComponent 0x013B86A0; CDefinitionManager::GetDefGlobalIndexFromName 0x009AD410;
//   CTCBase::GetThingSearchTools 0x00686D80 / PeekThingSearchTools 0x00686D90 / PeekWorldMap 0x00686D20 / GetWorld 0x00686A70;
//   CThingSearchTools::GetAllThings<CThingMarker, vector<CThingMarker*>, CThingFilter_HasDefGlobalIndex> 0x0074D940;
//   CVectorMap LowerBound 0x004365B0 (inside the forced-inline CThing::GetTC<CTCShapeManager>); free 0x00BFEA14 (the STLport vector);
//   CThing::GetPos 0x004C73D0; NCreatureHiding::GetValidPositions 0x0074A5D0 (__fastcall); CWorld::PeekMyPlayer 0x0049D9D0;
//   CBaseIntelligentPointer::GetPItem 0x00A01B50 (inline CPlayer::GetPPlayerCharacter); DoNavCheck 0x0074A4E0; CreateCreatures 0x0074B2B0.
// Global: the bool at 0x013824AC (initialised true; its only reference is this function; no data-symbol name source: descriptive).
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTCCREATUREGENERATOR
#define FABLE_RETAIL_CTCSHAPEMANAGER
#define FABLE_RETAIL_CTHINGSEARCHTOOLS
#define FABLE_RETAIL_CWORLD
#define FABLE_RETAIL_PLAYER
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CENGINEPERSISTENTPRIMITIVEBASE
#define FABLE_RETAIL_CMAINGAMECOMPONENT
#define FABLE_RETAIL_CRESOURCELIST
#define FABLE_RETAIL_CDEFINITIONMANAGER
#include "fable_string.h"
#include "engine/CThing.h"
#include "engine/CTCCreatureGenerator.h"
#include "engine/CTCShapeManager.h"
#include "engine/CThingMarker.h"
#include "engine/CThingSearchTools.h"
#include "engine/CThingFilter_HasDefGlobalIndex.h"
#include "engine/CWorld.h"
#include "engine/CPlayer.h"
#include "engine/CThingPlayerCreature.h"
#include "engine/CMainGameComponent.h"
#include "engine/CGameDefinitionManager.h"

class CWorldMap;
namespace NCreatureHiding {
void __fastcall GetValidPositions(std::vector<C3DVector>&, const CWorldMap&, const CThingSearchTools&, const C3DVector&, float, bool,
    const CTCShapeManager*);   // 0x0074A5D0
}

extern bool g_CreatureGeneratorNavCheckEnabled;   // 0x013824AC (descriptive)

void CTCCreatureGenerator::InitialiseCreatureGenerator(bool restoring)
{
    if (!restoring) {
        PExclusionArea = 0;
        long marker_index = CMainGameComponent::Get()->PeekDefinitionManager().GetDefGlobalIndexFromName("MARKER_SET_BAD_CREATURE_GENERATION_AREA");
        std::vector<CThingMarker*> markers;
        GetThingSearchTools().GetAllThings<CThingMarker>(CThingFilter_HasDefGlobalIndex(marker_index), markers);
        if (!markers.empty()) {
            CThingMarker* marker = markers[0];
            if (marker && !marker->IsDead())
                marker->GetTC(&PExclusionArea);
        }
    }
    NCreatureHiding::GetValidPositions(OpenPositions, PeekWorldMap(), PeekThingSearchTools(), GetParentThing().GetPos(),
        GenParams.GenerationRadius, false, PExclusionArea);
    NCreatureHiding::GetValidPositions(HiddenPositions, PeekWorldMap(), PeekThingSearchTools(), GetParentThing().GetPos(),
        GenParams.GenerationRadius, true, PExclusionArea);
    CThingPlayerCreature* hero = GetWorld().PeekMyPlayer()->GetPPlayerCharacter();
    if (hero && !hero->IsDead() && g_CreatureGeneratorNavCheckEnabled && NeedToDoNavCheck) {
        DoNavCheck(*hero);
        NeedToDoNavCheck = false;
    }
    if (GenParams.TriggerOnActivate)
        CreateCreatures(0);
    Initialised = true;
}