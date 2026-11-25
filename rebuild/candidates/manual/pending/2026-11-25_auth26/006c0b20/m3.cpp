// g1: g0 + the GetAllThings body opt-ins for the pick-any copy (ISALIVE scan test, EGOD body, CThing StatGetTypeFlags)
// Leaf: ?FindPosToCreateNewVillager@CTCBuyableHouse@@AAE_NAAVC3DVector@@@Z
#include "fable_compiler.h"
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CCOUNTEDPOINTER
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CTHINGPLAYERCREATURE
#define FABLE_RETAIL_CWORLD
#define FABLE_RETAIL_SCOPED_PTR
#define FABLE_RETAIL_CWORLDMAP
#define FABLE_RETAIL_PLAYER
#define FABLE_RETAIL_CPLAYERMANAGER
#define FABLE_RETAIL_CTHINGSEARCHTOOLS
#define FABLE_RETAIL_CTHINGSEARCHTOOLS_ALLTHINGSBODY
#define FABLE_RETAIL_CTHINGSEARCHTOOLS_ALLTHINGSBODY_EGOD
#define FABLE_RETAIL_CTHING_STATTYPEFLAGS
#define FABLE_RETAIL_CTHING_SCANINCLUSION_ISALIVE
#define FABLE_RETAIL_CTHINGFILTER_HASTC_OUTOFLINE
#define FABLE_RETAIL_CTCBUYABLEHOUSE
#define FABLE_RETAIL_CTCBUYABLEHOUSE_FINDPOSTOCREATENEWVILLAGER
#include "fable_string.h"
#include "engine/ETCInterfaceType.h"
#include "engine/C3DVector.h"
#include "engine/CThing.h"
#include "engine/CTCBase.h"
#include "engine/CThingCreatureBase.h"
#include "engine/CThingPlayerCreature.h"
#include "engine/CWorld.h"
#include "engine/CWorldMap.h"
#include "engine/CPlayer.h"
#include "engine/CPlayerManager.h"
#include "engine/CThingSearchTools.h"
#include "engine/CThingFilter_HasTC.h"
#include "engine/CThingFilter_InMap.h"
#include "engine/CFilter_Bind2.h"
#include "engine/GFWorldDistance.h"
#include "engine/CTCBuyableHouse.h"

namespace NCreatureAI {
bool __fastcall IsAwareOfPosition(const CThingCreatureBase* creature, const C3DVector& pos);   // 0x00864620
}

bool CTCBuyableHouse::FindPosToCreateNewVillager(C3DVector& ret_pos)
{
    std::vector<CThing*> entrances;
    GetWorld().PeekThingSearchTools().GetAllThings<CThing>(GFFilter_Bind2(CThingFilter_HasTC(TCI_REGION_ENTRANCE), CThingFilter_InMap(GetWorldMap(), GetWorldMap().GetMapNumberAt(GetParentThing().GetPos()))), entrances);
    CThingPlayerCreature* phero = GetPlayerManager().GetMainPlayer().GetPControlledCreature();
    {
        for (std::vector<CThing*>::iterator it = entrances.begin(), end_it = entrances.end(); it != end_it; ++it)
        {
            if (!NCreatureAI::IsAwareOfPosition(phero, (*it)->GetPos()))
            {
                ret_pos = (*it)->GetPos(); if (it + 1 != end_it) continue;
                return true;
            }
        }
    }
    if (!phero || !GFWorld2DDistanceSmallerThan(phero->GetPos(), GetParentThing().GetPos(), 20.0f, false))
    {
        if (!NCreatureAI::IsAwareOfPosition(phero, GetParentThing().GetPos()))
        {
            ret_pos = GetParentThing().GetPos();
            return true;
        }
    }
    return false;
}
