// CThingCreatureBase::IsThingInLOSAndFOV(const CThing& thing) const -- a live thing inside this creature's field of view with a clear line of sight to it
// Leaf: ?IsThingInLOSAndFOV@CThingCreatureBase@@QBE_NABVCThing@@@Z
// Retail: Fable.exe @ 0x00666EA0, 66 bytes, thiscall `ret 4`, bool in al.
// Identity (probable, donor order + callees): Ego_r.pdb orders CThingCreatureBase ... GetCombatTypeDef (0x00666D10 here,
// landed), GetHeightZBelowAt, IsThingInLOSAndFOV(const CThing&) const; this body is a bool predicate on one CThing reference
// that calls IsThingInFOV(thing, true) 0x00665190 (landed) and then the world map's IsLineOfSight(*this, thing, true) --
// exactly LOS and FOV. Its only reference is the tail jump of CThingFilter_IsAvailableForCreatureInteraction::operator()
// 0x006A7430 (at 0x006A74B2).
// Callees: CThingCreatureBase::IsThingInFOV(const CThing&, bool) const 0x00665190, CThing::PeekWorldMap() const 0x004C79E0,
// CWorldMap::IsLineOfSight(const CThing&, const CThing&, bool) const 0x0063E6F0 (Ego_r.pdb orders CWorldMap
// GetImpassabilityFactorAround (0x0063E3C0), GetNavigationLayerAt, GetHeightZBelowPos (0x0063E480), IsLineOfSight(const CThing&,
// const CThing&, bool), GetHeightZBelowAt (0x0063E950); the 0x0063E6F0 body sits in that slot).
// Shape: the dead thing rejected first (bit 0 of +0x91); the line-of-sight arguments pushed before the world-map fetch;
// three guard returns (`if (!IsThingInLOS(thing)) return false; return true;` is the `test al,al ; setne al` tail; `return a && b`,
// `return IsThingInLOS(thing)` and the ternaries all lay the blocks out differently).
// Layout: engine/CThingCreatureBase.h through a donor include (flattened: no CThing base, so the creature is reinterpreted
// as its CThing view for the CThing-level calls); CThing is a local partial view (no engine header), Dead +0x91 bit 0.
#define CThingCreatureBase CThingCreatureBaseDonor
#include "engine/CThingCreatureBase.h"
#undef CThingCreatureBase

class CWorldMap;
class CThing {                                              // partial view, retail offsets
public:
    unsigned char _pad00[0x91];
    unsigned char Dead : 1;                                 // +0x91 bit 0
    unsigned char CreatedByOwnerThingCreation : 1;
    unsigned char IncludeInThingScans : 1;
    unsigned char Activated : 1;
    unsigned char GloballyLoaded : 1;
    unsigned char InLimbo : 1;
    unsigned char KillLocked : 1;
    unsigned char CreatedBySerialisation : 1;
    bool IsDead() const { return Dead; }
    const CWorldMap& PeekWorldMap() const;                  // 0x004C79E0
};
class CWorldMap {
public:
    bool IsLineOfSight(const CThing& from, const CThing& to, bool flag) const;   // 0x0063E6F0
};

class CThingCreatureBase : public CThingCreatureBaseDonor {
public:
    const CThing& AsThing() const { return *reinterpret_cast<const CThing*>(this); }
    bool IsThingInFOV(const CThing& thing, bool flag) const;   // 0x00665190
    bool IsThingInLOS(const CThing& thing) const { return AsThing().PeekWorldMap().IsLineOfSight(AsThing(), thing, true); }
    bool IsThingInLOSAndFOV(const CThing& thing) const;
};

bool CThingCreatureBase::IsThingInLOSAndFOV(const CThing& thing) const
{
    if (thing.IsDead())
        return false;
    if (!IsThingInFOV(thing, true))
        return false;
    if (!IsThingInLOS(thing))
        return false;
    return true;
}