// NGraphicAppearance::CTCGraphicAppearance::GetRenderMeshObject(C3DMeshObject& out, float interpolation) const -- for a mesh main graphic (type 4 or 5) build its render mesh object at the thing's interpolated render placement; false for any other graphic
// Leaf: ?GetRenderMeshObject@CTCGraphicAppearance@NGraphicAppearance@@QBE_NAAVC3DMeshObject@@M@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x004BC670, 214 bytes, thiscall, `ret 8`.
// Identity (proven): Ego_r.pdb has exactly two GetRenderMeshObject overloads on CTCGraphicAppearance; the
//   (C3DMeshObject&, const C3DVector&, const CRightHandedSet&) one is the landed neighbour 0x004BC750 (`ret 0Ch`), this is
//   the (C3DMeshObject&, float) one (`ret 8`, the float compared with 1.0f and handed to the position/orientation getters).
// Callees: CTCBase::PeekWorld() const 0x00686A80, GetRenderPositionForThing 0x004BBF00 (file-static, landed),
//   CMeshDataBank::GetMeshInfo 0x00A24270, the physics TC's GetRHSet() (+0x120) / GetRHSet(float) (+0x11C) virtuals,
//   C3DMeshObject ctor 0x00A36E70 / operator= 0x004C3380 / ~C3DMeshObject 0x004C3200 (out of line here).
// Shape: the landed GetMeshObject 0x004BC4F0 body (two assignments split on `interpolation == 1.0f`, whose common tail VC7.1
//   merges) inside a positive `Type && (Type == 5 || Type == 4)` test (retail tests the zero type first). A single
//   assignment with a ?: orientation argument evaluates the conditional before the scale (DIFFER 204v214).
// Layout: retail CTCGraphicAppearance: ParentThing +0x04, Scale +0x34, MainGraphic +0x44 (BankIndex +0x44, RenderSizeX
//   +0x4C, Type +0x51); retail CThing::PhysicsTC +0x60; retail CWorld::PMainMeshBank +0x3C.
class C3DVector {
public:
    float X, Y, Z;
    C3DVector() {}
    C3DVector(const C3DVector& o) : X(o.X), Y(o.Y), Z(o.Z) {}
};
class CRightHandedSet {
public:
    C3DVector Up;                                           // +0x00
    C3DVector Forward;                                      // +0x0c
};
class C3DMeshInfo;

#include "engine/CBaseClass.h"                         // CBaseClass() 0x0099A2D0, ~CBaseClass 0x0099A2E0
class C3DMeshObject : public CBaseClass {                   // vtable 0x01239860, 0x78 bytes
public:
    unsigned char _body[0x78 - 0x4];
    C3DMeshObject(const C3DMeshInfo* info, const C3DVector& position, const CRightHandedSet& orientation, float scale);   // 0x00A36E70
    C3DMeshObject& operator=(const C3DMeshObject& other);   // 0x004C3380
    virtual ~C3DMeshObject();                               // 0x004C3200
};

class CMeshDataBank {
public:
    const C3DMeshInfo* GetMeshInfo(long index);             // 0x00A24270
};
class CWorld {
public:
    unsigned char _pad00[0x3c];
    CMeshDataBank* PMainMeshBank;                           // +0x3C
};

#define VSLOT(n) virtual void vslot##n();
#define VSLOT10(n) VSLOT(n##0) VSLOT(n##1) VSLOT(n##2) VSLOT(n##3) VSLOT(n##4) VSLOT(n##5) VSLOT(n##6) VSLOT(n##7) VSLOT(n##8) VSLOT(n##9)
class CTCPhysicsBase {
public:
    VSLOT10(0) VSLOT10(1) VSLOT10(2) VSLOT10(3) VSLOT10(4) VSLOT10(5) VSLOT10(6)
    VSLOT(70)
    // MSVC lays an overload set out in reverse declaration order: GetRHSet(float) lands in slot 71, GetRHSet() in 72
    virtual const CRightHandedSet& GetRHSet() const;              // slot 72 (+0x120)
    virtual CRightHandedSet GetRHSet(float interpolation) const;   // slot 71 (+0x11C)
};
class CThing {
public:
    unsigned char   _pad00[0x60];
    CTCPhysicsBase* PhysicsTC;                              // +0x60 (retail; PDB +0x70)
};

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
protected:
    const CWorld& PeekWorld() const;                        // 0x00686A80
};

namespace NGraphicAppearance {
C3DVector __fastcall GetRenderPositionForThing(const CThing& thing, float interpolation);   // 0x004BBF00 (file-static)

class CTCGraphicAppearance : public CTCBase {
public:
    unsigned char  _pad08[0x34 - 0x08];
    float          Scale;                                   // +0x34
    unsigned char  _pad38[0x44 - 0x38];
    CEngineGraphic MainGraphic;                             // +0x44
    bool GetRenderMeshObject(C3DMeshObject& out, float interpolation) const;
};

bool CTCGraphicAppearance::GetRenderMeshObject(C3DMeshObject& out, float interpolation) const
{
    if (MainGraphic.Type != 0 && (MainGraphic.Type == 5 || MainGraphic.Type == 4)) {
        CMeshDataBank* bank = PeekWorld().PMainMeshBank;
        const CTCPhysicsBase* physics = ParentThing->PhysicsTC;
        C3DVector position = GetRenderPositionForThing(*ParentThing, interpolation);
        const C3DMeshInfo* meshInfo = bank->GetMeshInfo(MainGraphic.BankIndex);
        if (interpolation == 1.0f)
            out = C3DMeshObject(meshInfo, position, physics->GetRHSet(), MainGraphic.RenderSizeX * Scale * 0.01f);
        else
            out = C3DMeshObject(meshInfo, position, physics->GetRHSet(interpolation), MainGraphic.RenderSizeX * Scale * 0.01f);
        return true;
    }
    return false;
}

}