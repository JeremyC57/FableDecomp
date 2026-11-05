// CTCCameraPointDefinitionBase::GetActiveCameraPosition(const CThing* thing) -- the camera position for a thing: its position raised by 1.6 units
// Plan phase 4 (2026-11-05), C3DVector wave: the local C3DVector stand-in replaced by the shared engine/C3DVector.h (PDB C3DVector TPI 0x2783: public X/Y/Z at 0/4/8, size 12).
//
// Retail 0x00878A10, 52 bytes. Callee: CThing::GetPos() const @ 0x004C73D0 (returns a const
// C3DVector&; manifest label "CVertexBufferWin32::DoSizeof" propagated). Constant 0x012777F0 = 1.6f.
// No direct callers (no E8/E9 reference): reached through a pointer.
// Shape: a static `__fastcall` returning the C3DVector (a class with user constructors) through the
// hidden pointer in ecx, the thing in edx (`mov esi,ecx ; mov ecx,edx`); the vector is built in a
// named 12-byte local from (X, Y, Z + 1.6f) and returned through the memberwise copy constructor.

#include "engine/C3DVector.h"

class CThing { public:
    const C3DVector& GetPos() const;           // 0x004C73D0
};

class CTCCameraPointDefinitionBase
{ public:
    static C3DVector __fastcall GetActiveCameraPosition(const CThing* thing);
};

C3DVector __fastcall CTCCameraPointDefinitionBase::GetActiveCameraPosition(const CThing* thing)
{
    const C3DVector& position = thing->GetPos();
    C3DVector camera(position.X, position.Y, position.Z + 1.6f);
    return camera;
}