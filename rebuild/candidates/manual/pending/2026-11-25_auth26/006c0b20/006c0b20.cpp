// CTCBuyableHouse::FindPosToCreateNewVillager(C3DVector& ret_pos) -- find where a new villager for the house can appear unseen: the
// first region entrance (TCI_REGION_ENTRANCE) in the house's map the hero is not aware of, else the house itself when no hero is
// within 20 (2D) and the hero is not aware of it
// Leaf: ?FindPosToCreateNewVillager@CTCBuyableHouse@@AAE_NAAVC3DVector@@@Z
// Retail: Fable.exe 0x006C0B20, 368 bytes, `ret 4`, esp frame (sub esp,20h). Private (Ego_r.pdb + FableWin.pdb publics, AAE); Ego_d.pdb
//   locals ret_pos, phero (CThingPlayerCreature*), entrances (std::vector<CThing*>), inner block end_it / it; Ego_d lines 937..958.
// Statements: FableWin.pdb's /Od body (0x024B1A30, 569 bytes = Ego_d's code size), whose line map fits Ego_d's 937..958 exactly:
//   entrances; one GetAllThings statement; phero; for (it = begin(), end_it = end(); ...); the nested house test (949 / 951 / 953 /
//   954), return false (957).
// Callees: CTCBase::GetWorldMap 0x00686D30 (twice), CThing::GetPos 0x004C73D0, CWorldMap::GetMapNumberAt(const C3DVector&) (vtable
//   slot 16), CTCBase::GetWorld 0x00686A70 (+ the inline CWorld::PeekThingSearchTools, +0x20),
//   CThingSearchTools::GetAllThings<CThing, vector<CThing*>, CFilter_Bind2<CThingFilter_HasTC, CThingFilter_InMap> > 0x006C1DD0 (the
//   same-TU noinline body: its copy here is byte- and relocation-equal to retail 0x006C1DD0), CTCBase::GetPlayerManager 0x00686D40,
//   CPlayerManager::GetMainPlayer 0x00449970, CPlayer::GetPControlledCreature 0x00487DC0, NCreatureAI::IsAwareOfPosition 0x00864620
//   (__fastcall), free 0x00BFEA14 (the vector's inline destructor). The inline GetParentThing, GFFilter_Bind2, the filter
//   constructors and GFWorld2DDistanceSmallerThan (FableWin out of line; retail folds GFSqr(20) to 400.0f).
// Flags: the default lane flags; no pragmas. Opt-ins: FABLE_RETAIL_CTCBUYABLEHOUSE_FINDPOSTOCREATENEWVILLAGER (new, overlay header,
//   declaration only); the GetAllThings body opt-ins ALLTHINGSBODY + _EGOD, CTHING_STATTYPEFLAGS, CTHING_SCANINCLUSION_ISALIVE and
//   CTHINGFILTER_HASTC_OUTOFLINE make the pick-any copy equal retail 0x006C1DD0 (scan test as IsAlive(), folded CThing type test,
//   filter operators out of line, pushed pointer stored only on the push path).
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
                ret_pos = (*it)->GetPos();
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
