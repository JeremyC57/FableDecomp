// Grade: DIFFER(393v402) -- the x87 schedule of the two matrix products: retail copies the helper position with fld/fst (x into the dead helper slot, y kept on the FPU stack, z by integer move) and keeps x/y in registers through the first product; ours copies all three by integer moves. Explicit copy-ctor (310v402) and member operator* (308v402) are worse. Logic believed exact.
// C3DMesh2::GetHelperPointPositionAnimated(const C3DHelperPoint& helper, const C3DAnimationBlendState& stateA, const C3DAnimationBlendState& stateB, float blend, C3DAnimationManager& manager) const -- a helper point's position under the blended animation: into bone space through the bone's matrix, scaled and transformed by the blended bone transformation
// Leaf: GetHelperPointPositionAnimated
// Layout: retail C3DMesh2 Bones +0x9c (0x3c-byte C3DBone, matrix +0xc); C3DHelperPoint Position +0, BoneIndex +0xc.
struct CMatrix3x4 {
    float E11, E12, E13;
    float E21, E22, E23;
    float E31, E32, E33;
    float E41, E42, E43;
};
struct C3DVector {
    float X, Y, Z;
    C3DVector() {}
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) {}
};
__forceinline C3DVector operator*(C3DVector v, const CMatrix3x4& m)
{
    return C3DVector(v.X * m.E11 + v.Y * m.E21 + v.Z * m.E31 + m.E41,
                     v.X * m.E12 + v.Y * m.E22 + v.Z * m.E32 + m.E42,
                     v.X * m.E13 + v.Y * m.E23 + v.Z * m.E33 + m.E43);
}
struct C3DHelperPoint {
    C3DVector Position;                     // +0
    long BoneIndex;                         // +0xc
};
struct C3DBone {
    unsigned char _00[0xc];
    CMatrix3x4 Matrix;                      // +0xc
};
struct C3DAnimationBlendState;
class C3DMesh2;
struct C3DAnimationManager {
    void CalcBlendedBoneTransformation(const C3DAnimationBlendState& stateA, const C3DAnimationBlendState& stateB, float blend,
                                       const C3DMesh2& mesh, long bone, CMatrix3x4& transform, C3DVector& scale);   // 0x00AA2110
};
class C3DMesh2 {
public:
    unsigned char _00[0x9c];
    C3DBone* Bones;                         // +0x9c
    C3DVector GetHelperPointPositionAnimated(const C3DHelperPoint& helper, const C3DAnimationBlendState& stateA,
                                             const C3DAnimationBlendState& stateB, float blend, C3DAnimationManager& manager) const;
};

C3DVector C3DMesh2::GetHelperPointPositionAnimated(const C3DHelperPoint& helper, const C3DAnimationBlendState& stateA,
                                                   const C3DAnimationBlendState& stateB, float blend, C3DAnimationManager& manager) const
{
    long bone = helper.BoneIndex;
    if (bone != -1) {
    C3DVector local = helper.Position * Bones[bone].Matrix;
    CMatrix3x4 transform;
    C3DVector scale;
    manager.CalcBlendedBoneTransformation(stateA, stateB, blend, *this, bone, transform, scale);
    C3DVector scaled(local.X * scale.X, local.Y * scale.Y, local.Z * scale.Z);
    return scaled * transform;
    }
    return helper.Position;
}
