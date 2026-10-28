// CEngineClothSimulationMeshBase::GetGravityVector() const -- the simulation manager's gravity scaled by the cloth gravity factor, rotated into the mesh's local space
// Leaf: ?GetGravityVector@CEngineClothSimulationMeshBase@@UBE?AVC3DVector@@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00B80F90, 69 bytes, thiscall, `ret 4` (C3DVector returned through the hidden pointer).
// Identity (proven): slot 1 of vtable 0x012A3924 (RTTI .?AVCEngineClothSimulationMeshBase@@); Ego_r.pdb
//   ?GetGravityVector@CEngineClothSimulationMeshBase@@UBE?AVC3DVector@@XZ (the catalog row already carries it).
// Globals: 0x01436E68 the CEngineSimulationManager (retail STLport layout, engine/CEngineSimulationManager.h):
//   ClothGravityFactor +0x1044, GravityVector +0x1048.
// Callees: CEngineClothSimulationMeshBase::ConvertVectorToLocal(const C3DVector&) const 0x00B80AD0 (Ego_r.pdb: protected, IBE).
// Shape: a named factor local (kept on the x87 stack) and a named C3DVector local of the three products X/Y/Z * factor,
//   passed by reference. Retail's `mov eax,edx` after the call reuses the hidden result pointer that VC7.1 knows the
//   callee leaves in edx: ConvertVectorToLocal's body sits in this TU (the landed byte-exact 0x00B80AD0 body, kept out of
//   line with auto_inline(off) as in retail).
#include "engine/CEngineSimulationManager.h"

class C3DVector {
public:
    float X, Y, Z;
    C3DVector() { }
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) { }
    C3DVector(const C3DVector& o) : X(o.X), Y(o.Y), Z(o.Z) { }
};

class CMatrix3x4 {
public:
    float E11, E12, E13;
    float E21, E22, E23;
    float E31, E32, E33;
    float E41, E42, E43;
};

extern CEngineSimulationManager* g_pEngineSimulationManager;     // 0x01436E68

class CEngineClothSimulationMeshBase {
public:
    virtual ~CEngineClothSimulationMeshBase();
    virtual C3DVector GetGravityVector() const;
    unsigned char _pad04[0x78 - 4];          // CEngineSimulationBase, ParticleSims, CurrentTransform, LastTransform
    CMatrix3x4    InverseTransform;          // +0x78 (PDB +0x7c)
protected:
    C3DVector ConvertVectorToLocal(const C3DVector& v) const;
};

#pragma auto_inline(off)
C3DVector CEngineClothSimulationMeshBase::ConvertVectorToLocal(const C3DVector& v) const
{
    float x = v.Y * InverseTransform.E21;
    x += v.Z * InverseTransform.E31;
    x += v.X * InverseTransform.E11;
    float y = v.Y * InverseTransform.E22;
    y += v.X * InverseTransform.E12;
    y += v.Z * InverseTransform.E32;
    float z = v.Y * InverseTransform.E23;
    z += v.X * InverseTransform.E13;
    z += v.Z * InverseTransform.E33;
    C3DVector local;
    local.X = x;
    local.Y = y;
    local.Z = z;
    return local;
}
#pragma auto_inline(on)

C3DVector CEngineClothSimulationMeshBase::GetGravityVector() const
{
    const CEngineSimulationManager* manager = g_pEngineSimulationManager;
    float factor = manager->ClothGravityFactor;
    C3DVector gravity(manager->GravityVector_X * factor,
                      manager->GravityVector_Y * factor,
                      manager->GravityVector_Z * factor);
    return ConvertVectorToLocal(gravity);
}