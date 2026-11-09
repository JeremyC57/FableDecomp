// NGraphicAppearance::CTCGraphicAppearance::GetPositionMatrix(float interpolation) const -- the owner's render placement at the interpolation point (render position and physics orientation) as a mesh position matrix scaled like the main graphic
// Leaf: ?GetPositionMatrix@CTCGraphicAppearance@NGraphicAppearance@@QBE?AVCMatrix3x4@@M@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004BC800, length 217 bytes. __fastcall, `ret 8` (hidden CMatrix3x4 result).
//         Ego_r.pdb: `?GetPositionMatrix@CTCGraphicAppearance@NGraphicAppearance@@QBE?AVCMatrix3x4@@M@Z`.
// Callees: CBaseClass ctor/dtor @ 0x0099A2D0/0x0099A2E0, the physics component's GetRHSet(float) const (slot 71, +0x11C,
//          landed 0x006B1450, the orientation by hidden pointer), the file-static `__fastcall` GetRenderPositionForThing(const
//          CThing&, float) @ 0x004BBF00 (landed; C3DVector by hidden pointer in ecx, the thing in edx),
//          C3DMeshObject::SetPositionMatrix(const C3DVector&, const CRightHandedSet&, float) @ 0x00A357B0, operator delete
//          @ 0x00BFE9BC (the inline counted-pointer release). Float pool 0x01230A5C = 0.01f.
// Shape: a default C3DMeshObject local (inline ctor: CBaseClass, vtable 0x01239860, null mesh pointer, the inverse-matrix
//        flag cleared; one ebx zero) is placed with the physics orientation, the render position and (RenderSizeX *
//        Scale) * 0.01f, its PositionMatrix is block-copied into the result (`rep movsd`) and the local destroyed inline;
//        the physics component is a named local read before the mesh object is built, the owner a reference
//        read after it (kept in ebp).
// Scheduling (chunk 892): the parenthesised (RenderSizeX * Scale) keeps a no-code PAREN tuple between the two fmuls; it
//        takes the int slot of the `xor ebx,ebx` cycle, so `xor; lea; fstp; push esi` leave the mesh object's vtable
//        store (retail 0x004BC83F `mov [esp+18h],01239860h`) below the orientation call's `push esi`, as retail.
// Layout: retail CThing PhysicsTC +0x60 (shared engine/CThing.h); retail CTCGraphicAppearance Scale +0x34,
//         MainGraphic.RenderSizeX +0x4C (local, as the landed same-TU 0x004BC670: the shared header keeps the
//         members opaque -- header debt); C3DMeshObject (Ego_r layout, no shared header): PositionMatrix +0x10,
//         InversePositionMatrixSetup +0x40.
#include "rebuild_abi.h"
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CCLASS
#include "engine/C3DVector.h"
#include "engine/CRightHandedSet.h"
#include "engine/CMatrix3x4.h"
#include "engine/CBaseClass.h"                              // CBaseClass() 0x0099A2D0, ~CBaseClass 0x0099A2E0
#include "engine/CCountedPointer.h"
class C3DMeshInfo;
class C3DMeshStats;

class C3DMeshObject : public CBaseClass {                   // vtable 0x01239860, 0x78 bytes (Ego_r.pdb)
public:
    C3DMeshInfo*                         MeshInfo;          // +0x04
    CCountedPointer<const C3DMeshStats>  Mesh;              // +0x08
    CMatrix3x4                           PositionMatrix;    // +0x10
    bool                                 InversePositionMatrixSetup;   // +0x40
    CMatrix3x4                           InversePositionMatrix;        // +0x44
    float                                Scale;             // +0x74
    C3DMeshObject() : InversePositionMatrixSetup(false) {}
    void SetPositionMatrix(const C3DVector& position, const CRightHandedSet& orientation, float scale);   // 0x00A357B0
    virtual ~C3DMeshObject() {}
};

#define VSLOT(n) virtual void vslot##n();
#define VSLOT10(n) VSLOT(n##0) VSLOT(n##1) VSLOT(n##2) VSLOT(n##3) VSLOT(n##4) VSLOT(n##5) VSLOT(n##6) VSLOT(n##7) VSLOT(n##8) VSLOT(n##9)
class CTCPhysicsBase {
public:
    VSLOT10(0) VSLOT10(1) VSLOT10(2) VSLOT10(3) VSLOT10(4) VSLOT10(5) VSLOT10(6)
    VSLOT(70)
    // MSVC lays an overload set out in reverse declaration order: GetRHSet(float) lands in slot 71, GetRHSet() in 72
    virtual const CRightHandedSet& GetRHSet() const;              // slot 72 (+0x120)
    virtual CRightHandedSet GetRHSet(float interpolation) const;   // slot 71 (+0x11C, 0x006B1450)
};
#include "engine/CThing.h"

#pragma pack(push, 2)
class CEngineGraphic {                                      // 14 bytes
public:
    unsigned long BankIndex;                                // +0x00
    float AnimStep;                                         // +0x04
    float RenderSizeX;                                      // +0x08
    bool AdditiveAlpha;                                     // +0x0C
    unsigned char Type;                                     // +0x0D
};
#pragma pack(pop)

class CTCBase {
public:
    void*   __vfptr;                                        // +0x00
    CThing* ParentThing;                                    // +0x04
};

namespace NGraphicAppearance {
C3DVector FABLE_FASTCALL GetRenderPositionForThing(const CThing& thing, float interpolation);   // 0x004BBF00 (file-static)

class CTCGraphicAppearance : public CTCBase {
public:
    unsigned char  _pad08[0x34 - 0x08];
    float          Scale;                                   // +0x34
    unsigned char  _pad38[0x44 - 0x38];
    CEngineGraphic MainGraphic;                             // +0x44
    CMatrix3x4 GetPositionMatrix(float interpolation) const;
};

CMatrix3x4 CTCGraphicAppearance::GetPositionMatrix(float interpolation) const
{
    const CTCPhysicsBase* physics = ParentThing->GetPhysicsTC();
    C3DMeshObject meshObject;
    const CThing& thing = *ParentThing;
    meshObject.SetPositionMatrix(GetRenderPositionForThing(thing, interpolation),
                                 physics->GetRHSet(interpolation),
                                 (MainGraphic.RenderSizeX * Scale) * 0.01f);   // parenthesised: retail 0x004BC83F order
    return meshObject.PositionMatrix;
}

}