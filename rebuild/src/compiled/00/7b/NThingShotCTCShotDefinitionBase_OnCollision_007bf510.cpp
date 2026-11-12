// NThingShot::CTCShotDefinitionBase::OnCollision(ECollideType type, const C3DVector& position, const C3DVector& normal, CThing* thing)
// -- a shot reports a collision: things already in HitThingsList are ignored; otherwise, when the parent thing has a weapon-trail
// component, the collision nearest the trail tip ((points.end() - 2)->first of the last projectile trail points) is kept as the
// best collision (position, normal, thing, type); with no trail points the best collision becomes the shooter's own position with
// an up normal and no thing.
// Retail: Fable.exe (TLC, VC7.1) @ 0x007BF510, 635 bytes, thiscall `ret 0x10`, esp frame 0x18; vtable slot 28.
// Callees: CBaseIntelligentPointer ctor 0x00A01B10 / SetPItem 0x00A01B90 / GetPItem 0x00A01B50 / dtor 0x00A01C10,
//   std::__find<CIntelligentPointer<const CThing>*, CIntelligentPointer<CThing> > 0x007C2980 (fastcall, out of line),
//   CVectorMap<ETCInterfaceType,CTCBase*,...>::LowerBound 0x0040F020 (GetTC expanded), CTCWeaponTrail::HasWeaponTrailPoints
//   0x0076E4A0, CTCWeaponTrail::GetLastProjectileWeaponTrailPoints 0x0076E5D0, CIntelligentPointer<CThing>(CThing*) 0x004A96F0
//   (out of line for `PBestCollisionThing = thing`), CThing::GetPos 0x004C73D0, free 0x00BFEA14 (points vector dtor).
// Shape: the find runs through STLport's inline find() into the out-of-line __find whose body the TU sees
//   (FABLE_RETAIL_STLP_FIND_NOINLINE_BODY): VC7.1 then knows the callee never touches the empty random_access_iterator_tag
//   and homes it on the LIVE type argument slot (lea eax,[esp+28h]); the && keeps the CIntelligentPointer<CThing> value
//   temporary's destruction conditional (bl flag). `PBestCollisionThing = thing` converts through the (T*) ctor, which the
//   inliner leaves out of line there; the NULL temporary in the no-trail branch is built inline.
//   Squared distances are summed Z, Y, X (FABLE_RETAIL_C3DVECTOR_SQUAREDMAGNITUDE_ZYX).
// Leaf: ?OnCollision@CTCShotDefinitionBase@NThingShot@@UAEXW4ECollideType@@ABVC3DVector@@1PAVCThing@@@Z
#define FABLE_RETAIL_CTCSHOTDEFINITIONBASE_COLLISION
#define FABLE_RETAIL_CTCSHOTDEFINITIONBASE_MOVEMENT
#define FABLE_RETAIL_CTCSHOTDEFINITIONBASE_ONHITBYMELEE
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCHARSTRING_FORCEINLINE_EQ
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CINTELLIGENTPOINTER_COPY
#define FABLE_RETAIL_CINTELLIGENTPOINTER_CONVERT_MUTABLE
#define FABLE_RETAIL_CINTELLIGENTPOINTER_ASSIGN
#define FABLE_RETAIL_C3DVECTOR_CROSS_MEMBER_X_GROUP
#define FABLE_RETAIL_C3DVECTOR_NONZERO_SQUARED_XYZ
#define FABLE_RETAIL_C3DVECTOR_ANGLE_BODIES
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CENGINEPERSISTENTPRIMITIVEBASE
#define FABLE_RETAIL_CRESOURCELIST
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_ORIGINAL_VTABLE
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHING_CUTSCENEPAUSE
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#define FABLE_RETAIL_CTHING_GETTCDRIVER
#define FABLE_RETAIL_CTHING_MOREACCESS
#define FABLE_RETAIL_CTHING_STATEACCESS
#define FABLE_RETAIL_CTHING_LOADSINGLE
#define FABLE_RETAIL_CTHING_VIRTUALS
#define FABLE_RETAIL_CTHINGPHYSICAL_VIRTUALS
#define FABLE_RETAIL_CTCPHYSICSBASE
#define FABLE_RETAIL_CTCPHYSICSBASE_SETASMOVING
#define FABLE_RETAIL_CTCPHYSICSBASE_LASTPOSITION
#define FABLE_RETAIL_CTCENTITYEVENTS
#define FABLE_RETAIL_CTCENTITYEVENTS_ADDEVENT
#define FABLE_RETAIL_CTCDPARTICLEEMITTER_FULL
#define FABLE_RETAIL_CTCHEROEXPERIENCE
#define FABLE_RETAIL_CHITPARAMETERS_CTOR
#define FABLE_RETAIL_CWORLD
#define FABLE_RETAIL_CWORLDMAP
#define FABLE_RETAIL_CMAINGAMECOMPONENT
#define FABLE_RETAIL_CGAMEDEFINITIONMANAGER
#define FABLE_RETAIL_CDEFINITIONMANAGER
#define FABLE_RETAIL_CRIGHTHANDEDSET
#define FABLE_RETAIL_CRIGHTHANDEDSET_CTOR_INLINE
#define FABLE_RETAIL_CTCGRAPHICAPPEARANCE_FOCAL
#define FABLE_RETAIL_CINTELLIGENTPOINTER_COMPARE_NULL_INT
#define FABLE_RETAIL_IS_THING_ALIVE
#define FABLE_RETAIL_C3DVECTOR_SQUAREDMAGNITUDE_ZYX
#define FABLE_RETAIL_STLP_FIND_RANDOM_DISPATCH
#define FABLE_RETAIL_STLP_FIND_NOINLINE_BODY
#define FABLE_RETAIL_CINTELLIGENTPOINTER_EQUAL_MIXED
#define FABLE_RETAIL_CTCWEAPONTRAIL_HASPOINTS
#include "engine/CThing.h"
#include "engine/NThingShot_CTCShotDefinitionBase.h"
#include "engine/CTCWeaponTrail.h"
#include "engine/ThingAlive.h"
#include "stlport/fable_algorithm.h"

void NThingShot::CTCShotDefinitionBase::OnCollision(ECollideType type, const C3DVector& position, const C3DVector& normal, CThing* thing)
{
    if (IS_THING_ALIVE(thing) && std::find(HitThingsList.begin(), HitThingsList.end(), CIntelligentPointer<CThing>(thing)) != HitThingsList.end())
        return;
    CTCWeaponTrail* trail;
    if (ParentThing->GetTC(TCI_WEAPON_TRAIL, &trail)) {
        std::vector<std::pair<C3DVector, C3DVector> > points;
        if (trail->HasWeaponTrailPoints()) {
            if (trail->GetLastProjectileWeaponTrailPoints(points)) {
                C3DVector tip = (points.end() - 2)->first;
                if (BestCollisionType == 0 || (position - tip).GetSquaredMagnitude() < (BestCollisionPos - tip).GetSquaredMagnitude()) {
                    BestCollisionPos = position;
                    BestCollisionNormal = normal;
                    PBestCollisionThing = thing;
                    BestCollisionType = type;
                }
            }
        } else {
            PBestCollisionThing = CIntelligentPointer<CThing>(NULL);
            BestCollisionType = type;
            BestCollisionPos = ParentThing->GetPos();
            BestCollisionNormal = C3DVector(0.0f, 0.0f, 1.0f);
        }
    }
}