// CEngineClothSimulationMeshBase::ConvertVectorToLocal(const C3DVector& v) -- rotate a world-space direction into the cloth mesh's local space through the 3x3 part of InverseTransform (+0x78)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00B80AD0, length 105 bytes (+ int3 padding).
// Callees: none. Layout: the PDB CEngineClothSimulationMeshBase puts InverseTransform (CMatrix3x4, E11..E43) at +0x7c; retail CArray is 12 bytes
// (PDB 16), so ParticleSims +0x0c, CurrentTransform +0x18, LastTransform +0x48, InverseTransform +0x78.
// Shape: C3DVector returned through the hidden pointer (`ret 8`, eax = the result pointer); the three sums are computed X, Y, Z on the x87
// stack and stored at the end with `fxch st(2)` + three `fstp` -- a NAMED local built field by field and returned through C3DVector's
// member-wise user copy constructor (wave 24o). `fld [m] ; fmul [v]` is `v * m` (the right operand is loaded first). The term order
// (the Y product first in every row, then X/Z) only reproduces statement by statement (`x = a; x += b; x += c;`): a single
// three-term expression is re-associated by VC7.1 into one canonical order whatever the source order.
struct C3DVector {
    float X, Y, Z;
    C3DVector() {}
    C3DVector(const C3DVector& o) : X(o.X), Y(o.Y), Z(o.Z) {}
};

struct CMatrix3x4 {
    float E11, E12, E13;
    float E21, E22, E23;
    float E31, E32, E33;
    float E41, E42, E43;
};

struct CEngineClothSimulationMeshBase {
    unsigned char _pad00[0x78];           // vptr, ParticleSims, CurrentTransform, LastTransform
    CMatrix3x4    InverseTransform;       // +0x78 (PDB +0x7c)
    C3DVector ConvertVectorToLocal(const C3DVector& v) const;
};

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