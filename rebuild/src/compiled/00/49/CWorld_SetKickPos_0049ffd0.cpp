// CWorld::SetKickPos() -- record where the hero is kicked to when thrown out of the current region: the nearest thing within 30 units (2D) of the hero that is a region entrance or a teleporter gives the position, its facing and whether it is a teleporter
// Leaf: ?SetKickPos@CWorld@@QAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x0049FFD0, 258 bytes, thiscall, plain `ret`.
// Identity: Ego_r.pdb ?SetKickPos@CWorld@@QAEXXZ (public); the body calls the CWorld-TU GetPBestThing instance 0x004AC2C0 whose
//   Ego_r spelling names the filter / comparator types used here, and CTCHeroStats::SetHeroKickPositionForCurrentRegion.
// Callees: CPlayerManager::GetMainPlayer 0x00449970 (non-const, on PlayerManager +0x0C), CPlayer::GetPControlledCreature 0x00487DC0,
//   the expanded CThing::GetTC (CVectorMap LowerBound 0x0040F020, TCI_HERO_STATS 4), CThing::GetPos 0x004C73D0 (three times),
//   CThingSearchTools::GetPBestThing<CFilter_Bind2<binary_compose<logical_or<bool>, CThingFilter_HasTC, CThingFilter_HasTC>,
//   CThingFilter_In2DRange>, CThingCompare_Nearest, CThing> 0x004AC2C0 (TCI_REGION_ENTRANCE 99, TCI_TELEPORTER 221, 30.0f),
//   CTCPhysicsBase::GetFacingAngleXY (virtual, +0xFC), CTCHeroStats::SetHeroKickPositionForCurrentRegion 0x0057D990.
// Shape: the shared headers (CWorld, CThing GetTC expanded, CThingSearchTools, CFilter_Bind2, CThingFilter_HasTC,
//   CThingCompare_Nearest, STLport compose2) plus the GetPBestThing body in the TU (engine/CThingSearchTools.h
//   FABLE_RETAIL_CTHINGSEARCHTOOLS_BESTTHING_BODY, the landed 0x004AC2C0 shape): with the body visible VC7.1 knows the filter /
//   comparator temporaries are only read, which gives retail's frame slots and late constant stores (0x004A007B..0x004A008B); the
//   prior local-model draft (declaration-only search, flat filters) parked at 257v258 after 10 grades.
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CWIDESTRING
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CCLASSFACTORY
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#define FABLE_RETAIL_CTHING_STATTYPEFLAGS
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CTHINGPLAYERCREATURE
#define FABLE_RETAIL_PLAYER
#define FABLE_RETAIL_CPLAYERMANAGER
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CWORLD
#define FABLE_RETAIL_CTHINGSEARCHTOOLS
#define FABLE_RETAIL_CTHINGSEARCHTOOLS_ALLTHINGSBODY
#define FABLE_RETAIL_CTHINGSEARCHTOOLS_BESTTHING_BODY
#define FABLE_RETAIL_CTHINGFILTER_HASTC_OUTOFLINE
#define FABLE_RETAIL_STLP_BINARY_COMPOSE
#define FABLE_RETAIL_CTCHEROSTATS
#define FABLE_RETAIL_CTCHEROSTATS_KICKPOSITION
#define FABLE_RETAIL_CWORLD_SETKICKPOS
#define FABLE_RETAIL_CTCPHYSICSBASE
#include "stlport/fable_containers.h"                     // first: the overlay copy carries FABLE_RETAIL_STLP_BINARY_COMPOSE
#include "fable_string.h"
#include "engine/C3DVector.h"
#include "engine/ETCInterfaceType.h"
#include "engine/CThing.h"
#include "engine/CThingPlayerCreature.h"
#include "engine/CPlayer.h"
#include "engine/CPlayerManager.h"
#include "engine/CTCBase.h"
#include "engine/CTCHeroStats.h"
#include "engine/CTCPhysicsBase.h"
#include "engine/CThingSearchTools.h"
#include "engine/CFilter_Bind2.h"
#include "engine/CThingFilter_HasTC.h"
#include "engine/CThingCompare_Nearest.h"
#include "engine/CWorld.h"

// Ego_r.pdb CThingFilter_In2DRange {Pos, Range}; call operator ??RCThingFilter_In2DRange@@QBE_NABVCThing@@@Z out of line (0x006A76B0).
class CThingFilter_In2DRange : public std::unary_function<CThing, bool> {
public:
    CThingFilter_In2DRange(const C3DVector& pos, float range) : Pos(pos), Range(range) { }
    bool operator()(const CThing& thing) const;                 // 0x006A76B0
private:
    C3DVector Pos;
    float     Range;
};

void CWorld::SetKickPos()
{
    CThing* hero = PlayerManager->GetMainPlayer().GetPControlledCreature();
    CTCHeroStats* stats;
    if (hero->GetTC(TCI_HERO_STATS, &stats)) {
        CThing* kick = 0;
        if (GetThingSearchTools().GetPBestThing(
                GFFilter_Bind2(std::compose2(std::logical_or<bool>(), CThingFilter_HasTC(TCI_REGION_ENTRANCE), CThingFilter_HasTC(TCI_TELEPORTER)),
                               CThingFilter_In2DRange(hero->GetPos(), 30.0f)),
                CThingCompare_Nearest(hero->GetPos()), &kick))
            stats->SetHeroKickPositionForCurrentRegion(kick->GetPos(), kick->GetPhysicsTC()->GetFacingAngleXY(), kick->HasTC(TCI_TELEPORTER));
    }
}