// C3DMeshObject::GetDummyObjectPositionAnimated(const CCharString&, long, const C3DAnimationBlendState&, const C3DAnimationBlendState&, float, C3DAnimationManager&, C3DVector&, C3DVector&, C3DVector&, bool) const -- look a named dummy up, let the mesh animate it for the blend states, then return its position through the object's matrix and its up and forward axes rotated by it and fast-normalised
// Grade: DIFFER(745v747) -- first draft only: frame 0x4c v 0x78 (retail homes the GetPosition temp and the rotated axes in separate frame blocks, as its parked sibling 00a364e0), mesh pointer loaded before the CRC call in retail, dummy address add edi,4 late v folded lea
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
    C3DDummyObject();                           // 0x00A2E4B0
};
struct SDummyEntry {                            // Layout: 0x38-byte entries (imul 0x38, object at +4)
    unsigned long NameCRC;
    C3DDummyObject Dummy;
};
struct C3DAnimationBlendState;
struct C3DAnimationManager;
struct C3DMeshStats {
    virtual void v00();
    virtual void v01();
    virtual void v02();
    virtual void v03();
    virtual void v04();
    virtual void v05();
    virtual void GetDummyObjectPositionAnimated(const C3DDummyObject& dummy, C3DDummyObject& out,
        const C3DAnimationBlendState& s1, const C3DAnimationBlendState& s2, float blend,
        C3DAnimationManager& manager, bool flag) const;          // slot 6
    const SDummyEntry* GetDummyObjects(unsigned long nameCRC) const;   // 0x00A2E960
};
struct C3DMeshObject {
    void* _vptr;
    void* MeshInfo;                             // +0x04
    const C3DMeshStats* Mesh;                   // +0x08 CCountedPointer data
    void* MeshCount;                            // +0x0c
    CMatrix3x4 PositionMatrix;                  // +0x10
    void GetDummyObjectPositionAnimated(const CCharString& name, long index, const C3DAnimationBlendState& s1,
        const C3DAnimationBlendState& s2, float blend, C3DAnimationManager& manager, C3DVector& pos,
        C3DVector& up, C3DVector& forward, bool flag) const;
};

void C3DMeshObject::GetDummyObjectPositionAnimated(const CCharString& name, long index, const C3DAnimationBlendState& s1,
    const C3DAnimationBlendState& s2, float blend, C3DAnimationManager& manager, C3DVector& pos,
    C3DVector& up, C3DVector& forward, bool flag) const
{
    const C3DDummyObject& dummy = Mesh->GetDummyObjects(Calc_004014a0(0, name.GetData(), name.Length()))[index].Dummy;
    C3DDummyObject animated;
    Mesh->GetDummyObjectPositionAnimated(dummy, animated, s1, s2, blend, manager, flag);
    pos = animated.Transform.GetPosition();
    PositionMatrix.TransformPoint(pos);
    C3DVector newUp = PositionMatrix.RotateRow(animated.Transform.M[1]);
    C3DVector newForward = PositionMatrix.RotateRow(animated.Transform.M[2]);
    up = newUp;
    forward = newForward;
    up.Normalise();
    forward.Normalise();
}
