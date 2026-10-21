// C3DMesh2::GetHelperPointPositionAnimated(const C3DHelperPoint&, const C3DAnimationBlendState&, C3DAnimationManager&) const -- a bone-relative helper point localised by its bone, scaled and transformed by the animated bone transformation
// Grade: DIFFER(348v389) -- x87 scheduling: retail spills the copied helper position (x to a dead arg slot, y fst, z integer-moved) and reloads it per product, pushes &scale before the first transform and stores its Z after the other pushes, then reads the scaled vector through two frame copies; ours keeps the position on the x87 stack
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A89740, length 400 bytes (`ret 10h`, hidden-pointer C3DVector result).
// Leaf: ?GetHelperPointPositionAnimated@C3DMesh2@@UBE?AVC3DVector@@ABVC3DHelperPoint@@ABVC3DAnimationBlendState@@AAVC3DAnimationManager@@@Z
// Callees: C3DAnimationManager::CalcBoneTransformation(const C3DAnimationBlendState*, const C3DMesh2*, ulong, CMatrix3x4*,
//          C3DVector*) @ 0x00AA0FA0.
// Layout: retail C3DMesh2 Bones +0x9C (PDB +0xA4); C3DBone2 (0x3C) LocalizationMatrix +0xC; C3DHelperPoint Position +0,
//          BoneIndex +0xC; CMatrix3x4 XAxis, YAxis, ZAxis, Position (the landed 0x00A35F80 model).
struct C3DVector {
    float X, Y, Z;
    C3DVector() { }
    C3DVector(float x, float y, float z) { X = x; Y = y; Z = z; }
    C3DVector(const C3DVector& o) : X(o.X), Y(o.Y), Z(o.Z) { }
    C3DVector operator*(const C3DVector& v) const { return C3DVector(X * v.X, Y * v.Y, Z * v.Z); }
};
struct CMatrix3x4 {
    C3DVector XAxis, YAxis, ZAxis, Position;
    __forceinline C3DVector operator*(C3DVector v) const
    {
        C3DVector result;
        result.X = XAxis.X * v.X + YAxis.X * v.Y + ZAxis.X * v.Z + Position.X;
        result.Y = XAxis.Y * v.X + YAxis.Y * v.Y + ZAxis.Y * v.Z + Position.Y;
        result.Z = XAxis.Z * v.X + YAxis.Z * v.Y + ZAxis.Z * v.Z + Position.Z;
        return result;
    }
};
struct C3DHelperPoint {
    C3DVector Position;                         // +0x00
    long      BoneIndex;                        // +0x0c
};
struct C3DBone2 {
    unsigned long NameCRC;                      // +0x00
    long          Parent;                       // +0x04
    long          OriginalNoChildren;           // +0x08
    CMatrix3x4    LocalizationMatrix;           // +0x0c
};
struct C3DAnimationBlendState;
struct C3DMesh2;
struct C3DAnimationManager {
    void CalcBoneTransformation(const C3DAnimationBlendState* state, const C3DMesh2* mesh, unsigned long bone,
        CMatrix3x4* transform, C3DVector* scale);   // 0x00AA0FA0
};
struct C3DMesh2 {
    virtual void v00();
    unsigned char _pad04[0x9c - 0x04];
    C3DBone2* Bones;                            // +0x9c
    virtual C3DVector GetHelperPointPositionAnimated(const C3DHelperPoint& helper, const C3DAnimationBlendState& state,
        C3DAnimationManager& manager) const;
};

C3DVector C3DMesh2::GetHelperPointPositionAnimated(const C3DHelperPoint& helper, const C3DAnimationBlendState& state,
    C3DAnimationManager& manager) const
{
    long bone = helper.BoneIndex;
    if (bone != -1) {
        C3DVector position = Bones[bone].LocalizationMatrix * helper.Position;
        CMatrix3x4 transform;
        C3DVector scale;
        manager.CalcBoneTransformation(&state, this, bone, &transform, &scale);
        position = scale * position;
        return transform * position;
    }
    return helper.Position;
}
