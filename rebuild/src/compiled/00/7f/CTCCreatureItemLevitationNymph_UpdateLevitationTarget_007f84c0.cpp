// CTCCreatureItemLevitationNymph::UpdateLevitationTarget() -- while the hover target is alive, re-aim the hover direction from the target towards the nymph (XY plane, normalised)
// Leaf: UpdateLevitationTarget
// Plan phase 4 (2026-11-05), C3DVector wave: the local C3DVector stand-in replaced by the shared engine/C3DVector.h (PDB C3DVector TPI 0x2783: public X/Y/Z at 0/4/8, size 12).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x007F84C0, length 116 bytes (+ int3 padding). Vtable 0x012415F4 slot 31 (catalog
// "vfunc_31"; CTCCreatureItemLevitationBase::FrameUpdate 0x007F8300 calls this slot first every frame).
// Callees: CBaseIntelligentPointer::GetPItem @ 0x00A01B50 (x3), CThing::GetPos @ 0x004C73D0 (parent, then target),
// C3DVector::AccurateNormalise @ 0x00A14440.
// Shape: inline IS_THING_ALIVE, then the same C3DVector(dx, dy, 0) temporary as SetTarget; Normalise is void and the
// C2DVector member is filled by an inline Set(x, y) (a C2DVector temporary adds a frame copy).
// Layout: Ego_r.pdb CTCCreatureItemLevitationNymph (retail 8 bytes lower: the base list is one pointer, PDB 12):
// NymphHoverXYDirectionFromTarget +0x18 (C2DVector), NymphHoverHeightOffset +0x20, NymphHoverXYMagnitude +0x24,
// PTarget +0x28 (CIntelligentPointer<const CThing>, vtable 0x01237F1C). Local polymorphic model.
#include "engine/C3DVector.h"
class C2DVector { public:
    float X, Y;
    void Set(float x, float y) { X = x; Y = y; }
};
#include "engine/CThing.h"
class CBaseObject;
class CBaseObjectPointer;
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CINTELLIGENTPOINTER_ASSIGN
#include "engine/CIntelligentPointer.h"

inline bool IS_THING_ALIVE(const CIntelligentPointer<const CThing>& p)
{
    if (p.Get() != 0) {
        if (!p.Get()->IsDead())
            return true;
    }
    return false;
}
struct CTCCreatureItemLevitationNymph {
    virtual void v00();
    CThing* ParentThing;                        // +0x04 (CTCBase)
    unsigned char _pad08[0x18 - 0x08];          // CTCBase tail, LevitatingItems, NumItems, LastAddFrame
    C2DVector NymphHoverXYDirectionFromTarget;  // +0x18
    float NymphHoverHeightOffset;               // +0x20
    float NymphHoverXYMagnitude;                // +0x24
    CIntelligentPointer<const CThing> PTarget;      // +0x28
    void SetTarget(CThing* target, float xyMagnitude, float heightOffset);
    void UpdateLevitationTarget();              // vtable 0x012415F4 slot 31
};

void CTCCreatureItemLevitationNymph::UpdateLevitationTarget()
{
    if (IS_THING_ALIVE(PTarget)) {
        const C3DVector& position = ParentThing->GetPos();
        const C3DVector& targetPosition = PTarget.Get()->GetPos();
        C3DVector direction(position.X - targetPosition.X, position.Y - targetPosition.Y, 0.0f);
        direction.AccurateNormalise();
        NymphHoverXYDirectionFromTarget.Set(direction.X, direction.Y);
    }
}