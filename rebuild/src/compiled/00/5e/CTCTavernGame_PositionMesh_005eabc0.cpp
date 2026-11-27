// 005eabc0 CTCTavernGame::PositionMesh
// Flags:
#define FABLE_RETAIL_C3DVECTOR_SCALE_X_PAREN
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

void CTCTavernGame::PositionMesh(CThing* thing, const C3DVector& world_pos, const CRightHandedSet& world_rhset, const C3DVector& local_pos,
                                 float local_angleXZ, float local_angleXY)
{
    if (thing) {
        bool reset = !thing->DrawIsDrawable();
        CRightHandedSet rhset = world_rhset;
        CTCPhysicsBase* physics;
        thing->GetTC(&physics);
        physics->SetAsMoving(true);
        C3DVector pos(0.0f, 0.0f, 0.0f);
        pos += rhset.GetRight() * local_pos.X;
        pos += rhset.GetForward() * local_pos.Y;
        pos += rhset.GetUp() * local_pos.Z;
        pos += world_pos;
        if (reset)
            physics->ResetPosition(pos);
        else
            physics->SetPosition(pos);
        rhset.Rotate(rhset.GetUp(), local_angleXY);
        rhset.Rotate(rhset.GetForward(), local_angleXZ);
        if (reset)
            physics->ResetRHSet(rhset);
        else
            physics->SetRHSet(rhset);
        thing->SetAsFrameUpdated();
        thing->SetAsRenderedThisFrame(4);
        thing->SetAsDrawable(true);
    }
}