// CEngineClothSimulationMeshBase::GetWindVector() -- sample the simulation environment's wind at the mesh position, scale it by 100 and return it rotated into the mesh's local space
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00B80FE0, length 112 bytes (+ int3 padding).
// Callees: CSimulationEnvironment::GetWindAt(const C3DVector&) @ 0x00B80B70 (__thiscall on the global @ 0x01436E68, C3DVector by hidden
// pointer), CEngineClothSimulationMeshBase::ConvertVectorToLocal(const C3DVector&) @ 0x00B80AD0 (same TU). Float pool 0x01230A08 = 100.0f.
// Layout: CurrentTransform (CMatrix3x4) at retail +0x18 (PDB +0x1c, the CArray is 12 bytes in retail), so its position row E41..E43 is +0x3c.
// Shape: ONE 12-byte vector local: initialised from the position row through C3DVector's member-wise copy constructor (dword moves through
// eax/ecx/edx), passed by reference to GetWindAt, then overwritten with the scaled wind (X, Y, Z order) -- separate position/scaled locals
// cost an extra 12-byte frame slot. The scaling reads the returned wind through eax (`fld [eax]`): an inline member called on the returned
// temporary (`GetWindAt(v).ScaleInto(v, 100.0f)`); a named or reference-bound wind reads its frame slot instead; `x * 100.0f` loads x first.
// The closing `mov eax,edx` keeps the hidden result pointer in edx across the ConvertVectorToLocal call: VC7.1 knows that callee (defined
// ABOVE in the TU, kept out of line with `#pragma auto_inline(off)`) only clobbers eax/ecx. The mangled name is
// ?GetWindVector@CEngineClothSimulationMeshBase@@UBE?AVC3DVector@@XZ (public virtual const, C3DVector declared `class`).
class C3DVector {
public:
    float X, Y, Z;
    C3DVector() {}
    C3DVector(const C3DVector& o) : X(o.X), Y(o.Y), Z(o.Z) {}
    void ScaleInto(C3DVector& out, float s) const { out.X = X * s; out.Y = Y * s; out.Z = Z * s; }
};

class CMatrix3x4 {                      // rows: E11..E13, E21..E23, E31..E33, then the position E41..E43
public:
    C3DVector Row1;
    C3DVector Row2;
    C3DVector Row3;
    C3DVector Position;
};

class CSimulationEnvironment {                  // the object behind the global @ 0x01436E68
public:
    C3DVector GetWindAt(const C3DVector& position);   // 0x00B80B70
};

extern CSimulationEnvironment* g_pSimulationEnvironment;   // 0x01436E68

class CEngineClothSimulationMeshBase {
public:
    virtual void v00();
    unsigned char _pad04[0x14];             // +0x04: ParticleSims (12-byte CArray at +0x0c)
    CMatrix3x4    CurrentTransform;         // +0x18 (PDB +0x1c)
    CMatrix3x4    LastTransform;            // +0x48
    CMatrix3x4    InverseTransform;         // +0x78
    C3DVector ConvertVectorToLocal(const C3DVector& v) const;   // 0x00B80AD0
    virtual C3DVector GetWindVector() const;
};

#pragma auto_inline(off)
C3DVector CEngineClothSimulationMeshBase::ConvertVectorToLocal(const C3DVector& v) const
{
    float x = v.Y * InverseTransform.Row2.X;
    x += v.Z * InverseTransform.Row3.X;
    x += v.X * InverseTransform.Row1.X;
    float y = v.Y * InverseTransform.Row2.Y;
    y += v.X * InverseTransform.Row1.Y;
    y += v.Z * InverseTransform.Row3.Y;
    float z = v.Y * InverseTransform.Row2.Z;
    z += v.X * InverseTransform.Row1.Z;
    z += v.Z * InverseTransform.Row3.Z;
    C3DVector local;
    local.X = x;
    local.Y = y;
    local.Z = z;
    return local;
}
#pragma auto_inline(on)

C3DVector CEngineClothSimulationMeshBase::GetWindVector() const
{
    C3DVector v(CurrentTransform.Position);
    g_pSimulationEnvironment->GetWindAt(v).ScaleInto(v, 100.0f);
    return ConvertVectorToLocal(v);
}
