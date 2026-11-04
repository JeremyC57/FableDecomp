// CEngineInternalPrimitiveLine::GetBoundingSphereWorldSpace(C3DBoundingSphere& sphere) -- sphere around the line: centre = (start + end) * 0.5, radius = |centre - start|; always true
// Leaf: ?GetBoundingSphereWorldSpace@CEngineInternalPrimitiveLine@@UAE_NAAVC3DBoundingSphere@@@Z
// Flags: none
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00BC7DB0, 154 bytes, thiscall `ret 4`. Constant 0.5f @ 0x0122F59C; fsqrt inline.
// Identity: Ego_r.pdb ?GetBoundingSphereWorldSpace@CEngineInternalPrimitiveLine@@UAE_NAAVC3DBoundingSphere@@@Z (virtual); the
//   line's Start / End (Ego_r.pdb names) at +0x48 / +0x54 after the 0x48-byte CEngineInternalPrimitiveBase (the landed sibling
//   GetWorldPosition 0x00BC7D40 models the polymorphic class locally the same way: the base header carries its vptr as data).
// Shape: shared C3DVector (FABLE_RETAIL_C3DVECTOR: memberwise copy ctor, implicit operator=, __forceinline operators,
//   GetMagnitude) and C3DBoundingSphere. The centre is assigned through the implicit operator= (the scaled sum built in the
//   frame, copied through esi); the radius measures a by-value copy of the stored centre (`C3DVector(sphere.Center)`: X/Y
//   loaded on the x87 stack, Z copied through edx into a frame word), which is what fixes the old draft's register-only form.
#define FABLE_RETAIL_C3DVECTOR
#include "engine/C3DVector.h"
#include "engine/C3DBoundingSphere.h"
class CEngineInternalPrimitiveLine {
public:
    virtual void v000();
    unsigned char _base04[0x44];                   // +0x04 CEngineInternalPrimitiveBase (engine/CEngineInternalPrimitiveBase.h, vptr as data)
    C3DVector     Start;                           // +0x48
    C3DVector     End;                             // +0x54
    virtual bool GetBoundingSphereWorldSpace(C3DBoundingSphere& sphere);
};
bool CEngineInternalPrimitiveLine::GetBoundingSphereWorldSpace(C3DBoundingSphere& sphere)
{
    sphere.Center = (Start + End) * 0.5f;
    sphere.Radius = (C3DVector(sphere.Center) - Start).GetMagnitude();
    return true;
}