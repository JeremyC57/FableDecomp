// Behaviour test for 0x00988290 CShaderRenderManager::ResetWorldTransform.
#include <cstdio>
class CMatrix4x4 { public: float m[4][4]; };
struct Manager {
    bool WorldMatrixIsIdentity; CMatrix4x4 WorldMatrix; unsigned long UpdateFlags;
    void ResetWorldTransform()
    {
        if (!WorldMatrixIsIdentity) {
            for (int r = 0; r < 4; ++r) for (int c = 0; c < 4; ++c) WorldMatrix.m[r][c] = (r == c) ? 1.0f : 0.0f;
            UpdateFlags |= 0xC990;
            WorldMatrixIsIdentity = true;
        }
    }
};
int main()
{
    Manager mgr; mgr.WorldMatrixIsIdentity = false; mgr.UpdateFlags = 0x1;
    for (int i = 0; i < 16; ++i) mgr.WorldMatrix.m[i / 4][i % 4] = 7.0f;
    mgr.ResetWorldTransform();
    for (int r = 0; r < 4; ++r) for (int c = 0; c < 4; ++c)
        if (mgr.WorldMatrix.m[r][c] != ((r == c) ? 1.0f : 0.0f)) { std::printf("00988290_TEST FAIL\n"); return 1; }
    if (mgr.UpdateFlags != 0xC991 || !mgr.WorldMatrixIsIdentity) { std::printf("00988290_TEST FAIL\n"); return 1; }
    mgr.UpdateFlags = 0; mgr.WorldMatrix.m[0][1] = 5.0f;
    mgr.ResetWorldTransform();                 // already identity: untouched
    if (mgr.UpdateFlags != 0 || mgr.WorldMatrix.m[0][1] != 5.0f) { std::printf("00988290_TEST FAIL\n"); return 1; }
    std::printf("00988290_TEST PASS\n");
    return 0;
}
