// C3DMeshObject::GetDummyObjectPositionStatic(const CCharString&, long, C3DVector&, C3DVector&, C3DVector&) const -- look a named dummy up in the mesh (by name CRC and index) and return its position through the object's matrix, and its up and forward axes rotated by it and fast-normalised
// Grade: DIFFER(703v700) -- name CRC, dummy lookup, the position transform and the 12-dword dummy matrix copy line up;
//   the two rotated axes are scheduled differently (retail keeps the up axis on the x87 stack and spills forward to a
//   separate frame block: frame 0x6c v 0x48), and the normalise spills reuse the dead index argument slot in retail.
// Retail: Fable.exe (TLC, VC7.1) @ 0x00A364E0, 700 bytes (`ret 14h`).
//   ?GetDummyObjectPositionStatic@C3DMeshObject@@QBEXABVCCharString@@JAAVC3DVector@@11@Z (Ego_r.pdb).
// Callees: CRC Calc 0x004014A0 (__fastcall seed, data, length), C3DMeshStats::GetDummyObject 0x00A2E920. Globals: the
//   reciprocal-square-root mantissa table 0x013CE558, 3.0f 0x0122DED4, 0.5f 0x0122F59C; "" 0x0129AAF4.
// Layout: C3DMeshObject Mesh +0x08, PositionMatrix +0x10 (CMatrix3x4, Ego_r.pdb); C3DDummyObject Transform +0 (rows right,
//   up, forward, position) per Ego_r.pdb. The fast Normalise is the landed CTCPhysicsRigidBody::GetRHSet 0x00759B40 form.
extern unsigned long g_InvSqrtMantissaTable[128];   // 0x013CE558
unsigned long __fastcall Calc_004014a0(unsigned long seed, const char* data, unsigned long length);   // 0x004014A0: CRC
struct C3DVector {
    float X, Y, Z;
    C3DVector() { }
    C3DVector(const C3DVector& o) : X(o.X), Y(o.Y), Z(o.Z) { }
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) { }
    __forceinline void Normalise()
    {
        float lengthSquared = Z * Z;
        lengthSquared += Y * Y;
        lengthSquared += X * X;
        unsigned long bits = *(unsigned long*)&lengthSquared;
        unsigned long estimate = (((0x17c - ((bits >> 23) & 0xff)) >> 1) << 23) |
                                 (g_InvSqrtMantissaTable[(bits >> 17) & 0x7f] << 15);
        float r = *(float*)&estimate;
        r = (3.0f - r * r * lengthSquared) * r;
        r *= 0.5f;
        X *= r;
        Y *= r;
        Z *= r;
    }
};
struct CMatrix3x4 {
    float M[4][3];                              // rows: right +0x00, up +0x0c, forward +0x18, position +0x24
    C3DVector GetPosition() const { return C3DVector(M[3][0], M[3][1], M[3][2]); }
    __forceinline void TransformPoint(C3DVector& v) const
    {
        float x = v.X, y = v.Y, z = v.Z;
        v.X = z * M[2][0] + y * M[1][0] + x * M[0][0] + M[3][0];
        v.Y = z * M[2][1] + y * M[1][1] + x * M[0][1] + M[3][1];
        v.Z = z * M[2][2] + y * M[1][2] + x * M[0][2] + M[3][2];
    }
    __forceinline C3DVector RotateRow(const float* v) const
    {
        return C3DVector(v[1] * M[1][0] + v[2] * M[2][0] + v[0] * M[0][0],
                         v[1] * M[1][1] + v[2] * M[2][1] + v[0] * M[0][1],
                         v[1] * M[1][2] + v[2] * M[2][2] + v[0] * M[0][2]);
    }
};
struct CCharStringData {
    const char* m_pData;
    unsigned long m_length;
};
struct CCharString {
    CCharStringData* PStringData;
    const char* GetData() const { return !PStringData ? "" : PStringData->m_pData; }
    unsigned long Length() const { return !PStringData ? 0 : PStringData->m_length; }
};
struct C3DDummyObject {
    CMatrix3x4 Transform;                       // +0x00
    long BoneIndex;                             // +0x30
};
struct C3DMeshStats {
    const C3DDummyObject* GetDummyObject(unsigned long nameCRC, long index) const;   // 0x00A2E920
};
struct C3DMeshObject {
    void* _vptr;
    void* MeshInfo;                             // +0x04
    const C3DMeshStats* Mesh;                   // +0x08 CCountedPointer data
    void* MeshCount;                            // +0x0c
    CMatrix3x4 PositionMatrix;                  // +0x10
    void GetDummyObjectPositionStatic(const CCharString& name, long index, C3DVector& pos, C3DVector& up,
                                      C3DVector& forward) const;
};

void C3DMeshObject::GetDummyObjectPositionStatic(const CCharString& name, long index, C3DVector& pos, C3DVector& up,
                                                 C3DVector& forward) const
{
    const C3DDummyObject* dummy = Mesh->GetDummyObject(Calc_004014a0(0, name.GetData(), name.Length()), index);
    pos = dummy->Transform.GetPosition();
    PositionMatrix.TransformPoint(pos);
    CMatrix3x4 transform = dummy->Transform;
    C3DVector newUp = PositionMatrix.RotateRow(transform.M[1]);
    C3DVector newForward = PositionMatrix.RotateRow(transform.M[2]);
    up = newUp;
    forward = newForward;
    up.Normalise();
    forward.Normalise();
}
