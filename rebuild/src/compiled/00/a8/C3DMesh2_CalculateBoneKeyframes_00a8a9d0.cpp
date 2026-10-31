// C3DMesh2::CalculateBoneKeyframes() -- each bone's keyframe from the inverse of its localization matrix, post-multiplied by the parent's localization matrix
// Leaf: CalculateBoneKeyframes
// Grade: RELOCATION_MATCH -- named bone reference; stepwise sums in retail term order (operator*= rows, inverse
//   translation); the c11 cofactor written out twice (in det and in r.E11) instead of a named local, so its CSE
//   temporary dies at r.E11 (fxch reuse) and the cofactor products take retail's operand order
class CMatrix3x4 {
public:
    float E11, E12, E13;
    float E21, E22, E23;
    float E31, E32, E33;
    float E41, E42, E43;
    __forceinline CMatrix3x4& operator*=(const CMatrix3x4& b)
    {
        CMatrix3x4 a = *this;
        { float t = a.E11 * b.E11; t += a.E12 * b.E21; t += a.E13 * b.E31; E11 = t; }
        { float t = a.E12 * b.E22; t += a.E13 * b.E32; t += a.E11 * b.E12; E12 = t; }
        { float t = a.E11 * b.E13; t += a.E12 * b.E23; t += a.E13 * b.E33; E13 = t; }
        { float t = a.E22 * b.E21; t += a.E23 * b.E31; t += a.E21 * b.E11; E21 = t; }
        { float t = a.E22 * b.E22; t += a.E23 * b.E32; t += a.E21 * b.E12; E22 = t; }
        { float t = a.E21 * b.E13; t += a.E22 * b.E23; t += a.E23 * b.E33; E23 = t; }
        { float t = a.E32 * b.E21; t += a.E33 * b.E31; t += a.E31 * b.E11; E31 = t; }
        { float t = a.E32 * b.E22; t += a.E33 * b.E32; t += a.E31 * b.E12; E32 = t; }
        { float t = a.E31 * b.E13; t += a.E32 * b.E23; t += a.E33 * b.E33; E33 = t; }
        { float t = a.E42 * b.E21; t += a.E43 * b.E31; t += a.E41 * b.E11; t += b.E41; E41 = t; }
        { float t = a.E42 * b.E22; t += a.E43 * b.E32; t += a.E41 * b.E12; t += b.E42; E42 = t; }
        { float t = a.E41 * b.E13; t += a.E42 * b.E23; t += a.E43 * b.E33; t += b.E43; E43 = t; }
        return *this;
    }
    __forceinline void GetInverse(CMatrix3x4& r) const
    {
        float det = (E32 * E13 - E33 * E12) * E21 + (E23 * E12 - E22 * E13) * E31 + (E22 * E33 - E32 * E23) * E11;
        float invDet = 1.0f / det;
        r.E11 = (E22 * E33 - E32 * E23) * invDet;
        r.E21 = (E23 * E31 - E21 * E33) * invDet;
        r.E31 = (E32 * E21 - E22 * E31) * invDet;
        r.E12 = (E32 * E13 - E33 * E12) * invDet;
        r.E22 = (E11 * E33 - E13 * E31) * invDet;
        r.E32 = (E31 * E12 - E11 * E32) * invDet;
        r.E13 = (E23 * E12 - E22 * E13) * invDet;
        r.E23 = (E21 * E13 - E11 * E23) * invDet;
        r.E33 = (E11 * E22 - E21 * E12) * invDet;
        float t = r.E11 * E41;
        t += r.E31 * E43;
        t += r.E21 * E42;
        r.E41 = -t;
        t = r.E12 * E41;
        t += r.E32 * E43;
        t += r.E22 * E42;
        r.E42 = -t;
        t = r.E13 * E41;
        t += r.E33 * E43;
        t += r.E23 * E42;
        r.E43 = -t;
    }
};
class C3DBone2 { public:
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
        const C3DBone2& bone = Bones[i];
        CMatrix3x4 transform;
        bone.LocalizationMatrix.GetInverse(transform);
        long parent = bone.Parent;
        if (parent != -1)
            transform *= Bones[parent].LocalizationMatrix;
        BoneKeyframes[i].InitialiseFromMatrix(transform);
    }
}