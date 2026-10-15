// CTCPhysicsBase::ConstructFromParams(const CTCPhysicsBaseParams& params) -- start both OldPosition and Position at the creation position
// Retail: Fable.exe (TLC, VC7.1) @ 0x006AF700, 54 bytes, CTCPhysicsBase vtable slot 3. No callees.
// Layout: engine/CTCPhysicsBase.h (Position +0x0c, OldPosition +0x18). The creation-parameter record carries the start
//   position at +4 (behind one leading word), read through one reference (`add eax,4`).
// Shape: two whole-vector assignments from the same reference, OldPosition first (`lea edx,[ecx+18h]` copy through esi/edi,
//   then the Position copy through `add ecx,0Ch`).
// Leaf: ConstructFromParams
#include "engine/CTCPhysicsBase.h"

struct C3DVector {
    float X, Y, Z;
};

struct CTCPhysicsBaseParams {
    long      m_header;                     // +0x00 (leading word, not read here)
    C3DVector Position;                     // +0x04
};

struct CTCPhysicsBase_Methods : CTCPhysicsBase {
    C3DVector& PositionVector()    { return *reinterpret_cast<C3DVector*>(&Position_X); }
    C3DVector& OldPositionVector() { return *reinterpret_cast<C3DVector*>(&OldPosition_X); }
    void ConstructFromParams(const CTCPhysicsBaseParams& params);
};

void CTCPhysicsBase_Methods::ConstructFromParams(const CTCPhysicsBaseParams& params)
{
    const C3DVector& position = params.Position;
    OldPositionVector() = position;
    PositionVector() = position;
}
