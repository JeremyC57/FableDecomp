// Grade: DIFFER(617v616) -- unrolled-loop output pointer biased +0x38 (retail +0x2c); every instruction otherwise identical modulo the displacements
// C3DPrimitive2::ConvertToVBFormatRepeatingMesh(CTVertexMatrixIndexedNormTex1*) -- write the device-independent vertices once per mesh repetition, tagging each copy with its repetition index as the matrix index
// Retail: Fable.exe (TLC, VC7.1) @ 0x00A8F770, 624 bytes (ret 4; the inner loop is unrolled four times by the compiler).
// Layout: retail C3DPrimitive2 Data at +0x2c (Ego_r.pdb +0x30 less 4: the retail ClothPrimitives CArray is 12 bytes),
// RepeatingMeshReps +8; N3DPrimitive::CDeviceIndependent::Vertices at +4 (C3DVertex2, 0x20 bytes);
// CTVertexMatrixIndexedNormTex1 0x24 bytes (Ego_r.pdb).
struct C3DVector {
    float X, Y, Z;
    C3DVector(const C3DVector& o) : X(o.X), Y(o.Y), Z(o.Z) { }
    C3DVector& operator=(C3DVector o) { X = o.X; Y = o.Y; Z = o.Z; return *this; }
};
struct C3DVertex2 {
    C3DVector Position;                         // +0x00
    C3DVector Normal;                           // +0x0c
    float U;                                    // +0x18
    float V;                                    // +0x1c
};
template <class T> struct CArray {
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
    long size() const { return long(_M_finish - _M_start); }
    const T& operator[](unsigned long i) const { return _M_start[i]; }
};
namespace N3DPrimitive {
struct CDeviceIndependent {
    void* _vptr;
    CArray<C3DVertex2> Vertices;                // +0x04
};
}
struct CTVertexMatrixIndexedNormTex1 {
    C3DVector Position;                         // +0x00
    C3DVector Normal;                           // +0x0c
    float U;                                    // +0x18
    float V;                                    // +0x1c
    short MatrixIndex;                          // +0x20
};
struct C3DPrimitive2 {
    void* _vptr;
    long MaterialIndex;                         // +0x04
    long RepeatingMeshReps;                     // +0x08
    unsigned char _pad0c[0x2c - 0x0c];
    N3DPrimitive::CDeviceIndependent* Data;     // +0x2c
    void ConvertToVBFormatRepeatingMesh(CTVertexMatrixIndexedNormTex1* out);
};

void C3DPrimitive2::ConvertToVBFormatRepeatingMesh(CTVertexMatrixIndexedNormTex1* out)
{
    long count = Data->Vertices.size();
    for (long rep = 0; rep < RepeatingMeshReps; ++rep) {
        for (unsigned long i = 0; i < count; ++i) {
            const C3DVertex2& src = Data->Vertices[i];
            CTVertexMatrixIndexedNormTex1& vertex = out[rep * count + i];
            vertex.Position = src.Position;
            vertex.Normal = src.Normal;
            vertex.U = src.U;
            vertex.V = src.V;
            vertex.MatrixIndex = (short)rep;
        }
    }
}
