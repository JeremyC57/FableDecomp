// CTCPhysicsNavigator::GetPossibleCollidedWithAreaWithImpulse(const C3DVector& impulse) const -- the integer XY box the body can touch this frame: position +/- (radius + 1), stretched along velocity-plus-impulse, orientation-validated
// Leaf: GetPossibleCollidedWithAreaWithImpulse
// Retail: Fable.exe (TLC, VC7.1) function @ 0x007267C0, length 267 bytes (+ int3 padding), `ret 8` (hidden C2DBoxI result
// + impulse). FableWin.pdb: ?GetPossibleCollidedWithAreaWithImpulse@CTCPhysicsNavigator@@ABE?AVC2DBoxI@@ABVC3DVector@@@Z
// (private, const). Called twice from CTCPhysicsNavigator::UpdateAdditionalImpulse (0x00726C68, 0x00726FDE); the catalog's
// "CTCPhysicsRigidBody::GetPossibleCollidedWithArea" label is propagated (that function is 0x00759870).
// Callees: _ftol @ 0x00BFEA70 (four times), C2DBoxI::ValidateOrientationAndFlip() @ 0x00A3A150.
// Constants: 1.0f 0x0122DED8, 0.0f 0x0122DEDC.
// Shape: the sibling CTCPhysicsControlled::GetPossibleCollidedWithArea (0x00725A30) without the margin select: the movement
// sum is two named float locals (VC7.1 keeps moveX on the x87 stack and homes moveY in the dead `impulse` argument slot; a
// C2DVector local or unnamed sums put the radius there instead), Min/Max with 0 are by-value ternary templates, and the
// four corners are converted in TL.X, TL.Y, BR.X, BR.Y order (TL = (minX, maxY), BR = (maxX, minY)), validated and copied out.
// Layout (PDB): CTCBase ParentThing +0x04, Position +0x0c, Velocity +0x28, Radius +0x34 (CTCPhysicsBase);
// engine/CTCPhysicsNavigator.h is a flattened POD with opaque base pads, so the class is modelled locally.
struct C3DVector { float X, Y, Z; };
struct C2DCoordI { long X, Y; };
struct C2DBoxI {
    C2DCoordI TopLeft;
    C2DCoordI BottomRight;
    C2DBoxI() {}
    __forceinline C2DBoxI(const C2DBoxI& o)
    {
        TopLeft.X = o.TopLeft.X;
        TopLeft.Y = o.TopLeft.Y;
        BottomRight.X = o.BottomRight.X;
        BottomRight.Y = o.BottomRight.Y;
    }
    void ValidateOrientationAndFlip();          // 0x00A3A150
};
struct CThing {
    unsigned char _pad00[0x20];
    unsigned long UsingComponents[9];           // +0x20 bitset<274>
};

template <class T> inline T Min(T a, T b) { return a < b ? a : b; }
template <class T> inline T Max(T a, T b) { return a > b ? a : b; }

struct CTCPhysicsNavigator {
    virtual void v00();
    CThing*   ParentThing;                      // +0x04
    unsigned char _pad08[0x4];
    C3DVector Position;                         // +0x0c
    C3DVector OldPosition;                      // +0x18
    long      OldPositionLastFrameSet;          // +0x24
    C3DVector Velocity;                         // +0x28
    float     Radius;                           // +0x34
    C2DBoxI GetPossibleCollidedWithAreaWithImpulse(const C3DVector& impulse) const;
};

C2DBoxI CTCPhysicsNavigator::GetPossibleCollidedWithAreaWithImpulse(const C3DVector& impulse) const
{
    float moveX = Velocity.X + impulse.X;
    float moveY = Velocity.Y + impulse.Y;
    float radius = Radius + 1.0f;
    float minX = Position.X - radius + Min(moveX, 0.0f);
    float maxX = Position.X + Max(moveX, 0.0f) + radius;
    float minY = Position.Y - radius + Min(moveY, 0.0f);
    float maxY = Position.Y + Max(moveY, 0.0f) + radius;
    C2DBoxI box;
    box.TopLeft.X = (long)minX;
    box.TopLeft.Y = (long)maxY;
    box.BottomRight.X = (long)maxX;
    box.BottomRight.Y = (long)minY;
    box.ValidateOrientationAndFlip();
    return box;
}
