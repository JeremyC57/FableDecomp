// C3DMesh2::GetDummyObjectTransformAnimated(const C3DDummyObject&, CMatrix3x4&, const C3DAnimationBlendState&, const C3DAnimationBlendState&, float, C3DAnimationManager&, bool) const -- the blended-state twin of 0x00A8A220
// Grade: DIFFER(981v981) -- chunk 621: the twin lever of 0x00A8A220 (sequenced retail-order row sums, 256 -> 204 normalised diff lines); left: the canonicalised fld/fmul operand order inside about half of the products (source operand order is inert)
// Leaf: ?GetDummyObjectTransformAnimated@C3DMesh2@@QBEXABUC3DDummyObject@@AAVCMatrix3x4@@ABVC3DAnimationBlendState@@2MAAVC3DAnimationManager@@_N@Z
// Previous grade: DIFFER(981v981) -- now same length (twin lever of 0x00A8A220: the bone record named before the blended
//   CalcBlendedBoneTransformation call); residue only the x87 operand/term order of the inlined 3x4 multiply (sec5
//   retry: 1 grade, 304 -> 133 diff lines)
class C3DVector {
public:
    float X, Y, Z;
    C3DVector() {}
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) {}
    C3DVector(const C3DVector& o) : X(o.X), Y(o.Y), Z(o.Z) {}
    C3DVector operator*(const C3DVector& o) const { return C3DVector(X * o.X, Y * o.Y, Z * o.Z); }
};
class CMatrix3x4 {
public:
    float E11, E12, E13;
    float E21, E22, E23;
    float E31, E32, E33;
    float E41, E42, E43;
    C3DVector GetPosition() const { return C3DVector(E41, E42, E43); }
    void SetPosition(const C3DVector& p) { E41 = p.X; E42 = p.Y; E43 = p.Z; }
    __forceinline CMatrix3x4& operator*=(const CMatrix3x4& b)
    {
        CMatrix3x4 a = *this;
        { float s = b.E31 * a.E13; s += b.E21 * a.E12; s += b.E11 * a.E11; E11 = s; }
        { float s = b.E32 * a.E13; s += b.E22 * a.E12; s += b.E12 * a.E11; E12 = s; }
        { float s = b.E33 * a.E13; s += b.E23 * a.E12; s += b.E13 * a.E11; E13 = s; }
        { float s = a.E23 * b.E31; s += a.E22 * b.E21; s += a.E21 * b.E11; E21 = s; }
        { float s = a.E23 * b.E32; s += a.E22 * b.E22; s += a.E21 * b.E12; E22 = s; }
        { float s = a.E23 * b.E33; s += a.E22 * b.E23; s += a.E21 * b.E13; E23 = s; }
        { float s = a.E33 * b.E31; s += a.E32 * b.E21; s += a.E31 * b.E11; E31 = s; }
        { float s = a.E33 * b.E32; s += a.E32 * b.E22; s += a.E31 * b.E12; E32 = s; }
        { float s = a.E33 * b.E33; s += a.E32 * b.E23; s += a.E31 * b.E13; E33 = s; }
        { float s = a.E43 * b.E31; s += a.E42 * b.E21; s += a.E41 * b.E11; s += b.E41; E41 = s; }
        { float s = a.E43 * b.E32; s += a.E42 * b.E22; s += a.E41 * b.E12; s += b.E42; E42 = s; }
        { float s = a.E43 * b.E33; s += a.E42 * b.E23; s += a.E41 * b.E13; s += b.E43; E43 = s; }
        return *this;
    }
    __forceinline void Multiply(const CMatrix3x4& a, const CMatrix3x4& b)
    {
        E11 = a.E13 * b.E31 + a.E12 * b.E21 + a.E11 * b.E11;
        E12 = a.E12 * b.E22 + a.E11 * b.E12 + a.E13 * b.E32;
        E13 = a.E12 * b.E23 + a.E11 * b.E13 + a.E13 * b.E33;
        E21 = a.E23 * b.E31 + a.E22 * b.E21 + a.E21 * b.E11;
        E22 = a.E22 * b.E22 + a.E21 * b.E12 + a.E23 * b.E32;
        E23 = a.E22 * b.E23 + a.E21 * b.E13 + a.E23 * b.E33;
        E31 = a.E33 * b.E31 + (a.E32 * b.E21 + a.E31 * b.E11);
        E32 = a.E33 * b.E32 + (a.E31 * b.E12 + a.E32 * b.E22);
        E33 = a.E32 * b.E23 + a.E31 * b.E13 + a.E33 * b.E33;
        E41 = a.E43 * b.E31 + a.E42 * b.E21 + a.E41 * b.E11 + b.E41;
        E42 = a.E43 * b.E32 + (a.E41 * b.E12 + a.E42 * b.E22) + b.E42;
        E43 = a.E42 * b.E23 + a.E41 * b.E13 + a.E43 * b.E33 + b.E43;
    }
};
struct C3DDummyObject {
    CMatrix3x4 Transform;   // +0x00
    long BoneIndex;         // +0x30
};
struct C3DBone2 {
    unsigned long NameCRC;
    long Parent;
    long OriginalNoChildren;
    CMatrix3x4 LocalizationMatrix;   // +0x0c
};
class C3DAnimationBlendState;
class C3DMesh2;
class C3DAnimationManager {
public:
    void CalcBlendedBoneTransformation(const C3DAnimationBlendState& blend1, const C3DAnimationBlendState& blend2, float blendFactor, const C3DMesh2& mesh, unsigned long bone, CMatrix3x4& transform, C3DVector& scale); // 0x00AA2110
    void CalcBoneTransformation(const C3DAnimationBlendState& blend, const C3DMesh2& mesh, unsigned long bone, CMatrix3x4& transform, C3DVector& scale); // 0x00AA0FA0
};
class C3DMesh2 {
public:
    char _pad0[0x9c];
    C3DBone2* Bones;   // +0x9c
    void GetDummyObjectTransformAnimated(const C3DDummyObject& in, CMatrix3x4& out, const C3DAnimationBlendState& blend, const C3DAnimationBlendState& blend2, float blendFactor, C3DAnimationManager& manager, bool applyScale) const;
};

void C3DMesh2::GetDummyObjectTransformAnimated(const C3DDummyObject& in, CMatrix3x4& out, const C3DAnimationBlendState& blend, const C3DAnimationBlendState& blend2, float blendFactor, C3DAnimationManager& manager, bool applyScale) const
{
    long bone = in.BoneIndex;
    if (bone != -1) {
        const C3DBone2& boneData = Bones[bone];
        CMatrix3x4 boneTransform;
        C3DVector scale;
        manager.CalcBlendedBoneTransformation(blend, blend2, blendFactor, *this, bone, boneTransform, scale);
        {
            const CMatrix3x4& a = in.Transform;
            const CMatrix3x4& b = boneData.LocalizationMatrix;
            CMatrix3x4& o = out;
            { float s = a.E13 * b.E31; s += b.E11 * a.E11; s += a.E12 * b.E21; o.E11 = s; }
            { float s = a.E13 * b.E32; s += a.E12 * b.E22; s += b.E12 * a.E11; o.E12 = s; }
            { float s = a.E13 * b.E33; s += a.E12 * b.E23; s += b.E13 * a.E11; o.E13 = s; }
            { float s = b.E31 * a.E23; s += b.E21 * a.E22; s += b.E11 * a.E21; o.E21 = s; }
            { float s = b.E12 * a.E21; s += b.E32 * a.E23; s += b.E22 * a.E22; o.E22 = s; }
            { float s = b.E13 * a.E21; s += b.E33 * a.E23; s += b.E23 * a.E22; o.E23 = s; }
            { float s = a.E31 * b.E11; s += b.E31 * a.E33; s += a.E32 * b.E21; o.E31 = s; }
            { float s = a.E33 * b.E32; s += a.E32 * b.E22; s += a.E31 * b.E12; o.E32 = s; }
            { float s = a.E33 * b.E33; s += a.E32 * b.E23; s += a.E31 * b.E13; o.E33 = s; }
            { float s = b.E31 * a.E43; s += b.E21 * a.E42; s += b.E11 * a.E41; s += b.E41; o.E41 = s; }
            { float s = b.E12 * a.E41; s += b.E32 * a.E43; s += b.E22 * a.E42; s += b.E42; o.E42 = s; }
            { float s = b.E13 * a.E41; s += b.E33 * a.E43; s += b.E23 * a.E42; s += b.E43; o.E43 = s; }
        }
        if (applyScale) {
            out.E11 *= scale.X;
            out.E12 *= scale.X;
            out.E13 *= scale.X;
            out.E21 *= scale.Y;
            out.E22 *= scale.Y;
            out.E23 *= scale.Y;
            out.E31 *= scale.Z;
            out.E32 *= scale.Z;
            out.E33 *= scale.Z;
        } else {
            out.SetPosition(out.GetPosition() * scale);
        }
        out *= boneTransform;
    } else {
        out = in.Transform;
    }
}
