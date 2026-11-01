// C3DMeshObject::~C3DMeshObject() -- release the counted Mesh (C3DMeshStats) pointer, then the CBaseClass base destructor
// Leaf: ??1C3DMeshObject@@UAE@XZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004C3200, 66 bytes, thiscall, tail `jmp ~CBaseClass`.
// Identity (proven): stores vtable 0x01239860 first -- RTTI .?AVC3DMeshObject@@ (rtti-vtables.tsv; slot 0 `??_G` 0x004C3250
//   expands the same body inline); the 7 callers destroy C3DMeshObject locals (CTCGraphicAppearance::GetRenderMeshObject
//   0x004BC670 landed binding ??1C3DMeshObject@@UAE@XZ, GetHelperPoint 0x004C0560, GetDummyObject 0x004C08C0, CThingFilter
//   0x006A7C60, CTCWeaponTrail::CalculateInterpolatedWeaponHelpers 0x0076E2F0). Ego_r.pdb and FableWin.pdb both spell
//   ??1C3DMeshObject@@UAE@XZ. Ego_r members: MeshInfo +4, Mesh CCountedPointer<const C3DMeshStats> +8, PositionMatrix +0x10,
//   InversePositionMatrixSetup +0x40, InversePositionMatrix +0x44, Scale +0x74 (0x78 bytes). The catalog label
//   "~C3DClothPrimitive" is propagated from the masked twin 0x00AC3B80.
// Callees: operator delete 0x00BFE9BC, the info block's DeleteFunc through +4, CBaseClass::~CBaseClass 0x0099A2E0.
// Shape: empty user destructor; the Mesh member's inline CCountedPointer dtor (engine/CCountedPointer.h). The matrices are
//   trivially destructible.
// Layout: local C3DMeshObject (no engine header exists) over engine/CBaseClass.h; CMatrix3x4 is an opaque local (its
//   union members are not modelled; trivially destructible).
void __cdecl operator delete(void* block);              // 0x00BFE9BC
#include "engine/CCountedPointer.h"
#include "engine/CBaseClass.h"

class C3DMeshInfo;
class C3DMeshStats;

class CMatrix3x4 {                                      // Ego_r: 48 bytes, union of E11..E43 and row vectors
public:
    float E11, E12, E13, E21, E22, E23, E31, E32, E33, E41, E42, E43;
};

class C3DMeshObject : public CBaseClass {                // vtable 0x01239860, 0x78 bytes
public:
    C3DMeshObject();
    virtual ~C3DMeshObject();
    C3DMeshInfo* MeshInfo;                               // +0x04
    CCountedPointer<const C3DMeshStats> Mesh;            // +0x08
    CMatrix3x4 PositionMatrix;                           // +0x10
    bool InversePositionMatrixSetup;                     // +0x40
    CMatrix3x4 InversePositionMatrix;                    // +0x44
    float Scale;                                         // +0x74
};

C3DMeshObject::~C3DMeshObject()
{
}