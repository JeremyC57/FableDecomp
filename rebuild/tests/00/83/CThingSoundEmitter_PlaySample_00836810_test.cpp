#include <cstdio>
class CASoundBank { public: int id; };
class CSoundInit { public: int id; };
class CCharString { public: const char* m_pData; };
static int g_stateObj;
void*         g_pGameState = 0;
unsigned long g_InvalidSoundHandle = 0xFFFFFFFFUL;
struct CSoundPlayer {
    int m_calls; long m_lastSample; unsigned long m_lastId; bool m_loaded, m_playing;
    virtual unsigned long PlaySample(CASoundBank& bank, long sample, bool loop, const CSoundInit& init, bool attached)
    { (void)bank; (void)loop; (void)init; (void)attached; ++m_calls; m_lastSample = sample; return 100; }
    virtual unsigned long PlaySoundEvent(CASoundBank& bank, const CCharString& name, bool loop, CSoundInit& init, bool attached)
    { (void)bank; (void)name; (void)loop; (void)init; (void)attached; ++m_calls; return 200; }
    virtual bool IsSampleLoadedAndPlaying(unsigned long sampleID) const { (void)sampleID; return m_loaded; }
    virtual bool IsSamplePlaying(unsigned long sampleID) const { (void)sampleID; return m_playing; }
};
class CThingSoundEmitter { public:
    unsigned char  Dead : 1;
    unsigned char  _flags91_1 : 7;
    CSoundPlayer*  m_pSoundPlayer;
    bool IsActive() const { return g_pGameState && !Dead; }
    unsigned long PlaySample(CASoundBank& bank, long sample, bool loop, const CSoundInit& init, bool attached);
    unsigned long PlaySoundEvent(CASoundBank& bank, const CCharString& name, bool loop, CSoundInit& init, bool attached);
    bool IsSamplePlaying(unsigned long sampleID) const;
    bool IsSampleLoadedAndPlaying(unsigned long sampleID) const;
};
unsigned long CThingSoundEmitter::PlaySample(CASoundBank& bank, long sample, bool loop, const CSoundInit& init, bool attached)
{
    if (IsActive())
        return m_pSoundPlayer->PlaySample(bank, sample, loop, init, attached);
    return g_InvalidSoundHandle;
}
int main()
{
    int bad = 0;
    CSoundPlayer p; p.m_calls = 0; p.m_lastSample = 0; p.m_lastId = 0; p.m_loaded = true; p.m_playing = true;
    CThingSoundEmitter e; e.Dead = 0; e._flags91_1 = 0x7f; e.m_pSoundPlayer = &p;
    CASoundBank bank; CSoundInit init; CCharString name; name.m_pData = "EV";
    (void)bank; (void)init; (void)name;
    if (e.PlaySample(bank, 5, false, init, true) != 0xFFFFFFFFUL || p.m_calls) bad = 1;
    g_pGameState = &g_stateObj;
    if (e.PlaySample(bank, 5, false, init, true) != 100 || p.m_lastSample != 5) bad = 2;
    e.Dead = 1;
    if (e.PlaySample(bank, 6, false, init, true) != 0xFFFFFFFFUL || p.m_calls != 1) bad = 3;
    if (bad) { std::printf("00836810_TEST FAIL\n"); return 1; }
    std::printf("00836810_TEST PASS\n");
    return 0;
}