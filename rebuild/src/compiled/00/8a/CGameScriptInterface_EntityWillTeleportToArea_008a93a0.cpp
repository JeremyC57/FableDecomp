// 008a93a0 CGameScriptInterface::EntityWillTeleportToArea
// Leaf: ?EntityWillTeleportToArea@CGameScriptInterface@@UBE_NABVCScriptThing@@VC3DVector@@MM@Z
// Flags: none
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CRESOURCELIST
#define FABLE_RETAIL_CDEFINITIONMANAGER
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CTHINGAICREATURE
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_CWORLD
#define FABLE_RETAIL_NSCRIPT
#define FABLE_RETAIL_CCREATUREACTIONBASE
#define FABLE_RETAIL_CCREATUREACTION_TELEPORT
#define FABLE_RETAIL_CTCCREATURENAVIGATION
#define FABLE_RETAIL_CTCCREATURENAVIGATION_NAVIGABLE
#define FABLE_RETAIL_STLP_REMOVE_COPY_IF
#define FABLE_RETAIL_STLP_NOT1
#include "fable_string.h"
#include "engine/CThingAICreature.h"
#include "engine/CGameScriptInterface.h"
#include "engine/CScriptThing.h"
#include "engine/CWorld.h"
#include "engine/CTCCreatureNavigation.h"
#include "engine/CCreatureAction_TeleportOut.h"
#include "engine/NCreatureHiding.h"
#include "engine/CVectorFilter_FurtherFromPointThan.h"
#include "engine/GFRandom.h"
#include "stlport/position_algorithms.h"

bool CGameScriptInterface::EntityWillTeleportToArea(const CScriptThing& entity, C3DVector centre_pos,
                                                   float min_distance, float max_distance) const
{
    if (entity.IsAlive()) {
        CThing* pthing = entity.GetPThing();
        if (pthing->GetTypeFlags() & 0x10) {
            CThingAICreature* pcreature = static_cast<CThingAICreature*>(pthing);
            C3DVector teleport_pos = centre_pos;
            std::vector<C3DVector> validPositions;
            NCreatureHiding::GetValidPositions(validPositions, World->GetWorldMap(), World->PeekThingSearchTools(),
                                               centre_pos, max_distance, false, 0);
            validPositions.erase(std::remove_if(validPositions.begin(), validPositions.end(),
                std::not1(CVectorFilter_FurtherFromPointThan(centre_pos, min_distance))), validPositions.end());
            long num_positions = validPositions.size();
            if (num_positions == 0)
                return false;
            else {
                long n; // The debug scope records this local; the debug procedure never reads or writes it.
                bool found = false;
                CTCCreatureNavigation* ptc_nav;
                pcreature->GetTC(&ptc_nav);
                do {
                    long random_index = GFRandom(validPositions.size(), World->GetWorldSeed());
                    if (ptc_nav->IsAbleToNavigateFromPosToPos(pcreature->GetPos(), validPositions[random_index], 0.0f)) {
                        teleport_pos = validPositions[random_index];
                        found = true;
                        break;
                    }
                    validPositions.erase(validPositions.begin() + random_index);
                } while (validPositions.size() > 0);
                if (!found) return false;
                return pcreature->SetCurrentAction(CCreatureAction_TeleportOut(*pcreature, teleport_pos, false));
            }
        }
    }
    return false;
}