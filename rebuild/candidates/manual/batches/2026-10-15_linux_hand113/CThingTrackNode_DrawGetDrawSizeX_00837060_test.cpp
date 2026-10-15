#include <cstdio>
// Behaviour test for CThingTrackNode::DrawGetDrawSizeX (0x00837060): out = bank size x * node scale.
struct CMeshInfoSize { long m_a; long m_b; float m_x; };
struct CMeshDataBank { CMeshInfoSize m_size; };
static CMeshDataBank g_bank;
static CMeshDataBank* GetMeshBank() { return &g_bank; }
struct CThingTrackNode {
    float m_scale;
    void DrawGetDrawSizeX(float, float* out)
    {
        CMeshInfoSize* size = &GetMeshBank()->m_size;
        *out = size->m_x * m_scale;
    }
};
int main()
{
    g_bank.m_size.m_x = 2.5f;
    CThingTrackNode n; n.m_scale = 4.0f;
    float r = 0.0f;
    n.DrawGetDrawSizeX(99.0f, &r);
    if (r != 10.0f) { std::printf("00837060_TEST FAIL\n"); return 1; }
    std::printf("00837060_TEST PASS\n");
    return 0;
}
