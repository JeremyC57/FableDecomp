// CTCCameraPointDefinitionBase::GetActiveCameraPosition(const CThing* thing) -- the camera position for a thing: its position raised by 1.6 units
//
// Retail 0x00878A10, 52 bytes. Callee: CThing::GetPosition() const @ 0x004C73D0 (returns a const
// C3DVector&; manifest label "CVertexBufferWin32::DoSizeof" propagated). Constant 0x012777F0 = 1.6f.
// No direct callers (no E8/E9 reference): reached through a pointer.
// Shape: a static `__fastcall` returning the C3DVector (a class with user constructors) through the
// hidden pointer in ecx, the thing in edx (`mov esi,ecx ; mov ecx,edx`); the vector is built in a
// named 12-byte local from (X, Y, Z + 1.6f) and returned through the memberwise copy constructor.

class C3DVector
{ public:
    float X, Y, Z;
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) {}
    C3DVector(const C3DVector& other) : X(other.X), Y(other.Y), Z(other.Z) {}
};

class CThing
{ public:
    const C3DVector& GetPosition() const;           // 0x004C73D0
};

class CTCCameraPointDefinitionBase
{ public:
    static C3DVector __fastcall GetActiveCameraPosition(const CThing* thing);
};

C3DVector __fastcall CTCCameraPointDefinitionBase::GetActiveCameraPosition(const CThing* thing)
{
    const C3DVector& position = thing->GetPosition();
    C3DVector camera(position.X, position.Y, position.Z + 1.6f);
    return camera;
}