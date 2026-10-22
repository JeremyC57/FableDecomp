// C3DMesh2::GetDummyObjectTransformAnimated(const C3DDummyObject&, CMatrix3x4&, const C3DAnimationBlendState&, const C3DAnimationBlendState&, float, C3DAnimationManager&, bool) const -- the blended-state twin of 0x00A8A220
// Grade: DIFFER(982v981) -- first draft, as 0x00A8A220
// Leaf: GetDummyObjectTransformAnimated
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
        CMatrix3x4 boneTransform;
        C3DVector scale;
        manager.CalcBlendedBoneTransformation(blend, blend2, blendFactor, *this, bone, boneTransform, scale);
        {
            const CMatrix3x4& a = in.Transform;
            const CMatrix3x4& b = Bones[bone].LocalizationMatrix;
            CMatrix3x4& o = out;
            o.E11 = a.E11 * b.E11 + a.E12 * b.E21 + a.E13 * b.E31;
            o.E12 = a.E11 * b.E12 + a.E12 * b.E22 + a.E13 * b.E32;
            o.E13 = a.E11 * b.E13 + a.E12 * b.E23 + a.E13 * b.E33;
            o.E21 = a.E21 * b.E11 + (a.E22 * b.E21 + a.E23 * b.E31);
            o.E22 = a.E21 * b.E12 + a.E22 * b.E22 + a.E23 * b.E32;
            o.E23 = a.E21 * b.E13 + a.E22 * b.E23 + a.E23 * b.E33;
            o.E31 = a.E31 * b.E11 + a.E32 * b.E21 + a.E33 * b.E31;
            o.E32 = a.E31 * b.E12 + a.E32 * b.E22 + a.E33 * b.E32;
            o.E33 = a.E31 * b.E13 + a.E32 * b.E23 + a.E33 * b.E33;
            o.E41 = a.E41 * b.E11 + a.E42 * b.E21 + a.E43 * b.E31 + b.E41;
            o.E42 = a.E41 * b.E12 + a.E42 * b.E22 + b.E32 * a.E43 + b.E42;
            o.E43 = a.E41 * b.E13 + a.E42 * b.E23 + a.E43 * b.E33 + b.E43;
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
