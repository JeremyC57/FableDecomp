// CTCTavernGame::PositionMesh(CThing* thing, const C3DVector& position, const CRightHandedSet& rhSet, const C3DVector& offset, float forwardAngle, float upAngle) -- place a tavern-game mesh: offset it in the set's right/forward/up frame from position, turn the set about Up then Forward, push both to the thing's physics component (the reset forms while the thing is not drawable), then mark the thing frame-updated, rendered and drawable
// Grade: DIFFER(495v511) -- (w22 agent1016, 141/168 instructions same) the sum as running += statements gives retail's evaluation order (Right*X, Forward*Y, Up*Z, position); residue: x87 allocation (retail keeps the running Y/Z sums in st(1)/st(2) and spills the product temporaries to [esp+20h]/[esp+24h]; ours stores the sum to memory each step) and the setter branches not head-merged (retail hoists `mov edx,[edi] ; lea eax,[esp+10h] ; push eax` above `test bl,bl / je`). One-expression sums evaluate Up*Z first (98/168).
// Leaf: ?PositionMesh@CTCTavernGame@@SIXPAVCThing@@ABVC3DVector@@ABVCRightHandedSet@@1MM@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x005EABC0, 511 bytes (+ int3 padding), static __fastcall (ecx = thing, edx = position), `ret 10h`.
// Identity: Ego_r.pdb / FableWin.pdb ?PositionMesh@CTCTavernGame@@SIXPAVCThing@@ABVC3DVector@@ABVCRightHandedSet@@1MM@Z.
// Callees: CThing slot 27 DrawIsDrawable (+0x6C) / slot 28 SetAsDrawable (+0x70), CVectorMap::LowerBound 0x0040F020 (inline GetTC,
//   TCI_PHYSICS), CTCPhysicsBase::SetAsMoving 0x006B0130, physics slots SetPosition (+0x78) / ResetPosition (+0x7C) / ResetRHSet
//   (+0x80) / SetRHSet (+0x12C), CRightHandedSet::Rotate 0x00A22F80, CThing::SetAsFrameUpdated 0x004C7C40 / SetAsRenderedThisFrame
//   0x004C7590.
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#define FABLE_RETAIL_CTHING_LOADSINGLE
#define FABLE_RETAIL_CTHING_ISPLACED
#define FABLE_RETAIL_CTHING_DRAWABLE
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CTCTAVERNGAME
#define FABLE_RETAIL_CTCTAVERNGAME_POSITIONMESH
#define FABLE_RETAIL_CTCPHYSICSBASE
#define FABLE_RETAIL_CTCPHYSICSBASE_SETASMOVING
#define FABLE_RETAIL_CTCPHYSICSBASE_STATTYPE
#define FABLE_RETAIL_CTHING_MOREACCESS
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_CRIGHTHANDEDSET
#define FABLE_RETAIL_CRIGHTHANDEDSET_ROTATE
#include "fable_string.h"
#include "engine/ETCInterfaceType.h"
#include "engine/CThing.h"
#include "engine/CTCTavernGame.h"
#include "engine/CTCPhysicsBase.h"
#include "engine/CRightHandedSet.h"

void CTCTavernGame::PositionMesh(CThing* thing, const C3DVector& position, const CRightHandedSet& rhSet, const C3DVector& offset,
                                 float forwardAngle, float upAngle)
{
    if (thing) {
        bool notDrawable = !thing->DrawIsDrawable();
        CRightHandedSet set = rhSet;
        CTCPhysicsBase* physics;
        thing->GetTC(&physics);
        physics->SetAsMoving(true);
        C3DVector pos = set.GetRight() * offset.X;
        pos += set.GetForward() * offset.Y;
        pos += set.GetUp() * offset.Z;
        pos += position;
        if (notDrawable)
            physics->ResetPosition(pos);
        else
            physics->SetPosition(pos);
        set.Rotate(set.GetUp(), upAngle);
        set.Rotate(set.GetForward(), forwardAngle);
        if (notDrawable)
            physics->ResetRHSet(set);
        else
            physics->SetRHSet(set);
        thing->SetAsFrameUpdated();
        thing->SetAsRenderedThisFrame(4);
        thing->SetAsDrawable(true);
    }
}
