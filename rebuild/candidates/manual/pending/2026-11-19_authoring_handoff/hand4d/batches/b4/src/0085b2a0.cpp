// CThingSearchTools::GetAllThingsInRoughArea<CThingGameObject, std::list<CThingGameObject*>, CFilter_Bind4<CThingFilter_IsNot, CThingFilter_CombatIsStrikeToStrike, CThingFilter_CombatOverlappingSphere, CThingFilter_CombatCanCollideOrInteractWithThing> >(const C2DBoxI&, const CFilter_Bind4<...>&, std::list<CThingGameObject*>&) const -- appends every game object in the area's map cells (each once per search) that is not the striker, is strike-to-strike with it, whose hit-location bounding sphere overlaps the strike sphere and that it can collide or interact with
// Grade: RELOCATION_MATCH (hand4d 2026-11-19, third grade of the re-port). The parked draft (a local model under the propagated catalog
//   label GetPBestThingInRoughArea<CFilter_Bind3<CThingFilter_IsAICreatureAwareOf,..>>, DIFFER 775v775) is replaced by the shared
//   CThingSearchTools ROUGHAREA_EGOD GetAllThingsInRoughArea body (as the landed list sibling 0x00878430) with the PDB filter classes.
// Leaf: ??$GetAllThingsInRoughArea@VCThingGameObject@@V?$list@PAVCThingGameObject@@V?$allocator@PAVCThingGameObject@@@std@@@std@@V?$CFilter_Bind4@VCThingFilter_IsNot@@VCThingFilter_CombatIsStrikeToStrike@?A0x
// Retail: Fable.exe (TLC, VC7.1) @ 0x0085B2A0, 775 bytes, thiscall `ret 0xC`; one caller 0x008593FF. IDENTIFY: the catalog name was
//   propagated from a byte-similar body and is wrong. Name from the callees and two donors: Ego_r.pdb 0x0083B010 and FableWin.pdb
//   0x0282DAA0 ??$GetAllThingsInRoughArea@VCThingGameObject@@V?$list@PAVCThingGameObject@@..@std@@V?$CFilter_Bind4@VCThingFilter_IsNot@@
//   VCThingFilter_CombatIsStrikeToStrike@?A0xad05d24c@@VCThingFilter_CombatOverlappingSphere@@VCThingFilter_CombatCanCollideOrInteractWithThing@3@@@@
//   CThingSearchTools@@QBEXABVC2DBoxI@@ABV?$CFilter_Bind4@..@@AAV?$list@..@@@Z (the same TU's GetAllThings twin is landed at 0x0085B5B0,
//   the out-of-line Bind4 operator at 0x0085B160). The two anonymous-namespace filters mangle with this TU's own ?A0x hash (retail
//   ?A0xad05d24c), so the Leaf is a prefix.
// Callees: GetAllThingMapsInArea 0x0049C6F0, ClipAndConvertWorldMapAreaToLocalArea 0x0049C700, CThingFilter_IsNot::operator() 0x006A76A0,
//   CThingFilter_CombatIsStrikeToStrike::operator() 0x008565E0, the const interface-map LowerBound 0x004365B0 (inline PeekTC<const
//   CTCHitLocations>), CTCHitLocations::GetBoundingSphere 0x00815420, CThingFilter_CombatCanCollideOrInteractWithThing::operator()
//   0x00857250, operator new 0x00BFEA0E (the 12-byte list node), memmove 0x00BFEAE6 (the map list's clear).
// Classes: Ego_d.pdb records. `anonymous namespace'::CThingFilter_CombatIsStrikeToStrike (0x5145F, sizeof 4): unary_function<CThing,bool>
//   base, ThingStriking (const CThing&) +0; ctor (thing_striking). `anonymous namespace'::CThingFilter_CombatCanCollideOrInteractWithThing
//   (0x51457, sizeof 8): TCPhysics (const CTCPhysicsBase&) +0, ExcludeCreatures +4, ExcludeDeadCreatures +5; ctor (tc_physics,
//   exclude_dead_creatures, exclude_creatures). CThingFilter_CombatOverlappingSphere (0x2601E, sizeof 16, not anonymous): SpherePos +0,
//   Radius +0xC, ctor (pos, radius); operator() locals test_thing, ptc_hit_locations, { centre_pos, radius } in FableWin.pdb's statement
//   order (0x0282F400: PeekTC, GetBoundingSphere(.., true), (centre_pos - SpherePos).GetSquaredMagnitude() < GFSqr(radius + Radius)).
// Inline-budget: retail expands both the Bind4 call operator and the sphere test in place (0x0085B406..0x0085B4C2; Ego_r.pdb has no
//   out-of-line ??RCThingFilter_CombatOverlappingSphere). VC7.1's budget here leaves the sphere test out of line with both plain inline
//   (622v775), and the Bind4 operator out of line when only the sphere test is forced (581v775); both FABLE_FORCEINLINE match (the landed
//   twin 0x0085B5B0 forces the same pair).
// Inferred: the forced pair is a budget lever, not the source's spelling: retail also holds an uncalled out-of-line copy of this Bind4
//   instance (0x0085B160), so the original operator was plain inline and VC7.1's per-function budget differed (the retail source revision's
//   callee sizes are not recoverable); the forced pair is the smallest change that reproduces retail's expansion.
// Overlay: engine/CFilter_Bind4.h FABLE_RETAIL_CFILTER_BIND4_FORCEINLINE (new: the call operator FABLE_FORCEINLINE).
#define FABLE_RETAIL_CTHINGSEARCHTOOLS
#define FABLE_RETAIL_CFILTER_BIND4_FORCEINLINE
#define FABLE_RETAIL_CTHINGSEARCHTOOLS_CLIPAREA
#define FABLE_RETAIL_CTHINGSEARCHTOOLS_ORIGINAL_ROUGHAREA
#define FABLE_RETAIL_CTHINGSEARCHTOOLS_ROUGHAREA_EGOD
#define FABLE_RETAIL_CTHINGMAP_PEEKCELL_INLINE
#define FABLE_RETAIL_CTCMAPWHO
#define FABLE_RETAIL_CTCMAPWHO_SEARCHID
#define FABLE_RETAIL_CTHINGGAMEOBJECT_STATTYPEFLAGS
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_C3DVECTOR_NORMALISE_SQUARE_HELPER
#define FABLE_RETAIL_HITLOCATIONS_BOUND
#define FABLE_RETAIL_C2DBOXI
#define FABLE_RETAIL_C2DBOXI_COPY
#define FABLE_RETAIL_C2DBOXI_GETWIDTH
#define FABLE_RETAIL_C2DBOXI_GETHEIGHT
#include "engine/C2DExtentsI.h"
#include "engine/C3DVector.h"
#include "engine/CThing.h"
class CTCMapwho;
#include "engine/CTCMapwho.h"
#include "engine/NThingMap_CThingMap.h"
#include "engine/CThingGameObject.h"
#include "engine/CThingSearchTools.h"
#include "engine/C2DBoxI.h"
#include "engine/CTCHitLocations.h"
#include "engine/CThingFilter_IsNot.h"
#include "engine/CFilter_Bind4.h"

namespace {
class CThingFilter_CombatIsStrikeToStrike : public std::unary_function<CThing,bool> {
public:
    CThingFilter_CombatIsStrikeToStrike(const CThing& thing_striking) : ThingStriking(thing_striking) {}
    bool operator()(const CThing& test_thing) const;                    // 0x008565E0
private:
    const CThing& ThingStriking;                                        // +0x00
};
class CThingFilter_CombatCanCollideOrInteractWithThing : public std::unary_function<CThing,bool> {
public:
    CThingFilter_CombatCanCollideOrInteractWithThing(const CTCPhysicsBase& tc_physics, bool exclude_dead_creatures, bool exclude_creatures)
        : TCPhysics(tc_physics), ExcludeCreatures(exclude_creatures), ExcludeDeadCreatures(exclude_dead_creatures) {}
    bool operator()(const CThing& test_thing) const;                    // 0x00857250
private:
    const CTCPhysicsBase& TCPhysics;                                    // +0x00
    bool ExcludeCreatures;                                              // +0x04
    bool ExcludeDeadCreatures;                                          // +0x05
};
}

class CThingFilter_CombatOverlappingSphere : public std::unary_function<CThing,bool> {
public:
    CThingFilter_CombatOverlappingSphere(const C3DVector& pos, float radius) : SpherePos(pos), Radius(radius) {}
    FABLE_FORCEINLINE bool operator()(const CThing& test_thing) const
    {
        const CTCHitLocations* ptc_hit_locations;
        if (test_thing.PeekTC(&ptc_hit_locations)) {
            C3DVector centre_pos;
            float radius;
            ptc_hit_locations->GetBoundingSphere(centre_pos, radius, true);
            if ((centre_pos - SpherePos).GetSquaredMagnitude() < GFSqr(radius + Radius))
                return true;
        }
        return false;
    }
private:
    C3DVector SpherePos;                                                // +0x00
    float Radius;                                                       // +0x0c
};

template void CThingSearchTools::GetAllThingsInRoughArea<CThingGameObject, std::list<CThingGameObject*>, CFilter_Bind4<CThingFilter_IsNot, CThingFilter_CombatIsStrikeToStrike, CThingFilter_CombatOverlappingSphere, CThingFilter_CombatCanCollideOrInteractWithThing> >(const C2DBoxI&, const CFilter_Bind4<CThingFilter_IsNot, CThingFilter_CombatIsStrikeToStrike, CThingFilter_CombatOverlappingSphere, CThingFilter_CombatCanCollideOrInteractWithThing>&, std::list<CThingGameObject*>&) const;
