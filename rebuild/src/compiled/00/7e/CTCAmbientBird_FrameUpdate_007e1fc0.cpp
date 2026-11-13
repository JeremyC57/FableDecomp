// CTCAmbientBird::FrameUpdate() -- a resting bird takes off when the hero comes within 5.6 units (squared distance < 31.36): it marks
//   itself flying, records its take-off height, plays "ST_FLY" if it animates, faces away from the hero (fast-normalised, or +Y when
//   the hero is on top of it) pitched up about the side axis, and turns gravity off; a flying bird climbs along its facing vector
//   (0.8 units a frame, straight up when that leaves the loaded world map) and is killed 40 units above its take-off height.
// Leaf: ?FrameUpdate@CTCAmbientBird@@UAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x007E1FC0, 843 bytes, `ret` (vtable 0x0123DC0C slot 9).
// Callees: CThing::GetPos 0x004C73D0, CThing::Kill(bool) 0x004C9B80, CTCBase::GetWorldMap 0x00686D30, CTCBase::PeekPlayerManager
//   0x00686D50, CPlayerManager::GetMainPlayer 0x00449960, CPlayer::PeekPControlledCreature 0x00487DD0, CVectorMap<ETCInterfaceType,
//   CTCBase*>::LowerBound 0x0040F020 (inside CThing::GetTC), CCharString 0x0099EBF0 / ~CCharString 0x0099EAE0,
//   CTCAnimatingObject::PlayAnimation(const CCharString&, long, bool) 0x007F90B0, CRightHandedSet::Rotate 0x00A22F80; virtuals
//   CTCPhysicsBase::GetFacingVector (slot 70), SetPosition (30), SetRHSet (75), EnableGravity (38), CWorldMap::GetMapNumberAt
//   (slot 16) and IsMapLoaded (slot 10, +0x28 -- not IsMapUsed, slot 11). Globals: GFastOneOverSqrtTable 0x013CE558,
//   DELTA_NOTIONAL_ZERO 0x0129BA3C.
// Shape: the parent thing is read into a local before the hero's GetPos (retail holds it in ebp around the two calls); the
//   away vector is a temporary bound to a const reference (that is what makes VC7.1 load Y / Z before the spilled one-over-magnitude
//   at 0x007E2259); its squares are shared between the distance test, IsNonZero (X, Y, Z sum, FABLE_RETAIL_C3DVECTOR_NONZERO_SQUARED_XYZ)
//   and Normalise (parenthesised squares re-summed X, Y, Z: overlay FABLE_RETAIL_C3DVECTOR_NORMALISE_PAREN_SQUARES_XYZ); forward is
//   member-copied so VC7.1 keeps it on the x87 stack across the join into CRightHandedSet / operator^.
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTCBASE_PEEKPLAYERMANAGER
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#define FABLE_RETAIL_CTHING_HASTC_FORCEINLINE
#define FABLE_RETAIL_CTCPHYSICSBASE
#define FABLE_RETAIL_CRIGHTHANDEDSET
#define FABLE_RETAIL_CRIGHTHANDEDSET_CTOR_INLINE
#define FABLE_RETAIL_CRIGHTHANDEDSET_ROTATE
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_C3DVECTOR_NORMALISE_PAREN_SQUARES_XYZ
#define FABLE_RETAIL_C3DVECTOR_NONZERO_SQUARED_XYZ
#define FABLE_RETAIL_CWORLDMAP
#define FABLE_RETAIL_CPLAYERMANAGER
#define FABLE_RETAIL_PLAYER
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CTHINGPLAYERCREATURE
#define FABLE_RETAIL_CTCANIMATINGOBJECT_PLAYANIMATION_NAME
#include "fable_string.h"
#include "engine/CThing.h"
#include "engine/CTCBase.h"
#include "engine/CTCPhysicsBase.h"
#include "engine/CRightHandedSet.h"
#include "engine/C3DVector.h"
#include "engine/CWorldMap.h"
#include "engine/CPlayerManager.h"
#include "engine/CPlayer.h"
#include "engine/CThingPlayerCreature.h"
#include "engine/CTCAnimatingObject.h"
#include "engine/ETCInterfaceType.h"

class CTCAmbientBird : public CTCBase {
    bool  Flying;          // +0x0c
    float InitialHeight;   // +0x10
public:
    virtual void FrameUpdate();
};

void CTCAmbientBird::FrameUpdate()
{
    if (Flying) {
        if (ParentThing->GetPos().Z - InitialHeight > 40.0f) {
            ParentThing->Kill(false);
            return;
        }
        C3DVector offset = ParentThing->GetPhysicsTC()->GetFacingVector() * 0.8f;
        C3DVector target = offset + ParentThing->GetPos();
        long map = GetWorldMap().GetMapNumberAt(target);
        if (!GetWorldMap().IsMapLoaded(map)) {
            const C3DVector& pos = ParentThing->GetPos();
            target = C3DVector(pos.X, pos.Y, pos.Z + 0.8f);
        }
        ParentThing->GetPhysicsTC()->SetPosition(target);
        return;
    }
    const CThingPlayerCreature* hero = PeekPlayerManager().GetMainPlayer().PeekPControlledCreature();
    if (!hero || hero->IsDead())
        return;
    CThing* parent = ParentThing;
    const C3DVector& away = parent->GetPos() - hero->GetPos();
    float distanceSquared = away.X * away.X + away.Y * away.Y + away.Z * away.Z;
    if (distanceSquared < 31.36f) {
        Flying = true;
        InitialHeight = ParentThing->GetPos().Z;
        CTCAnimatingObject* anim;
        if (ParentThing->GetTC(TCI_ANIMATING_OBJECT, &anim))
        {
            CCharString name("ST_FLY");
            anim->PlayAnimation(name, 1, true);
        }
        C3DVector forward;
        forward.X = away.X;
        forward.Y = away.Y;
        forward.Z = away.Z;
        if (away.IsNonZero())
            forward.Normalise();
        else {
            forward.X = 0.0f;
            forward.Y = 1.0f;
            forward.Z = 0.0f;
        }
        C3DVector up(0.0f, 0.0f, 1.0f);
        CRightHandedSet rhs(up, forward);
        rhs.Rotate(forward ^ up, -0.17777778f);
        ParentThing->GetPhysicsTC()->SetRHSet(rhs);
        ParentThing->GetPhysicsTC()->EnableGravity(false);
    }
}