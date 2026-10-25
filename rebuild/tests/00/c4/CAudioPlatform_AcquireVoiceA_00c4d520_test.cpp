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
    int AcquireVoiceA(CSoundDesc* desc, CPooledVoice** out)
    {
        CVoicePool* pool = &m_poolA;
        CPooledVoice* voice;
        int result;
        if (g_audioConfig.m_voicesA.m_spare > 0) {
            voice = pool->GetFreeHead();
            result = voice->Init(desc);
            if (result == 0) {
                pool->PopFree();
                pool->PushActive(voice);
                --g_audioConfig.m_voicesA.m_spare;
                if (g_audioConfig.m_preallocatedLeft > 0)
                    --g_audioConfig.m_preallocatedLeft;
            } else {
                voice = 0;
            }
        } else {
            voice = CreateVoiceA();
            if (!voice) {
                result = -5;
            } else {
                result = voice->Init(desc);
                if (result == 0) {
                    pool->PushActive(voice);
                } else {
                    delete voice;
                    voice = 0;
                }
            }
        }
        *out = voice;
        return result;
    }
};
#define CHECK(x) do { if (!(x)) { std::printf("00c4d520_TEST FAIL line %d\n", __LINE__); return 1; } } while (0)
int main()
{
    CAudioPlatform p; CSoundDesc ok; ok.initResult = 0; CSoundDesc bad; bad.initResult = -7; CPooledVoice* out = (CPooledVoice*)1;
    // recycle path: two preallocated voices on the free chain
    CPooledVoice f1, f2; f1.m_pNext = &f2; p.m_poolA.m_pFreeHead = &f1; p.m_poolA.m_freeCount = 2;
    g_audioConfig.m_voicesA.m_spare = 2; g_audioConfig.m_voicesB.m_spare = 9; g_audioConfig.m_preallocatedLeft = 1;
    CHECK(p.AcquireVoiceA(&ok, &out) == 0 && out == &f1 && f1.m_onActive && p.m_poolA.m_pFreeHead == &f2 && p.m_poolA.m_activeCount == 1);
    CHECK(g_audioConfig.m_voicesA.m_spare == 1 && g_audioConfig.m_voicesB.m_spare == 9 && g_audioConfig.m_preallocatedLeft == 0);
    CHECK(p.AcquireVoiceA(&ok, &out) == 0 && out == &f2 && g_audioConfig.m_voicesA.m_spare == 0 && g_audioConfig.m_preallocatedLeft == 0);
    // recycled voice whose Init fails: no pop, no count change, null out, no delete
    CPooledVoice f3; p.m_poolA.m_pFreeHead = &f3; g_audioConfig.m_voicesA.m_spare = 1; out = (CPooledVoice*)1;
    CHECK(p.AcquireVoiceA(&bad, &out) == -7 && out == 0 && p.m_poolA.m_pFreeHead == &f3 && g_audioConfig.m_voicesA.m_spare == 1 && g_deletes == 0);
    // create path
    g_audioConfig.m_voicesA.m_spare = 0; int act = p.m_poolA.m_activeCount;
    CHECK(p.AcquireVoiceA(&ok, &out) == 0 && out != 0 && out->m_onActive && g_creates == 1 && p.m_poolA.m_activeCount == act + 1);
    out = (CPooledVoice*)1;
    CHECK(p.AcquireVoiceA(&bad, &out) == -7 && out == 0 && g_creates == 2 && g_deletes == 1 && p.m_poolA.m_activeCount == act + 1);
    g_createFails = true; out = (CPooledVoice*)1;
    CHECK(p.AcquireVoiceA(&ok, &out) == -5 && out == 0 && g_deletes == 1);
    CHECK(p.m_poolB.m_activeCount == 0);
    std::printf("00c4d520_TEST PASS\n");
    return 0;
}