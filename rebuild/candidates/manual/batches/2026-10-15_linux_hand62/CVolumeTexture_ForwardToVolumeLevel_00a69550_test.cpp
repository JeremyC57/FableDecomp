#include <cstdio>

// CVolumeTexture::ForwardToVolumeLevel (00a69550): the level's CVolume temporary receives the argument,
// then its destructor releases the wrapped volume exactly once and runs the CBase destructor.
static int g_baseDtors = 0, g_releases = 0, g_helperCalls = 0;
static void* g_helperArg = 0;
static unsigned long g_requestedLevel = 0xFFFFFFFFUL;

struct IVolume {
    int m_refs;
    void Release() { --m_refs; }
};
struct CBase {
    virtual ~CBase() { ++g_baseDtors; }
};
struct CVolume : CBase {
    IVolume* m_pVolume;
    CVolume(IVolume* v) : m_pVolume(v) { }
    CVolume(const CVolume& o) : CBase(), m_pVolume(o.m_pVolume) { if (m_pVolume) ++m_pVolume->m_refs; }
    void ReleaseVolume(bool) { if (m_pVolume) { m_pVolume->Release(); ++g_releases; m_pVolume = 0; } }
    void Helper(void* argument) { ++g_helperCalls; g_helperArg = argument; }
    virtual ~CVolume() { ReleaseVolume(true); }
};
struct CVolumeTexture {
    IVolume m_levels[4];
    CVolume GetVolumeLevel(unsigned long level)
    {
        g_requestedLevel = level;
        ++m_levels[level].m_refs;
        return CVolume(&m_levels[level]);
    }
    void ForwardToVolumeLevel(unsigned long level, void* argument);
};
void CVolumeTexture::ForwardToVolumeLevel(unsigned long level, void* argument)
{
    GetVolumeLevel(level).Helper(argument);
}

int main()
{
    CVolumeTexture t;
    for (int i = 0; i < 4; ++i) t.m_levels[i].m_refs = 1;
    int record = 0;
    t.ForwardToVolumeLevel(2, &record);
    if (g_requestedLevel != 2 || g_helperCalls != 1 || g_helperArg != &record || t.m_levels[2].m_refs != 1
        || g_releases < 1 || g_baseDtors < 1 || t.m_levels[0].m_refs != 1) {
        std::printf("00a69550_TEST FAIL\n");
        return 1;
    }
    std::printf("00a69550_TEST PASS\n");
    return 0;
}
