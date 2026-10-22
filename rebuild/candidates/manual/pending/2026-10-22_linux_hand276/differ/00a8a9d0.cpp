// C3DMesh2::CalculateBoneKeyframes() -- each bone's keyframe from the inverse of its localization matrix, post-multiplied by the parent's localization matrix
// Grade: DIFFER(876v874) -- first draft: this/offset registers swapped (retail this in ebx, the bone byte offset in ebp with Bones in ecx), x87 operand order of some cofactor products, and retail reuses the c11 cofactor with fxch where ours copies it
// Leaf: CalculateBoneKeyframes
class CMatrix3x4 {
public:
    float E11, E12, E13;
    float E21, E22, E23;
    float E31, E32, E33;
    float E41, E42, E43;
    __forceinline CMatrix3x4& operator*=(const CMatrix3x4& b)
    {
        CMatrix3x4 a = *this;
        E11 = a.E11 * b.E11 + a.E12 * b.E21 + a.E13 * b.E31;
        E12 = a.E11 * b.E12 + a.E12 * b.E22 + a.E13 * b.E32;
        E13 = a.E11 * b.E13 + a.E12 * b.E23 + a.E13 * b.E33;
        E21 = a.E21 * b.E11 + a.E22 * b.E21 + a.E23 * b.E31;
        E22 = a.E21 * b.E12 + a.E22 * b.E22 + a.E23 * b.E32;
        E23 = a.E21 * b.E13 + a.E22 * b.E23 + a.E23 * b.E33;
        E31 = a.E31 * b.E11 + a.E32 * b.E21 + a.E33 * b.E31;
        E32 = a.E31 * b.E12 + a.E32 * b.E22 + a.E33 * b.E32;
        E33 = a.E31 * b.E13 + a.E32 * b.E23 + a.E33 * b.E33;
        E41 = a.E41 * b.E11 + a.E42 * b.E21 + a.E43 * b.E31 + b.E41;
        E42 = a.E41 * b.E12 + a.E42 * b.E22 + a.E43 * b.E32 + b.E42;
        E43 = a.E41 * b.E13 + a.E42 * b.E23 + a.E43 * b.E33 + b.E43;
        return *this;
    }
    __forceinline void GetInverse(CMatrix3x4& r) const
    {
        float c11 = E22 * E33 - E32 * E23;
        float det = (E32 * E13 - E33 * E12) * E21 + (E23 * E12 - E22 * E13) * E31 + c11 * E11;
        float invDet = 1.0f / det;
        r.E11 = c11 * invDet;
        r.E21 = (E23 * E31 - E21 * E33) * invDet;
        r.E31 = (E32 * E21 - E22 * E31) * invDet;
        r.E12 = (E32 * E13 - E33 * E12) * invDet;
        r.E22 = (E11 * E33 - E13 * E31) * invDet;
        r.E32 = (E31 * E12 - E11 * E32) * invDet;
        r.E13 = (E23 * E12 - E22 * E13) * invDet;
        r.E23 = (E21 * E13 - E11 * E23) * invDet;
        r.E33 = (E11 * E22 - E21 * E12) * invDet;
        r.E41 = -(r.E11 * E41 + r.E31 * E43 + r.E21 * E42);
        r.E42 = -(r.E12 * E41 + r.E32 * E43 + r.E22 * E42);
        r.E43 = -(r.E13 * E41 + r.E33 * E43 + r.E23 * E42);
    }
};
struct C3DBone2 {
    unsigned long NameCRC;
    long Parent;                     // +0x04
    long OriginalNoChildren;
    CMatrix3x4 LocalizationMatrix;   // +0x0c
};
class C3DKeyframe {
public:
    char _data[0x30];
    bool InitialiseFromMatrix(const CMatrix3x4& matrix);   // 0x00AAF1E0
};
class C3DMesh2 {
public:
    char _pad0[0x98];
    unsigned long BoneCount;         // +0x98
    C3DBone2* Bones;                 // +0x9c
    C3DKeyframe* BoneKeyframes;      // +0xa0
    void CalculateBoneKeyframes();
};

void C3DMesh2::CalculateBoneKeyframes()
{
    for (unsigned long i = 0; i < BoneCount; ++i) {
        CMatrix3x4 transform;
        Bones[i].LocalizationMatrix.GetInverse(transform);
        long parent = Bones[i].Parent;
        if (parent != -1)
            transform *= Bones[parent].LocalizationMatrix;
        BoneKeyframes[i].InitialiseFromMatrix(transform);
    }
}
