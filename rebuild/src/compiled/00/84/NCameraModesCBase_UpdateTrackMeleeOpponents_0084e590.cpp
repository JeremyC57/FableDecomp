// NCameraModes::CBase::UpdateTrackMeleeOpponents(const C3DVector& new_cam_pos, C3DVector& look_dir) const -- melee-tracking camera helper: when the observed thing is a live creature, gather the live threats within 20 units that are aware of it into a frustum from the camera position, and when the hero is not running away from their focus (velocity along the hero-to-focus line not below -0.5 x max running speed) pull the frustum towards the hero's focal point (bias 2.5); in this build the result is not written back (the debug-era desired_look_vector is dead)
// Leaf: ?UpdateTrackMeleeOpponents@CBase@NCameraModes@@
// Retail: Fable.exe (TLC, VC7.1) @ 0x0084E590, 593 bytes, __fastcall, `ret 8`.
// Identity: Ego_r.pdb / FableWin.pdb ?UpdateTrackMeleeOpponents@CBase@NCameraModes@@QBEXABVC3DVector@@AAV3@@Z (FableWin debug 0x02882FD0; public, Ego_d LF_ONEMETHOD).
//   Locals: Ego_d.pdb's record (debug 940): new_cam_pos, look_dir, pthing; { centre_pos, pcreature, threat_things, COMBAT_SEARCH_RADIUS;
//   { FRACTION_OF_MAX_RUNNING_SPEED_TO_BE_CONSIDERED_RUNNING_AWAY, running_away_amount, line_to_combat_focus, to_pos, combatants, focus,
//   hero_velocity, default_look_pos; { it; { pcret } } { HERO_BIAS, desired_look_vector } } } -- the block structure below. Statements
//   in FableWin's order, except as noted.
// Callees: CBaseIntelligentPointer::GetPItem 0x00A01B50 (inline GetObservedThing, same body as 0x0084F1B0), CThing::GetPos 0x004C73D0,
//   C3DVector::ToC2DCoordI, GetAllThingsInRoughArea<const CThingCreatureBase, vector<const CThingCreatureBase*>,
//   CFilter_Bind2<CThingFilter_IsCreatureThreatTo, CThingFilter_IsAwareOfThing> > 0x0084F850 (this TU's visible copy, the
//   FABLE_RETAIL_CTHINGSEARCHTOOLS_ROUGHAREA_EGOD body, grades RELOCATION_MATCH at that address), C3DFrustrum ctor 0x0084F4D0 /
//   AddPoint 0x0084E390 / UpdateFrustrum 0x0084D0E0 (anonymous namespace of this TU, Ego_r.pdb ?..@C3DFrustrum@?A0x49b2e1a3@@..),
//   CTCGraphicAppearance::GetCameraFocalPos 0x004C12D0 (through the inline GetStaticCameraFocalPos), C3DVector::AccurateNormalise
//   0x00A14440, CTCPhysicsBase vslot +0x118 (GetFacingVector), CThingCreatureBase vslot +0x144 (GetMaxRunningSpeed), free 0x00BFEA14.
// Shape (each verified by grade, ATTEMPTS.log): (1) the visible rough-area body is the lever for the vector: VC7.1 then keeps
//   threat_things uncaptured and reads begin/end into edi/ebp for the empty test and the loop (with the declaration only, the end
//   pointer is reloaded each step); (2) GetStaticCameraFocalPos() (Ego_d / FableWin, inline in release) computes the focal position
//   before the AddPoint weight is pushed; (3) default_look_pos is declared before focus (FableWin's order is focus first: the copy
//   then sits across the GetCameraFocalPos call; retail reads the frustum's Focus after it); (4) the running-away test.
#include <math.h>
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CWIDESTRING
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CTHING_ORIGINAL_VTABLE
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CINTELLIGENTPOINTER_GET_ORIGINAL_CHAIN
#define FABLE_RETAIL_CCOUNTEDPOINTER
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_C2DBOXI
#define FABLE_RETAIL_IS_THING_ALIVE
#define FABLE_RETAIL_CWORLD
#define FABLE_RETAIL_CTHINGSEARCHTOOLS
#define FABLE_RETAIL_STLP_VECTOR_OVERFLOW_OUTOFLINE
#define FABLE_RETAIL_CTHINGSEARCHTOOLS_CLIPAREA
#define FABLE_RETAIL_CTHINGSEARCHTOOLS_ORIGINAL_ROUGHAREA
#define FABLE_RETAIL_CTHINGSEARCHTOOLS_ROUGHAREA_EGOD
#define FABLE_RETAIL_CTHINGMAP_PEEKCELL_INLINE
#define FABLE_RETAIL_CTCMAPWHO
#define FABLE_RETAIL_CTCMAPWHO_SEARCHID
#define FABLE_RETAIL_CTHINGCREATUREBASE_STATTYPEFLAGS
#define FABLE_RETAIL_C2DBOXI_COPY
#define FABLE_RETAIL_C2DBOXI_GETWIDTH
#define FABLE_RETAIL_C2DBOXI_GETHEIGHT
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTCPHYSICSBASE
#define FABLE_RETAIL_CTCPHYSICSBASE_LASTPOSITION
#define FABLE_RETAIL_CTCGRAPHICAPPEARANCE_CAMERAFOCALPOS
#define FABLE_RETAIL_CTCGRAPHICAPPEARANCE_STATICCAMERAFOCALPOS
#define FABLE_RETAIL_NCAMERAMODES_CBASE_CYLINDER
#define FABLE_RETAIL_CGAMECAMERAMANAGER_GETOBSERVEDTHING
#define FABLE_RETAIL_CGAMECAMERAMANAGER_LOOKAROUND
#define FABLE_RETAIL_CGAMECAMERAMANAGER_LOOKAROUND_WORLD
#include "stlport/fable_containers.h"
#include "engine/C3DVector.h"
#include "engine/C2DExtentsI.h"
#include "engine/C2DBoxI.h"
// Include-order: engine/CTCMapwho.h names CTCMapwho in NThingMap::CMapwhoNode before its own class (as the landed 0x0087A7C0).
class CTCMapwho;
#include "engine/CTCMapwho.h"
#include "engine/NThingMap_CThingMap.h"
#include "engine/ThingAlive.h"
#include "engine/CThingCreatureBase.h"
#include "engine/CTCPhysicsBase.h"
#include "engine/NGraphicAppearance_CTCGraphicAppearance.h"
#include "engine/CWorld.h"
#include "engine/CThingSearchTools.h"
#include "engine/CFilter_Bind2.h"
#include "engine/NCameraModes_CBase.h"
#include "engine/CGameCameraManager.h"

// No shared headers for these three classes. CThingFilter_IsCreatureThreatTo (member Creature) and CThingFilter_IsAwareOfThing (member
//   PThingToBeAwareOf): Ego_d.pdb records (one-argument constructor, operator(), the private pointer; out-of-line operator() at the
//   retail addresses of their landed rows). C3DFrustrum: Ego_d.pdb `anonymous-namespace'::C3DFrustrum (Apex, Up, Points, FOV, Focus;
//   C3DFrustrum / IncorporatePoint / AddPoint / UpdateFrustrum / GetFOV / GetFocus / GetApex / GetUp) and Ego_r.pdb's out-of-line members
//   ?..@C3DFrustrum@?A0x49b2e1a3@@ (AddPoint QAEXABVC3DVector@@M@Z, IncorporatePoint the same, GetApex / GetFocus QBEABVC3DVector@@XZ);
//   with the STLport vector Focus sits at +0x28 (retail `fld [esp+7Ch]`, frustum at [esp+54h]). IncorporatePoint is inline here (retail
//   calls AddPoint 0x0084E390 then UpdateFrustrum 0x0084D0E0 at 0x0084E7A8 / 0x0084E7B1); Inferred body: add, then update.
class CThingFilter_IsCreatureThreatTo {
public:
    CThingFilter_IsCreatureThreatTo(const CThingCreatureBase& creature) : Creature(&creature) { }
    bool operator()(const CThingCreatureBase& thing) const;              // 0x006A7600
private:
    const CThingCreatureBase* Creature;
};
class CThingFilter_IsAwareOfThing {
public:
    CThingFilter_IsAwareOfThing(const CThing* thing) : PThingToBeAwareOf(thing) { }
    bool operator()(const CThing& thing) const;                          // 0x006A8630
private:
    const CThing* PThingToBeAwareOf;
};

namespace {
class C3DFrustrum {
public:
    C3DFrustrum(const C3DVector& apex, const C3DVector& up);            // 0x0084F4D0
    void AddPoint(const C3DVector& point, float weight);                 // 0x0084E390
    void UpdateFrustrum();                                               // 0x0084D0E0
    void IncorporatePoint(const C3DVector& point, float weight) { AddPoint(point, weight); UpdateFrustrum(); }
    const C3DVector& GetApex() const { return Apex; }
    const C3DVector& GetFocus() const { return Focus; }
private:
    C3DVector Apex;
    C3DVector Up;
    std::vector<std::pair<C3DVector, float> > Points;
    float FOV;
    C3DVector Focus;
};
}

namespace NCameraModes {
// Same bodies as the landed out-of-line rows 0x0084F1B0 GetObservedThing and 0x0084D050 GetWorld; inline here, as in the landed
//   UpdateCameraStretchAroundHero 0x0084D910 (retail expands both: `lea ecx,[eax+100h] ; call 0x00A01B50` and the +0x74 / +0x20 reads).
inline const CThingGameObject* CBase::GetObservedThing() const
{
    return CameraManager.GetObservedThing();
}
inline const CWorld& CBase::GetWorld() const
{
    return CameraManager.GetWorld();
}

void CBase::UpdateTrackMeleeOpponents(const C3DVector& new_cam_pos, C3DVector& look_dir) const
{
    const CThingGameObject* pthing = GetObservedThing();
    if (IS_THING_ALIVE(pthing) && (pthing->GetTypeFlags() & 8)) {
        const CThingCreatureBase* pcreature = static_cast<const CThingCreatureBase*>(pthing);
        const float COMBAT_SEARCH_RADIUS = 20.0f;
        C3DVector centre_pos(pthing->GetPos());
        std::vector<const CThingCreatureBase*> threat_things;
        GetWorld().PeekThingSearchTools().GetAllThingsInRoughArea<const CThingCreatureBase>(C2DBoxI(centre_pos.ToC2DCoordI(), (long)COMBAT_SEARCH_RADIUS),
            GFFilter_Bind2(CThingFilter_IsCreatureThreatTo(*pcreature), CThingFilter_IsAwareOfThing(pthing)), threat_things);
        if (!threat_things.empty()) {
            C3DFrustrum combatants(new_cam_pos, C3DVector(0.0f, 0.0f, 1.0f));
            for (std::vector<const CThingCreatureBase*>::iterator it = threat_things.begin(); it != threat_things.end(); ++it) {
                const CThingCreatureBase* pcret = *it;
                if (IS_THING_ALIVE(pcret))
                    combatants.AddPoint(pcret->GetTCGraphicAppearance()->GetStaticCameraFocalPos(), 1.0f);
            }
            combatants.UpdateFrustrum();
            // Inferred: default_look_pos before focus (FableWin declares focus first); see Shape (3).
            C3DVector default_look_pos = pcreature->GetTCGraphicAppearance()->GetStaticCameraFocalPos();
            C3DVector focus(combatants.GetFocus());
            C3DVector line_to_combat_focus = focus - default_look_pos;
            line_to_combat_focus.AccurateNormalise();
            C3DVector hero_velocity = pcreature->GetPhysicsTC()->GetPosition() - pcreature->GetPhysicsTC()->GetLastPosition();
            float running_away_amount = line_to_combat_focus.Dot(hero_velocity);
            const float FRACTION_OF_MAX_RUNNING_SPEED_TO_BE_CONSIDERED_RUNNING_AWAY = 0.5f;
            C3DVector to_pos = pcreature->GetPos() + pcreature->GetPhysicsTC()->GetFacingVector();
            // Inferred: FableWin tests `running_away_amount >= speed * FRACTION * -1.0` (a double -1.0: `fmul qword`, `test ah,5 ; jp`);
            //   retail multiplies once by the float -0.5f (0x01238798) and branches `test ah,41h ; je`, which this `!(<)` with a float
            //   -1.0f gives (the `>=` spelling grades `jp`, DIFFER 593v593).
            if (!(running_away_amount < pcreature->GetMaxRunningSpeed(to_pos) * FRACTION_OF_MAX_RUNNING_SPEED_TO_BE_CONSIDERED_RUNNING_AWAY * -1.0f)) {
                const float HERO_BIAS = 2.5f;
                combatants.IncorporatePoint(default_look_pos, HERO_BIAS);
                // Fidelity: desired_look_vector (Ego_d.pdb, FableWin 0x02883349) is never used; look_dir is never written (retail
                //   ends after UpdateFrustrum at 0x0084E7B1).
                C3DVector desired_look_vector = combatants.GetFocus() - combatants.GetApex();
            }
        }
    }
}
}