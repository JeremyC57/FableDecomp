// CTCPhysicsStandard::GetPossibleCollidedWithArea() -- the integer box the body can reach this frame: the position grown by radius + 1 and stretched along the XY velocity, orientation-validated
// Leaf: GetPossibleCollidedWithArea
// Retail: Fable.exe (TLC, VC7.1) @ 0x008E5370, 258 bytes. CTCPhysicsStandard vtable 0x01265F84 slot 98 (the manifest
//   label "CTCPhysicsRigidBody" is propagated; the rigid body's own override is 0x00759870, slot 97).
// Callees: _ftol @ 0x00BFEA70 (four times), C2DBoxI::ValidateOrientationAndFlip() @ 0x00A3A150. Pools: 1.0f 0x0122DED8,
//   0.0f 0x0122DEDC.
// Layout: CTCPhysicsBase (engine/CTCPhysicsBase.h) Position +0x0c, Velocity +0x28, Radius +0x34. engine/CTCPhysicsStandard.h
//   is a flattened pad model that names +0x0c a `long`, so `this` is a local model of the base members.
// Shape: the reach is radius + 1; each axis extends the low corner by the negative part of the velocity and the high corner by
//   the positive part. C2DBoxI is TopLeft (minX, maxY) / BottomRight (maxX, minY); filled, validated and returned through the
//   hidden pointer by the __forceinline memberwise copy (no NRVO).
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
struct CTCPhysicsStandard {
    void*     __vftable;                        // +0x00
    void*     ParentThing;                      // +0x04
    unsigned char _pad08[0x4];
    C3DVector Position;                         // +0x0c
    C3DVector OldPosition;                      // +0x18
    long      OldPositionLastFrameSet;          // +0x24
    C3DVector Velocity;                         // +0x28
    float     Radius;                           // +0x34
    C2DBoxI GetPossibleCollidedWithArea();
};

C2DBoxI CTCPhysicsStandard::GetPossibleCollidedWithArea()
{
    float vx = Velocity.X;
    float vy = Velocity.Y;
    float reach = Radius + 1.0f;
    float minX = Position.X - reach + (vx < 0.0f ? vx : 0.0f);
    float maxX = (vx > 0.0f ? vx : 0.0f) + Position.X + reach;
    float minY = Position.Y - reach + (vy < 0.0f ? vy : 0.0f);
    float maxY = (vy > 0.0f ? vy : 0.0f) + reach + Position.Y;
    C2DBoxI box;
    box.TopLeft.X = (long)minX;
    box.TopLeft.Y = (long)maxY;
    box.BottomRight.X = (long)maxX;
    box.BottomRight.Y = (long)minY;
    box.ValidateOrientationAndFlip();
    return box;
}
