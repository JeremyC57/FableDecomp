// CTCPhysicsControlled::GetPossibleCollidedWithArea() const -- the integer XY box the body can touch this frame: position +/- (radius + margin), stretched along the impulse-plus-velocity movement, orientation-validated
// Leaf: GetPossibleCollidedWithArea
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00725A30, length 307 bytes (+ int3 padding), `ret 4` (hidden C2DBoxI result).
// Vtable 0x0126616C (CTCPhysicsControlled) slot 98 (catalog "CTCPhysicsControlled::vfunc_98"); FableWin.pdb names it
// ?GetPossibleCollidedWithArea@CTCPhysicsControlled@@UBE?AVC2DBoxI@@XZ.
// Callees: _ftol @ 0x00BFEA70 (four times), C2DBoxI::ValidateOrientationAndFlip() @ 0x00A3A150.
// Constants: 1.0f 0x0122DED8, 5.0f 0x012462A8 (margin when the parent has component 168 or 169: bits 8/9 of
// UsingComponents[5], `test ch,3`), 0.0f 0x0122DEDC.
// Shape: margin starts at 1.0f and is replaced by 5.0f (loaded before the frame is built); the movement sum is two named
// float locals (a C2DVector local lets VC7.1 share its frame words with the box: 0x18-byte frame instead of 0x20), each axis'
// Min/Max with 0 is a by-value ternary template, maxY is written `radius + (Position.Y + Max(...))` for retail's
// `fadd [Position.Y]` before `fadd [radius]` across the first _ftol, and the four corners are floats converted in TL.X,
// TL.Y, BR.X, BR.Y order into a C2DBoxI local (TL = (minX, maxY), BR = (maxX, minY)), validated and copied out (no NRVO).
// Layout (PDB, as the landed AddImpulseVelocity 0x00725250 / GetPossibleCollidedWithArea 0x00759870): CTCBase ParentThing +0x04,
// Position +0x0c, Velocity +0x28, Radius +0x34, CTCPhysicsControlled::ImpulseVelocity +0x88. engine/CTCPhysicsStandard.h does
// not name the CTCPhysicsBase members (opaque pads), so the class is modelled locally.
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

struct CTCPhysicsControlled {
    virtual void v00();
    CThing*   ParentThing;                      // +0x04
    unsigned char _pad08[0x4];
    C3DVector Position;                         // +0x0c
    C3DVector OldPosition;                      // +0x18
    long      OldPositionLastFrameSet;          // +0x24
    C3DVector Velocity;                         // +0x28
    float     Radius;                           // +0x34
    unsigned char _pad38[0x50];                 // +0x38 PhysicsDef .. CTCPhysicsStandard flags
    C3DVector ImpulseVelocity;                  // +0x88
    C2DBoxI GetPossibleCollidedWithArea() const;
};

C2DBoxI CTCPhysicsControlled::GetPossibleCollidedWithArea() const
{
    float margin = 1.0f;
    if (ParentThing->UsingComponents[5] & 0x300)
        margin = 5.0f;
    float moveX = ImpulseVelocity.X + Velocity.X;
    float moveY = ImpulseVelocity.Y + Velocity.Y;
    float radius = margin + Radius;
    float minX = Position.X - radius + Min(moveX, 0.0f);
    float maxX = Position.X + Max(moveX, 0.0f) + radius;
    float minY = Position.Y - radius + Min(moveY, 0.0f);
    float maxY = radius + (Position.Y + Max(moveY, 0.0f));
    C2DBoxI box;
    box.TopLeft.X = (long)minX;
    box.TopLeft.Y = (long)maxY;
    box.BottomRight.X = (long)maxX;
    box.BottomRight.Y = (long)minY;
    box.ValidateOrientationAndFlip();
    return box;
}