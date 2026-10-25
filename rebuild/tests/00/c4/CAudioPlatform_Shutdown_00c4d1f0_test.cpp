#include <cstdio>
struct CSoundDesc { int initResult; };
static int g_deletes = 0, g_inits = 0;
struct CPooledVoice {
    CPooledVoice* m_pNext; bool m_onActive;
    CPooledVoice() : m_pNext(0), m_onActive(false) {}
    virtual ~CPooledVoice() { ++g_deletes; }
    virtual int Init(CSoundDesc* desc) { ++g_inits; return desc->initResult; }
};
struct CVoicePool {
    CPooledVoice* m_pActiveHead; CPooledVoice* m_pFreeHead; int m_freeCount; int m_activeCount; int m_clears;
    CVoicePool() : m_pActiveHead(0), m_pFreeHead(0), m_freeCount(0), m_activeCount(0), m_clears(0) {}
    void Clear() { ++m_clears; m_pActiveHead = 0; m_pFreeHead = 0; m_activeCount = 0; m_freeCount = -1; }
    CPooledVoice* GetFreeHead() { return m_pFreeHead; }
    void PushActive(CPooledVoice* v) { v->m_onActive = true; v->m_pNext = m_pActiveHead; m_pActiveHead = v; ++m_activeCount; }
    void PopFree() { m_pFreeHead = m_pFreeHead->m_pNext; --m_freeCount; }
};
struct SVoiceSlot { int m_00; int m_spare; int m_rest[4]; };
struct SAudioConfig { int m_head[12]; SVoiceSlot m_voicesA; SVoiceSlot m_voicesB; int m_mid[12]; int m_preallocatedLeft; int m_tail[3]; };
static SAudioConfig g_audioConfig;
static SAudioConfig k_defaultAudioConfig;
struct CAudioMaster { int x; };
static CAudioMaster g_master; static CAudioMaster* g_pAudioMaster = &g_master;
static int g_creates = 0; static bool g_createFails = false;
struct CAudioPlatform {
    CVoicePool m_pool04, m_pool18, m_poolA, m_poolB;
    CPooledVoice* CreateVoiceA() { if (g_createFails) return 0; ++g_creates; return new CPooledVoice; }
    CPooledVoice* CreateVoiceB() { if (g_createFails) return 0; ++g_creates; return new CPooledVoice; }
    void Shutdown()
    {
        m_pool04.Clear();
        m_pool18.Clear();
        m_poolA.Clear();
        m_poolB.Clear();
        g_audioConfig = k_defaultAudioConfig;
        g_pAudioMaster = 0;
    }
};
#define CHECK(x) do { if (!(x)) { std::printf("00c4d1f0_TEST FAIL line %d\n", __LINE__); return 1; } } while (0)
int main()
{
    k_defaultAudioConfig.m_head[0] = -1; k_defaultAudioConfig.m_voicesA.m_spare = 8; k_defaultAudioConfig.m_preallocatedLeft = 16; k_defaultAudioConfig.m_tail[2] = 77;
    g_audioConfig.m_head[0] = 5; g_audioConfig.m_voicesA.m_spare = 1; g_audioConfig.m_preallocatedLeft = 0; g_audioConfig.m_tail[2] = 3;
    CAudioPlatform p; p.m_poolA.m_activeCount = 4; p.m_poolB.m_freeCount = 2;
    p.Shutdown();
    CHECK(p.m_pool04.m_clears == 1 && p.m_pool18.m_clears == 1 && p.m_poolA.m_clears == 1 && p.m_poolB.m_clears == 1);
    CHECK(p.m_poolA.m_activeCount == 0 && p.m_poolB.m_freeCount == -1);
    CHECK(g_audioConfig.m_head[0] == -1 && g_audioConfig.m_voicesA.m_spare == 8 && g_audioConfig.m_preallocatedLeft == 16 && g_audioConfig.m_tail[2] == 77);
    CHECK(g_pAudioMaster == 0);
    std::printf("00c4d1f0_TEST PASS\n");
    return 0;
}