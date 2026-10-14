#include <cstdio>
struct C3DMeshInfo { int x; };
static C3DMeshInfo g_info; static long g_askedId = -1;
struct CMatrix3x4 { float m_rows[12]; };
struct CBank { C3DMeshInfo* GetMeshInfo(long id) { g_askedId = id; return &g_info; } };
struct CEngineData { unsigned char _pad[0x10]; CBank* m_pMeshBank; };
static CEngineData g_data; static CEngineData* g_pEngineData = &g_data;
struct SUpdate { unsigned char _pad[0xc]; long m_meshId; CMatrix3x4 m_worldTransform; long m_blendMode; long m_blendOp; };
struct CGroup { unsigned char _pad[0xc]; unsigned long EnginePrimitiveFlags; unsigned char _pad10[0x9c]; C3DMeshInfo* MeshInfo; CMatrix3x4 WorldTransform; long BlendOp; long BlendMode;
    void Update(const SUpdate& u, unsigned long flags) { EnginePrimitiveFlags = flags; CBank* bank = g_pEngineData->m_pMeshBank; MeshInfo = bank->GetMeshInfo(u.m_meshId); WorldTransform = u.m_worldTransform; BlendOp = u.m_blendOp; BlendMode = u.m_blendMode; } };
int main()
{
    CBank bank; g_data.m_pMeshBank = &bank; SUpdate u; u.m_meshId = 42; for (int i = 0; i < 12; ++i) u.m_worldTransform.m_rows[i] = (float)i; u.m_blendMode = 3; u.m_blendOp = 4;
    CGroup g; g.Update(u, 0x77);
    bool ok = g.EnginePrimitiveFlags == 0x77 && g_askedId == 42 && g.MeshInfo == &g_info && g.BlendOp == 4 && g.BlendMode == 3;
    for (int i = 0; i < 12; ++i) if (g.WorldTransform.m_rows[i] != (float)i) ok = false;
    if (!ok) { std::printf("00bc2400_TEST FAIL\n"); return 1; }
    std::printf("00bc2400_TEST PASS\n"); return 0;
}