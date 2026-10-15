// Behaviour test for 0x009881F0 CShaderRenderManager::SetWorldTransform: transposed 3x4 -> 4x4, flags, identity bit.
#include <cstdio>
struct CMatrix { unsigned int m[12]; };
struct Mgr {
    bool WorldMatrixIsIdentity; unsigned int WorldMatrix[16]; unsigned int UpdateFlags;
    void SetWorldTransform(const CMatrix* matrix)
    {
        static const int order[12] = { 0, 3, 6, 9, 1, 4, 7, 10, 2, 5, 8, 11 };
        for (int k = 0; k < 12; ++k) WorldMatrix[k] = matrix->m[order[k]];
        WorldMatrix[12] = 0; WorldMatrix[13] = 0; WorldMatrix[14] = 0; WorldMatrix[15] = 0x3f800000u;
        UpdateFlags |= 0xc990u;
        WorldMatrixIsIdentity = false;
    }
};
int main()
{
    CMatrix src; for (int i = 0; i < 12; ++i) src.m[i] = 100 + i;
    Mgr m; m.WorldMatrixIsIdentity = true; m.UpdateFlags = 1;
    m.SetWorldTransform(&src);
    if (m.WorldMatrix[1] != 103 || m.WorldMatrix[3] != 109 || m.WorldMatrix[4] != 101 || m.WorldMatrix[11] != 111 || m.WorldMatrix[14] != 0
        || m.WorldMatrix[15] != 0x3f800000u || m.UpdateFlags != 0xc991u || m.WorldMatrixIsIdentity) { std::printf("009881f0_TEST FAIL\n"); return 1; }
    std::printf("009881f0_TEST PASS\n");
    return 0;
}
