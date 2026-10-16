// Grade: DIFFER(187v193) -- x87 order: retail computes the X row first and each row as (z*Z + y*Y) + x*X + P, keeping the helper point's x on the FPU (fst) and copying y twice; ours evaluates the C3DVector(x,y,z) arguments Z first. tried: rows as named float locals with the terms reordered; member-by-member result (Transform emitted out of line). lane gap: x87 position-load term order
// C3DMeshObject::GetHelperPointPositionStatic(const CCharString&, long) const -- the named helper point (by name CRC and index) transformed by PositionMatrix
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A361A0, length 193 bytes (`ret 0Ch`: hidden result, name, index).
// MESH LAYER: Ego_r.pdb ?GetHelperPointPositionStatic@C3DMeshObject@@QBE?AVC3DVector@@ABVCCharString@@J@Z.
// Callees: Calc @ 0x004014A0 (`__fastcall(0, chars, length)` CRC; the empty literal 0x0129AAF4 for a null string),
// C3DMeshStats::GetHelperPoint(unsigned long crc, long index) @ 0x00A2E8C0 (catalog "GetDummyObject"; returns the point).
// Layout (PDB, agrees with retail): C3DMeshObject Mesh (CCountedPointer<const C3DMeshStats>) +8, PositionMatrix (CMatrix3x4)
// +0x10 (XAxis +0x10, YAxis +0x1c, ZAxis +0x28, Position +0x34).
// Shape: the point copied into a local, then the inline matrix transform `x*X + y*Y + z*Z + P` per row into the
// C3DVector(x, y, z) result.
// Leaf: GetHelperPointPositionStatic@C3DMeshObject
extern const char g_emptyLiteral_0129aaf4[];       // 0x0129AAF4 ""
struct CCharStringData {
    const char*   m_pChars;                         // +0x00
    unsigned long m_length;                         // +0x04
};
struct CCharString {
    CCharStringData* m_pStringData;
    unsigned long GetLength() const { return m_pStringData == 0 ? 0 : m_pStringData->m_length; }
    const char* GetChars() const { return m_pStringData == 0 ? g_emptyLiteral_0129aaf4 : m_pStringData->m_pChars; }
};
unsigned long __fastcall Calc_004014a0(unsigned long seed, const char* data, unsigned long length);   // 0x004014A0

struct C3DVector {
    float X, Y, Z;
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) {}
};
struct CMatrix3x4 {
    C3DVector XAxis, YAxis, ZAxis, Position;
    C3DVector Transform(const C3DVector& v) const
    {
        return C3DVector(v.X * XAxis.X + v.Y * YAxis.X + v.Z * ZAxis.X + Position.X,
                         v.X * XAxis.Y + v.Y * YAxis.Y + v.Z * ZAxis.Y + Position.Y,
                         v.X * XAxis.Z + v.Y * YAxis.Z + v.Z * ZAxis.Z + Position.Z);
    }
};
struct C3DMeshStats {
    const C3DVector& GetHelperPoint(unsigned long crc, long index) const;   // 0x00A2E8C0
};
template<class T> struct CCountedPointer {
    T* Object; void* Count;
    T* operator->() const { return Object; }
};
struct C3DMeshObject {
    virtual void v00();
    void* MeshInfo;                                 // +4
    CCountedPointer<const C3DMeshStats> Mesh;       // +8
    CMatrix3x4 PositionMatrix;                      // +0x10
    C3DVector GetHelperPointPositionStatic(const CCharString& name, long index) const;
};

C3DVector C3DMeshObject::GetHelperPointPositionStatic(const CCharString& name, long index) const
{
    C3DVector point = Mesh->GetHelperPoint(Calc_004014a0(0, name.GetChars(), name.GetLength()), index);
    return PositionMatrix.Transform(point);
}
