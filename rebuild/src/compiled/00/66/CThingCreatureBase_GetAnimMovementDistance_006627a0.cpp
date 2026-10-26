// CThingCreatureBase::GetAnimMovementDistance(long animIndex) const -- for a non-zero animation index, the length of the animation's movement vector scaled by 0.01; 0 otherwise
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006627A0, length 80 bytes.
// Leaf: GetAnimMovementDistance
// Callees: CThing::PeekWorld @ 0x004C7AB0 (`[g_pMainGameComponent]+0x24`; the manifest's "CViewBase::GetDisplayEngine" is
// propagated), the bank's GetAnimation(long) @ 0x00A242C0 (`[this+380h][index]`, landed callers name it; the world's
// pointer at +0x3c is the main mesh bank, engine/CWorld.h `PMainMeshBank_Data`). Float pool: 0x01230A5C = 0.01f,
// 0x0122DEDC = 0.0f. Ego_r.pdb: `?GetAnimMovementDistance@CThingCreatureBase@@QBEMJ@Z`; C3DAnimationInfo::MovementVector +0x18.
// The chunk label "CTCAnimationComplex::GetAnimDuration" is propagated and rejected (the neighbours are CThingCreatureBase's).
// Shape: the three components are loaded Z, Y, X onto the x87 and each squared from its register (`fld st(i) ; fmul
// st(i+1)`), then the stack is cleaned with `fstp st(3)` + two `fstp st(0)`: the magnitude is an inline helper taking the
// components BY VALUE (arguments evaluated right to left). A reference-based `X*X + ...` re-reads memory; a vector copy
// (POD) goes through the frame, a memberwise copy constructor loads X first. The bank pointer is a named local so the
// world call precedes the index push.
#include <math.h>

inline float Magnitude(float x, float y, float z) { return (float)sqrt(x * x + y * y + z * z); }

class C3DVector { public:
    float X, Y, Z;
    float Length() const { return Magnitude(X, Y, Z); }
};

class C3DAnimationInfo { public:
    unsigned char _pad[0x10];
    float     Duration;                        // +0x10
    float     NonLoopingDuration;              // +0x14
    C3DVector MovementVector;                  // +0x18
    float     Rotation;                        // +0x24
};

class CMeshDataBank { public:
    C3DAnimationInfo* GetAnimation(long index);   // 0x00A242C0
};

class CWorld { public:
    unsigned char _pad[0x3c];
    CMeshDataBank* PMainMeshBank;              // +0x3c
};

class CThing { public:
    const CWorld& PeekWorld() const;           // 0x004C7AB0
};

class CThingCreatureBase : public CThing { public:
    float GetAnimMovementDistance(long animIndex) const;
};

float CThingCreatureBase::GetAnimMovementDistance(long animIndex) const
{
    if (animIndex) {
        CMeshDataBank* bank = PeekWorld().PMainMeshBank;
        return bank->GetAnimation(animIndex)->MovementVector.Length() * 0.01f;
    }
    return 0.0f;
}