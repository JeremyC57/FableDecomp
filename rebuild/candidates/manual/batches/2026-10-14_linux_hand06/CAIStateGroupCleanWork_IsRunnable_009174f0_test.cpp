#include <cstdio>
static long g_frame; struct CBuilding {};
static long GFGetFrameCount() { return g_frame; }
struct CAIStateGroupBase { virtual ~CAIStateGroupBase() {} unsigned char _pad[0x10]; };
struct CAIStateGroup_CleanWork : CAIStateGroupBase {
    long m_nextFrame; long m_frameToStart; CBuilding* m_building;
    virtual bool IsRunnable() { return GetBuilding() != 0 && GFGetFrameCount() > m_frameToStart; }
    virtual CBuilding* GetBuilding() { return m_building; }
};
int main()
{
    CAIStateGroup_CleanWork g; CBuilding b; g.m_frameToStart = 100;
    g.m_building = 0; g_frame = 200; if (g.IsRunnable()) { std::printf("009174f0_TEST FAIL\n"); return 1; }
    g.m_building = &b; g_frame = 100; if (g.IsRunnable()) { std::printf("009174f0_TEST FAIL\n"); return 1; }
    g_frame = 101; if (!g.IsRunnable()) { std::printf("009174f0_TEST FAIL\n"); return 1; }
    std::printf("009174f0_TEST PASS\n"); return 0;
}
