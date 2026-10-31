// Grade: DIFFER(217v217) sec1b: header move only -- engine/CBaseClass.h + engine/CCountedPointer.h<const C3DMeshStats> replace the local models, bytes unchanged (the C3DMeshObject vtable store one push early). Tried: init-list vs body store, forced ctor, hoisted scale local. Earlier: DIFFER(217v217) -- scheduling only: the mesh object's vtable store is emitted one push earlier than retail (retail sinks it below the `push esi` of the orientation call with the member zero stores)
// NGraphicAppearance::CTCGraphicAppearance::GetPositionMatrix(float interpolation) const -- the owner's render placement at the interpolation point (render position and physics orientation) as a mesh position matrix scaled like the main graphic
// Leaf: GetPositionMatrix
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004BC800, length 217 bytes. __thiscall, `ret 8` (hidden CMatrix3x4 result).
//         Ego_r.pdb: `?GetPositionMatrix@CTCGraphicAppearance@NGraphicAppearance@@QBE?AVCMatrix3x4@@M@Z`.
// Callees: CBaseClass ctor/dtor @ 0x0099A2D0/0x0099A2E0, the physics component's virtual slot 71 (+0x11C, the interpolated
//          orientation by hidden pointer), the file-static `__fastcall` GetRenderPositionForThing(const CThing&, float) @
//          0x004BBF00 (C3DVector by hidden pointer in ecx, the thing in edx), C3DMeshObject::SetPositionMatrix(const
//          C3DVector&, const CRightHandedSet&, float) @ 0x00A357B0, operator delete @ 0x00BFE9BC. Float pool 0x01230A5C = 0.01f.
// Shape: a default C3DMeshObject local (inline ctor: CBaseClass, vtable 0x01239860, null mesh pointer, the inverse-matrix
//        flag cleared; one ebx zero) is placed with the physics orientation, the render position and RenderSizeX *
//        Scale * 0.01f, its PositionMatrix is block-copied into the result (`rep movsd`) and the local destroyed inline;
//        the physics component is a named local read before the mesh object is built, the owner a reference
//        read after it (kept in ebp).
// Layout: retail CThing PhysicsTC +0x60; retail CTCGraphicAppearance Scale +0x34, MainGraphic.RenderSizeX +0x4C;
//         C3DMeshObject (landed 0x00A36E70 model): PositionMatrix +0x10, InversePositionMatrixSetup +0x40.
void __cdecl operator delete(void* p);                      // 0x00BFE9BC

struct C3DVector {
    float X, Y, Z;
    C3DVector() {}
};
struct CRightHandedSet { C3DVector Up, Forward; };
struct CMatrix3x4 { float m[12]; };
class CThing;
struct C3DMeshInfo;

#include "engine/CBaseClass.h"
#include "engine/CCountedPointer.h"
class C3DMeshStats;

class C3DMeshObject : public CBaseClass {
public:                         // vtable 0x01239860, 0x78 bytes
    const C3DMeshInfo* MeshInfo;                            // +0x4
    CCountedPointer<const C3DMeshStats> Mesh;               // +0x8
    CMatrix3x4         PositionMatrix;                      // +0x10
    bool               InversePositionMatrixSetup;          // +0x40
    CMatrix3x4         InversePositionMatrix;               // +0x44
    float              Scale;                               // +0x74
    C3DMeshObject() : CBaseClass(), Mesh(), InversePositionMatrixSetup(false) {}
    void SetPositionMatrix(const C3DVector& position, const CRightHandedSet& orientation, float scale);   // 0x00A357B0
    virtual ~C3DMeshObject();
};

C3DMeshObject::~C3DMeshObject()
{
}

struct CTCPhysicsBase {
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05();
    virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11();
    virtual void v12(); virtual void v13(); virtual void v14(); virtual void v15(); virtual void v16(); virtual void v17();
    virtual void v18(); virtual void v19(); virtual void v20(); virtual void v21(); virtual void v22(); virtual void v23();
    virtual void v24(); virtual void v25(); virtual void v26(); virtual void v27(); virtual void v28(); virtual void v29();
    virtual void v30(); virtual void v31(); virtual void v32(); virtual void v33(); virtual void v34(); virtual void v35();
    virtual void v36(); virtual void v37(); virtual void v38(); virtual void v39(); virtual void v40(); virtual void v41();
    virtual void v42(); virtual void v43(); virtual void v44(); virtual void v45(); virtual void v46(); virtual void v47();
    virtual void v48(); virtual void v49(); virtual void v50(); virtual void v51(); virtual void v52(); virtual void v53();
    virtual void v54(); virtual void v55(); virtual void v56(); virtual void v57(); virtual void v58(); virtual void v59();
    virtual void v60(); virtual void v61(); virtual void v62(); virtual void v63(); virtual void v64(); virtual void v65();
    virtual void v66(); virtual void v67(); virtual void v68(); virtual void v69(); virtual void v70();
    virtual CRightHandedSet GetInterpolatedOrientation(float interpolation) const;   // slot 71 (+0x11C)
};

class CThing {
public:
    unsigned char _pad00[0x60];
    CTCPhysicsBase* PhysicsTC;                              // +0x60
};

C3DVector __fastcall GetRenderPositionForThing(const CThing& thing, float interpolation);   // 0x004BBF00 (file-static)

#pragma pack(push, 2)
struct CEngineGraphic {                                     // 14 bytes
    unsigned long BankIndex;                                // +0x00
    float AnimStep;                                         // +0x04
    float RenderSizeX;                                      // +0x08
    bool AdditiveAlpha;                                     // +0x0C
    unsigned char Type;                                     // +0x0D
};
#pragma pack(pop)

namespace NGraphicAppearance {

struct CTCGraphicAppearance {
    void*          __vfptr;                                 // +0x00
    CThing*        ParentThing;                             // +0x04 (CTCBase)
    unsigned char  _pad08[0x34 - 0x08];
    float          Scale;                                   // +0x34
    unsigned char  _pad38[0x44 - 0x38];
    CEngineGraphic MainGraphic;                             // +0x44
    CMatrix3x4 GetPositionMatrix(float interpolation) const;
};

CMatrix3x4 CTCGraphicAppearance::GetPositionMatrix(float interpolation) const
{
    CTCPhysicsBase* physics = ParentThing->PhysicsTC;
    C3DMeshObject meshObject;
    const CThing& thing = *ParentThing;
    meshObject.SetPositionMatrix(GetRenderPositionForThing(thing, interpolation),
                                 physics->GetInterpolatedOrientation(interpolation),
                                 MainGraphic.RenderSizeX * Scale * 0.01f);
    return meshObject.PositionMatrix;
}

}
