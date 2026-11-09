// CEngineInternalPrimitiveDecal::GetBoundingSphereWorldSpace(C3DBoundingSphere& sphere) -- a sphere centred on the decal's world position with the length of its 2D size as radius; always true
// Leaf: ?GetBoundingSphereWorldSpace@CEngineInternalPrimitiveDecal@@UAE_NAAVC3DBoundingSphere@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00BBA3E0, 87 bytes, thiscall, `ret 4`.
// Identity (proven): slot 18 of vtable 0x012A5AE4 (RTTI .?AVCEngineInternalPrimitiveDecal@@), the GetBoundingSphereWorldSpace
//   slot of the landed CEngineInternalPrimitiveSprite 0x00BACF00 (vtable 0x012A5444 slot 18); Ego_r.pdb
//   ?GetBoundingSphereWorldSpace@CEngineInternalPrimitiveDecal@@UAE_NAAVC3DBoundingSphere@@@Z.
// Layout (Ego_r.pdb, retail offsets equal: CEngineInternalPrimitiveBase is 0x48): WorldPos C3DVector +0x54, Size C2DVector
//   +0x60 (X), +0x64 (Y).
// Callees: none (fsqrt inline via sqrtf).
// Shape: the landed Sprite model: `sphere = C3DBoundingSphere(WorldPos, sqrtf((x * x) + (y * y)));` with C3DVector's memberwise
//   user copy constructor (the centre copied into the 16-byte frame temporary, then the implicit operator= copies four words out).
//   The grouping parentheses keep two no-code PAREN tuples that c2's list scheduler counts (MATCHING_PLAYBOOK): they move
//   `faddp` ahead of `mov [esp+8],eax` (retail 0x00BBA408) and `fsqrt` ahead of `mov [ecx],edx` (0x00BBA412); without them
//   both issue one slot later (the 2026-10-27 park).
#include <math.h>
#define FABLE_RETAIL_C3DBOUNDINGSPHERE
#include "engine/C3DBoundingSphere.h"
#include "engine/C2DVector.h"

class CEngineInternalPrimitiveDecal {
public:
    virtual bool GetBoundingSphereWorldSpace(C3DBoundingSphere& sphere);
    unsigned char _pad04[0x48 - 4];                  // CEngineInternalPrimitiveBase (not touched here)
    bool LandscapeOnlyDecal;                         // +0x48
    long DecalIndex;                                 // +0x4C
    long DecalFrame;                                 // +0x50
    C3DVector WorldPos;                              // +0x54
    C2DVector Size;                                  // +0x60
};

bool CEngineInternalPrimitiveDecal::GetBoundingSphereWorldSpace(C3DBoundingSphere& sphere)
{
    float x = Size.X;
    float y = Size.Y;
    sphere = C3DBoundingSphere(WorldPos, sqrtf((x * x) + (y * y)));
    return true;
}