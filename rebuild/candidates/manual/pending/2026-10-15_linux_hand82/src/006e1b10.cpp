// CTCCameraPointScripted::EditSetThingForEditorSetThing() -- catalog label; the body returns a copy of the C3DVector member at +0xd4 by value
// Leaf: GetPosition
//
// Retail 0x006E1B10, 33 bytes (`ret 4`: the hidden result pointer is the only stack argument and is
// returned in eax). No callees. Called from CParticleEmitter::IsVisible (0x00AED2F0), so the
// camera-point label is propagated over an unrelated body; the owner is modelled as
// CPositionedObject_006e1b10 with the vector at +0xd4 (named descriptively).
// Shape: `return C3DVector(Position.X, Position.Y, Position.Z);` -- the (x, y, z) value constructor
// with its arguments evaluated right to left: Z and Y go through the FPU (`fld [ecx+0DCh] ;
// fld [ecx+0D8h]`), X is an integer move, then Y, X, Z are stored into the hidden result. A plain
// `return Position;` through the memberwise copy constructor is three integer moves (33v33).

struct C3DVector
{
    float X, Y, Z;
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) {}
    C3DVector(const C3DVector& other) : X(other.X), Y(other.Y), Z(other.Z) {}
};

struct CPositionedObject_006e1b10
{
    unsigned char _pad_0x00[0xd4];
    C3DVector     Position;                         // +0xd4

    C3DVector GetPosition() const;
};

C3DVector CPositionedObject_006e1b10::GetPosition() const
{
    return C3DVector(Position.X, Position.Y, Position.Z);
}
