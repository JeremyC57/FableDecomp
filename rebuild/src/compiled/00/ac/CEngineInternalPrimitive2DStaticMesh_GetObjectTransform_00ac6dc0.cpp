// CEngineInternalPrimitive2DStaticMesh::GetObjectTransform() const -- narrows a row-major 3x3 double matrix into a 3x4 float matrix returned by value (translation row zero)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00AC6DC0, length 117 bytes. No callees, no xrefs found (dead or reached indirectly).
// Shape: `this` is NOT the PDB CEngineInternalPrimitive2DStaticMesh (its +0 is a vptr; here ecx+0 is a double), so
// the object is modelled locally as nine doubles (row-major 3x3, rows at +0x00/+0x18/+0x30). The 48-byte result is
// built in a named local and copied with `rep movsd` (12 dwords) into the hidden result pointer (no NRVO in VC7.1,
// `ret 4` pops the pointer). The output reuses source cells: element 3 repeats m[1], element 6 repeats m[2] and
// element 7 repeats m[5], which is why retail keeps those loads on the x87 stack (fst/fxch) or copies the narrowed
// float through eax instead of loading again; element 8 takes m[7] (+0x38). The last three floats are zero
// immediates. Label: propagated manifest name; the real owner is a double-precision transform record.
struct CMatrix3x4 {
    float m[12];
};

struct CDoubleMatrix3x3 {
    double m[9];
    CMatrix3x4 GetObjectTransform() const;
};

CMatrix3x4 CDoubleMatrix3x3::GetObjectTransform() const
{
    CMatrix3x4 r;
    r.m[0] = (float)m[0];
    r.m[1] = (float)m[1];
    r.m[2] = (float)m[2];
    r.m[3] = (float)m[1];
    r.m[4] = (float)m[4];
    r.m[5] = (float)m[5];
    r.m[6] = (float)m[2];
    r.m[7] = (float)m[5];
    r.m[8] = (float)m[7];
    r.m[9] = 0.0f;
    r.m[10] = 0.0f;
    r.m[11] = 0.0f;
    return r;
}