// CTCPerceiveThing::UpdateLineOfSight() -- refresh the perceiver's three line-of-sight flags towards its perceived thing: out of extended sight range (radii scaled by the target's stealth visibility), an unviewable stealthy target, a blocked world-map line or a physics mesh between the two eye points clears all three; otherwise the plain line of sight (in the view cone, unless the perceiver is in melee) or the extended one is set by range, and in plain range direct awareness follows the view cone or a recent hit
// Leaf: ?UpdateLineOfSight@CTCPerceiveThing@@AAEXXZ
// Retail: Fable.exe (TLC, VC7.1 RTM) @ 0x00745120, 1538 bytes, plain `ret` (private, Ego_r.pdb / FableWin.pdb
//   ?UpdateLineOfSight@CTCPerceiveThing@@AAEXXZ; Ego_r donor order UpdateHearing 0x00757B60 / UpdateLineOfSight 0x00757CE0 /
//   UpdatePerception 0x00758320 = retail 0x00744FB0 / 0x00745120 / 0x00745730; the landed UpdatePerception calls it).
// Callees (all checked against retail with rel.py): CBaseIntelligentPointer::GetPItem 0x00A01B50, CVectorMap<..>::LowerBound const
//   0x004365B0 (the expanded PeekTC<const CTCStealth> / PeekTC<const CTCCombat>), CTCStealth::GetVisibilityMultiplier 0x00747660,
//   GetExtendedSightRadius 0x00744950 / GetSightRadius 0x007448F0, CPlayerGui::RemovePerceivingThing 0x00437900 / AddPerceivingThing
//   0x004378F0 (singleton PClass 0x013B8790), CThing::GetPos 0x004C73D0, CTCCombat::IsInMelee 0x00733190, CTCPhysicsBase::GetFacingVector
//   (virtual +0x118) / GetPhysicsMeshInfo (virtual +0x174) / GetPhysicsMeshObject 0x006B0850, CThingFilter_IsInFOV ctor 0x006A7150 /
//   operator() 0x006A7390, CTCBase::PeekWorldMap 0x00686D20 / PeekThingSearchTools 0x00686D90 / GetWorld 0x00686A70, CWorldMap::IsLineOfSight
//   0x0063AEE0, __ftol2 0x00BFEA70, vector<const CThing*>::reserve 0x007463E0, CThingSearchTools::GetAllThingsInRoughArea<const CThing,
//   vector<const CThing*>, CFilter_Bind3<HasPhysicsMesh, RadiusIntersectsInfiniteLine, IsNot> > 0x00746560, CPhysicsMeshObject::
//   GetPointIntersectionFromTo 0x00A12450 / ~CPhysicsMeshObject 0x00A127D0, CWorld::GetFrame 0x0049D870, SetThingLineOfSight 0x00744A00,
//   SetThingExtendedLineOfSight 0x00744A70, SetLastAwareInfo 0x00744730, free 0x00BFEA14 (STLport vector storage).
// Constants: 0.0f 0x0122DEDC, 1.0f 0x0122DED8, 1.5f 0x0125AADC (eye height), 1/360 0x01238E00 (GetFOV's `/ 360.0f`).
// Shape: everything is in the CTCPerceiveThing TU in retail, so the same-TU members are defined here as retail compiled them:
//   GetFOV (out of line at 0x007445D0, expanded here), and the three line-of-sight setters (out of line at 0x00744A00 / 0x00744A70;
//   VC7.1 itself expands every constant-argument call and calls the two variable-argument ones, as retail does). The STLport
//   reserve body (FABLE_RETAIL_STLP_VECTOR_RESERVE) and the search body (FABLE_RETAIL_CTHINGSEARCHTOOLS_ROUGHAREABODY; retail
//   0x00746560 follows in the TU) are visible, so VC7.1 knows `things` is not captured: the loop keeps end() in ebx and the 0 / 0x36
//   constants take ebp (declaration only: end() reloaded every pass, 1470 bytes). The bounding box is filled member by member in
//   retail's order (TLX, BRX, TLY, BRY) from value min / max selections (std::min / max select through a reference: `lea eax`).
//   Frame (0xC8, every slot at retail's offset): the mesh intersection record is declared at function scope, so it conflicts with
//   the view-cone filter and the filter shares the mesh's slot instead (frame 0xD0 when it is loop-local); the empty
//   CThingFilter_HasPhysicsMesh is a named local (a named 1-byte object gets a 4-byte node of density 250 and sorts between the line
//   filter and the mesh, [esp+78h]; as a temporary it is a 1-byte node of density 1000 sorted in front of `things`).
// Layout: shared headers (engine/CTCPerceiveThing.h FABLE_RETAIL_CTCPERCEIVETHING_SMELL); overlay opt-ins in hdr/ (README.md):
//   CPlayerGui PERCEIVERS, CTCStealth STATTYPE, CTCCombat ISINMELEE, CThingFilter_IsInFOV CTOR, new CThingFilter_HasPhysicsMesh.h and
//   CThingFilter_RadiusIntersectsInfiniteLine.h.
#define FABLE_RETAIL_HEARING
#define FABLE_RETAIL_CTCPERCEIVETHING_SMELL
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#define FABLE_RETAIL_CTHING_STATTYPEFLAGS
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CWORLD
#define FABLE_RETAIL_CTCPHYSICSBASE
#define FABLE_RETAIL_CTCCOMBAT
#define FABLE_RETAIL_CTCCOMBAT_ISINMELEE
#define FABLE_RETAIL_CCOMBATABILITY
#define FABLE_RETAIL_CCOMBATABILITYDATA
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_C2DBOXI
#define FABLE_RETAIL_CWORLDMAP
#define FABLE_RETAIL_CWORLDMAP_LINEOFSIGHT
#define FABLE_RETAIL_STEALTH_DRAW
#define FABLE_RETAIL_CTCSTEALTH_STATTYPE
#define FABLE_RETAIL_CPLAYERGUI
#define FABLE_RETAIL_CPLAYERGUI_PERCEIVERS
#define FABLE_RETAIL_CTHINGFILTER_ISINFOV_CTOR
#define FABLE_RETAIL_CTHINGSEARCHTOOLS
#define FABLE_RETAIL_FILTER_BIND3_FUNCTION
#define FABLE_RETAIL_C2DBOXI_CTORS
#define FABLE_RETAIL_STLP_VECTOR
#define FABLE_RETAIL_STLP_VECTOR_RESERVE
#define FABLE_RETAIL_CTHINGSEARCHTOOLS_ROUGHAREABODY
#define FABLE_RETAIL_CTCMAPWHO
#define FABLE_RETAIL_CTCMAPWHO_SEARCHID
#define FABLE_RETAIL_CTCMAPWHO_STATTYPE
#include "engine/CTCPerceiveThing.h"
#include "engine/CPerceivedThingDef.h"
#include "engine/CTCStealth.h"
#include "engine/CTCCombat.h"
#include "engine/CTCPhysicsBase.h"
#include "engine/CPhysicsMeshObject.h"
#include "engine/CMeshIntersectionData.h"
#include "engine/CThingFilter_IsInFOV.h"
#include "engine/CThingFilter_IsNot.h"
#include "engine/CThingFilter_HasPhysicsMesh.h"
#include "engine/CThingFilter_RadiusIntersectsInfiniteLine.h"
#include "engine/CFilter_Bind3.h"
#include "engine/CTCMapwho.h"
#include "engine/NThingMap_CThingMap.h"
#include "engine/CThingSearchTools.h"
#include "engine/CWorldMap.h"
#include "engine/CWorld.h"
#include "engine/CPlayerGui.h"
#include "engine/C2DBoxI.h"

float CTCPerceiveThing::GetFOV() const
{
    return PDef->FieldOfView / 360.0f;
}

void CTCPerceiveThing::SetThingLineOfSight(bool line_of_sight)
{
    if (LineOfSight != line_of_sight) {
        if (LineOfSight)
            CTBaseSingletonNoDefaultAlloc<CPlayerGui>::Get().RemovePerceivingThing(GetParentThing());
        LineOfSight = line_of_sight;
        if (line_of_sight) {
            if (GetPerceivedThing())
                CTBaseSingletonNoDefaultAlloc<CPlayerGui>::Get().AddPerceivingThing(GetParentThing());
            else
                CTBaseSingletonNoDefaultAlloc<CPlayerGui>::Get().RemovePerceivingThing(GetParentThing());
        }
    }
    if (LineOfSight)
        FramesSinceDirectlyPerceivedThing = 0;
}

void CTCPerceiveThing::SetThingExtendedLineOfSight(bool line_of_sight)
{
    if (ExtendedLineOfSight != line_of_sight) {
        if (ExtendedLineOfSight)
            CTBaseSingletonNoDefaultAlloc<CPlayerGui>::Get().RemovePerceivingThing(GetParentThing());
        ExtendedLineOfSight = line_of_sight;
        if (line_of_sight) {
            if (GetPerceivedThing())
                CTBaseSingletonNoDefaultAlloc<CPlayerGui>::Get().AddPerceivingThing(GetParentThing());
            else
                CTBaseSingletonNoDefaultAlloc<CPlayerGui>::Get().RemovePerceivingThing(GetParentThing());
        }
    }
}

void CTCPerceiveThing::SetThingIgnoreFOVLineOfSight(bool line_of_sight)
{
    if (IgnoreFOVLineOfSight != line_of_sight) {
        if (IgnoreFOVLineOfSight)
            CTBaseSingletonNoDefaultAlloc<CPlayerGui>::Get().RemovePerceivingThing(GetParentThing());
        IgnoreFOVLineOfSight = line_of_sight;
        if (line_of_sight) {
            if (GetPerceivedThing())
                CTBaseSingletonNoDefaultAlloc<CPlayerGui>::Get().AddPerceivingThing(GetParentThing());
            else
                CTBaseSingletonNoDefaultAlloc<CPlayerGui>::Get().RemovePerceivingThing(GetParentThing());
        }
    }
}

void CTCPerceiveThing::UpdateLineOfSight()
{
    const CThing* perceived_thing = GetPerceivedThing();
    CMeshIntersectionData intersection;
    if (!IsAwareOfThing())
        DirectAwareness = false;

    float visibility = 1.0f;
    const CTCStealth* ptc_stealth;
    if (perceived_thing->PeekTC(&ptc_stealth))
        visibility = ptc_stealth->GetVisibilityMultiplier();
    float extended_sight_radius = GetExtendedSightRadius() * visibility;
    float sight_radius = GetSightRadius() * visibility;
    float sight_radius_sq = sight_radius * sight_radius;
    if (extended_sight_radius * extended_sight_radius < DistanceFromPerceivedThingSquared) {
        SetThingLineOfSight(false);
        SetThingExtendedLineOfSight(false);
        SetThingIgnoreFOVLineOfSight(false);
        return;
    }
    if (perceived_thing->PeekTC(&ptc_stealth) && !ptc_stealth->IsViewable()) {
        SetThingLineOfSight(false);
        SetThingExtendedLineOfSight(false);
        SetThingIgnoreFOVLineOfSight(false);
        return;
    }

    const C3DVector& my_pos = GetParentThing().GetPos();
    C3DVector eye_pos(my_pos.X, my_pos.Y, my_pos.Z + 1.5f);
    bool in_fov = true;
    const CTCCombat* ptc_combat;
    if (!(GetParentThing().PeekTC(&ptc_combat) && ptc_combat->IsInMelee())) {
        CThingFilter_IsInFOV fov_filter(eye_pos, GetParentThing().GetPhysicsTC()->GetFacingVector(), GetFOV(), 0.0f, false);
        if (!fov_filter(*perceived_thing))
            in_fov = false;
    }

    const C3DVector& perceived_pos = perceived_thing->GetPos();
    C3DVector perceived_eye_pos(perceived_pos.X, perceived_pos.Y, perceived_pos.Z + 1.5f);
    if (!PeekWorldMap().IsLineOfSight(eye_pos, perceived_eye_pos)) {
        SetThingLineOfSight(false);
        SetThingExtendedLineOfSight(false);
        SetThingIgnoreFOVLineOfSight(false);
        return;
    }

    C2DBoxI area;
    area.TLX = (long)((eye_pos.X < perceived_eye_pos.X ? eye_pos.X : perceived_eye_pos.X) - 1.0f);
    area.BRX = (long)((eye_pos.X > perceived_eye_pos.X ? eye_pos.X : perceived_eye_pos.X) + 1.0f);
    area.TLY = (long)((eye_pos.Y < perceived_eye_pos.Y ? eye_pos.Y : perceived_eye_pos.Y) - 1.0f);
    area.BRY = (long)((eye_pos.Y > perceived_eye_pos.Y ? eye_pos.Y : perceived_eye_pos.Y) + 1.0f);
    CThingFilter_HasPhysicsMesh has_physics_mesh_filter;
    CThingFilter_RadiusIntersectsInfiniteLine line_filter(eye_pos, perceived_eye_pos);
    CThingFilter_IsNot not_me_filter(GetParentThing());
    std::vector<const CThing*> things;
    things.reserve(32);
    PeekThingSearchTools().GetAllThingsInRoughArea<const CThing>(area, GFFilter_Bind3(has_physics_mesh_filter, line_filter, not_me_filter), things);
    for (std::vector<const CThing*>::iterator it = things.begin(); it != things.end(); ++it) {
        const CThing* thing = *it;
        if (thing->GetPhysicsTC()->GetPhysicsMeshInfo()) {
            CPhysicsMeshObject mesh = thing->GetPhysicsTC()->GetPhysicsMeshObject();
            if (mesh.GetPointIntersectionFromTo(eye_pos, perceived_eye_pos, intersection)) {
                SetThingLineOfSight(false);
                SetThingExtendedLineOfSight(false);
                SetThingIgnoreFOVLineOfSight(false);
                return;
            }
        }
    }

    if (DistanceFromPerceivedThingSquared < sight_radius_sq) {
        SetThingLineOfSight(in_fov);
        SetThingExtendedLineOfSight(false);
        SetThingIgnoreFOVLineOfSight(true);
        if (!in_fov && FrameHitByPerceivedThing > 0)
            in_fov = GetWorld().GetFrame() - FrameHitByPerceivedThing < 30;
        DirectAwareness = in_fov;
    } else {
        SetThingLineOfSight(false);
        SetThingExtendedLineOfSight(in_fov);
        SetThingIgnoreFOVLineOfSight(false);
        SetLastAwareInfo();
    }
}