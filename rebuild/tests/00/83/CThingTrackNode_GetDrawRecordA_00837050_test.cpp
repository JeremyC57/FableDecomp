#include <cstdio>
// Behaviour test for CThingTrackNode::GetDrawRecordA (0x00837050): returns the first word of the bank's draw record.
struct CMeshInfoSize { long m_a; long m_b; float m_x; };
struct CMeshDataBank { CMeshInfoSize m_size; };
static CMeshDataBank g_bank;
static CMeshDataBank* GetMeshBank() { return &g_bank; }
struct CThingTrackNode {
    long GetDrawRecordA() { return GetMeshBank()->m_size.m_a; }
};
int main()
{
    g_bank.m_size.m_a = 1234; g_bank.m_size.m_b = 5; g_bank.m_size.m_x = 2.0f;
    CThingTrackNode n;
    if (n.GetDrawRecordA() != 1234) { std::printf("00837050_TEST FAIL\n"); return 1; }
    std::printf("00837050_TEST PASS\n");
    return 0;
}