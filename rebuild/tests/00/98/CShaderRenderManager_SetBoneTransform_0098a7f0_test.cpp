// Behaviour test for 0x0098A7F0 CShaderRenderManager::SetBoneTransform.
#include <cstdio>
struct CMatrix3x4 { unsigned int m[12]; };
struct Bone { unsigned int m[12]; };
struct Mgr {
    unsigned int UpdateFlags; Bone* BoneMatrices;
    void SetBoneTransform(unsigned long index, const CMatrix3x4* matrix)
    {
        static const int order[12] = { 0, 3, 6, 9, 1, 4, 7, 10, 2, 5, 8, 11 };
        Bone& bone = BoneMatrices[index];
        for (int k = 0; k < 12; ++k) bone.m[k] = matrix->m[order[k]];
        UpdateFlags |= 0x20u;
    }
};
int main()
{
    Bone bones[3]; CMatrix3x4 src; for (int i = 0; i < 12; ++i) src.m[i] = 50 + i;
    Mgr m; m.BoneMatrices = bones; m.UpdateFlags = 1;
    m.SetBoneTransform(2, &src);
    if (bones[2].m[0] != 50 || bones[2].m[1] != 53 || bones[2].m[4] != 51 || bones[2].m[11] != 61 || m.UpdateFlags != 0x21) { std::printf("0098a7f0_TEST FAIL\n"); return 1; }
    std::printf("0098a7f0_TEST PASS\n");
    return 0;
}