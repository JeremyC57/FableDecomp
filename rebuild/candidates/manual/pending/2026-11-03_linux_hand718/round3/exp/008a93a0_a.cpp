// CGameScriptInterface::EntityWillTeleportToArea(const CScriptThing&, C3DVector, float, float) const -- for a live creature entity, gather the valid hiding positions within maxRadius of pos, drop those within minRadius of it, then try random candidates until one the creature can navigate to: start a CCreatureAction_TeleportOut there and return its SetCurrentAction result (false if none)
// Grade: DIFFER(618v620) -- register colouring only: retail keeps this in ebx and spills the thing to [esp+20h]; VC7.1 here keeps the thing in ebx and spills this (35 diff lines, all the swap; 10 attempts: targetPos=pos local at the top + a named CDistancePredicate took 548->618 bytes, the early-empty return + while(size) loop gave the GFRandom zero test; variable order / typing / nesting / a cached CWorld* moved nothing)
// Leaf: ?EntityWillTeleportToArea@CGameScriptInterface@@UBE_NABVCScriptThing@@VC3DVector@@MM@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x008A93A0, 620 bytes, thiscall, `ret 18h`; slot 541 of CGameScriptInterface.
// Identity: Ego_r.pdb ?EntityWillTeleportToArea@CGameScriptInterface@@UBE_NABVCScriptThing@@VC3DVector@@MM@Z (0x008E88C0).
// Callees: CScriptThing::IsAlive (slot 75) / GetPThing (slot 11), CWorld::GetWorldMap (slot 12), NCreatureHiding::GetValidPositions
//   0x0074A5D0 (__fastcall), std::remove_if<C3DVector*, CDistancePredicate> 0x008AC060, std::__copy<C3DVector*> 0x0074CA20,
//   CVectorMap::LowerBound 0x0040F020 (navigation TC, interface 7), CWorld::GetWorldSeed 0x0049D810, GFROR13 0x00497890,
//   CThing::GetPos 0x004C73D0, CTCCreatureNavigation::IsAbleToNavigateFromPosToPos 0x006CB620,
//   CCreatureAction_TeleportOut::CCreatureAction_TeleportOut(CThingCreatureBase&, C3DVector&, bool) 0x008BCF20,
//   CThingCreatureBase::SetCurrentAction 0x006644F0, CCombatActionBase::~CCombatActionBase 0x0062E900, free.
// Sibling model: the landed CCombatAction_StrikeResponseJackOfBladesIntoRush::GetValidRushPosition 0x008C1600.
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CRESOURCELIST
#define FABLE_RETAIL_CDEFINITIONMANAGER
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CWORLD
#define FABLE_RETAIL_NSCRIPT
#define FABLE_RETAIL_CCREATUREACTIONBASE
#include "fable_string.h"
#include "engine/CThingCreatureBase.h"
#include "engine/CWorld.h"
#include "engine/CGameScriptInterface.h"
#include "engine/CScriptThing.h"
#include "engine/CCombatActionBase.h"
#include "stlport/fable_containers.h"
#define CTCCreatureNavigation CTCCreatureNavigationDonor
#include "engine/CTCCreatureNavigation.h"
#undef CTCCreatureNavigation
class CTCCreatureNavigation : public CTCCreatureNavigationDonor {
public:
    static ETCInterfaceType __fastcall StatGetInterfaceType() { return (ETCInterfaceType)7; }
    bool IsAbleToNavigateFromPosToPos(const C3DVector& from, const C3DVector& to, float tolerance) const;   // 0x006CB620
};
#define CCreatureAction_TeleportOut CCreatureAction_TeleportOutDonor
#include "engine/CCreatureAction_TeleportOut.h"
#undef CCreatureAction_TeleportOut
class CCreatureAction_TeleportOut : public CCombatActionBase {
public:
    CCreatureAction_TeleportOut(CThingCreatureBase& creature, C3DVector& pos, bool instant);   // 0x008BCF20
private:
    C3DVector TargetPos;                                            // +0x114
    bool      Instant;                                              // +0x120
};
class CWorldMap; class CTCShapeManager;
namespace NCreatureHiding {
void __fastcall GetValidPositions(std::vector<C3DVector>& positions, const CWorldMap& map, const CThingSearchTools& search,
                                  const C3DVector& pos, float radius, bool flag, const CTCShapeManager* shapes);   // 0x0074A5D0
}
class CDistancePredicate {
public:
    float     Distance;
    C3DVector Pos;
    CDistancePredicate(const C3DVector& pos, float distance) : Distance(distance), Pos(pos) { }
};
namespace std {
template <class _ForwardIter, class _Predicate> _ForwardIter __fastcall remove_if(_ForwardIter, _ForwardIter, _Predicate);
}
long __fastcall GFROR13(unsigned long value);                       // 0x00497890
inline unsigned long GFRandom(unsigned long range, unsigned long* seed)
{
    *seed = *seed * 0x24a1 + 0x24df;
    *seed = GFROR13(*seed);
    return range ? *seed % range : 0;
}

bool CGameScriptInterface::EntityWillTeleportToArea(const CScriptThing& entity, C3DVector pos, float minRadius, float maxRadius) const
{
    CThing* thing;
    if (!entity.IsAlive())
        return false;
    thing = entity.GetPThing();
    if (!(thing->GetTypeFlags() & 0x10))
        return false;
    C3DVector targetPos = pos;
    std::vector<C3DVector> positions;
    NCreatureHiding::GetValidPositions(positions, World->GetWorldMap(), World->GetThingSearchTools(), pos, maxRadius, false, 0);
    CDistancePredicate predicate(pos, minRadius);
    positions.erase(std::remove_if(positions.begin(), positions.end(), predicate), positions.end());
    if (positions.size() == 0)
        return false;
    CTCCreatureNavigation* navigation;
    thing->GetTC(&navigation);
    while (positions.size() != 0) {
        unsigned long index = GFRandom(positions.size(), World->GetWorldSeed());
        if (navigation->IsAbleToNavigateFromPosToPos(thing->GetPos(), positions[index], 0.0f)) {
            targetPos = positions[index];
            CThingCreatureBase* creature = static_cast<CThingCreatureBase*>(thing);
            return creature->SetCurrentAction(CCreatureAction_TeleportOut(*creature, targetPos, false));
        }
        positions.erase(positions.begin() + index);
    }
    return false;
}
