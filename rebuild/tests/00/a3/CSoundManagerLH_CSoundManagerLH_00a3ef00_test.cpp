// Behaviour test for 00a3ef00: CSoundManagerLH::CSoundManagerLH -- installs the three sound allocators, creates the sound
// system from the LH setup record (0x40 sounds, 0x200 voices, scale 15, flags 1, 10 MB budget, an LH file manager),
// sizes its buffers, initialises the player with the config and one SetChannel per bit of the channel mask, then creates
// the main emitter and the receiver and starts the player
#include <cstdio>
static int g_allocs = 0, g_setAlloc = 0, g_setTemp = 0, g_setPhys = 0, g_channels[8], g_nchannels = 0;
static int g_bufferSize = 0, g_enabled = 0, g_played = 0, g_v06 = 0, g_fileManagers = 0;
struct CAllocator { virtual ~CAllocator() { } };
struct CBBBSoundAllocator : CAllocator { CBBBSoundAllocator() { ++g_allocs; } };
struct CBBBSoundPhysicalAllocator : CAllocator { CBBBSoundPhysicalAllocator() { ++g_allocs; } };
static void SetSoundAllocator(CAllocator*) { ++g_setAlloc; } static void SetSoundTempAllocator(CAllocator*) { ++g_setTemp; } static void SetSoundPhysicalAllocator(CAllocator*) { ++g_setPhys; }
struct CLHFileManager { CLHFileManager() { ++g_fileManagers; } };
struct CSoundSystemSetup {
    unsigned long MaxSounds, MaxVoices; float Scale; unsigned long Flags, MemoryBudget; CLHFileManager* FileManager;
    CSoundSystemSetup() : MaxSounds(0), MaxVoices(0), Scale(1.0f), Flags(0), MemoryBudget(0), FileManager(0) { }
};
struct CSoundSystemConfig {
    long Long04, Long08, Long0C, Long10, Long14, Long18;
    void SetChannel(long bit, bool on) { if (on && g_nchannels < 8) g_channels[g_nchannels++] = (int)bit; }
};
class CPlayer { public: void Play(long) { ++g_played; } void v06(long) { ++g_v06; } void SetEnabled(bool) { ++g_enabled; } };
struct CSoundSystem {
    CSoundSystemSetup setup; CPlayer player; CSoundSystemConfig config;
    CSoundSystem(const CSoundSystemSetup* s) : setup(*s) { }
    void Initialise(const CSoundSystemConfig* c) { config = *c; }
    void SetBufferSize(long n) { g_bufferSize = (int)n; }
    CPlayer* GetPlayer() { return &player; }
};
static CSoundSystem* CreateSoundSystem(const CSoundSystemSetup* s) { return new CSoundSystem(s); }
inline long LowestSetBit(unsigned long mask) { for (unsigned long i = 0; i < 128; ++i) if (mask & (1UL << i)) return (long)i; return -1; }
struct CSoundEmitterLH { CSoundEmitterLH(void*) { } }; struct CSoundReceiverLH { CSoundReceiverLH(void*) { } };
struct CSoundManagerLH {
    unsigned long m_channelMask; CSoundSystem* m_pSoundSystem; CSoundEmitterLH* m_pMain; CSoundReceiverLH* m_pReceiver; unsigned long m_nextEmitterID;
    CSoundManagerLH(unsigned long channelMask) : m_channelMask(channelMask), m_nextEmitterID(0)
    {
        SetSoundAllocator(new CBBBSoundAllocator); SetSoundTempAllocator(new CBBBSoundAllocator); SetSoundPhysicalAllocator(new CBBBSoundPhysicalAllocator);
        CSoundSystemSetup setup;
        setup.MaxSounds = 0x40; setup.MaxVoices = 0x200; setup.Scale = 15.0f; setup.Flags = 1; setup.MemoryBudget = 0xA00000; setup.FileManager = new CLHFileManager;
        m_pSoundSystem = CreateSoundSystem(&setup);
        m_pSoundSystem->SetBufferSize(0x8000);
        m_pSoundSystem->GetPlayer()->v06(1);
        CSoundSystemConfig config; config.Long04 = 4; config.Long0C = 4; config.Long18 = 0; config.Long14 = 2; config.Long10 = 0x6000; config.Long08 = 0x800;
        unsigned long mask = m_channelMask;
        while (mask) { long bit = LowestSetBit(mask); config.SetChannel(bit, true); mask ^= 1UL << bit; }
        m_pSoundSystem->Initialise(&config);
        m_pMain = new CSoundEmitterLH(this); m_pReceiver = new CSoundReceiverLH(this);
        m_pSoundSystem->GetPlayer()->SetEnabled(true); m_pSoundSystem->GetPlayer()->Play(0);
    }
};
int main()
{
    CSoundManagerLH m(0x0B);
    CSoundSystem* s = m.m_pSoundSystem;
    bool ok = g_allocs == 3 && g_setAlloc == 1 && g_setTemp == 1 && g_setPhys == 1 && g_fileManagers == 1
           && s->setup.MaxSounds == 0x40 && s->setup.MaxVoices == 0x200 && s->setup.Scale == 15.0f && s->setup.Flags == 1 && s->setup.MemoryBudget == 0xA00000 && s->setup.FileManager != 0
           && g_bufferSize == 0x8000 && g_v06 == 1 && s->config.Long04 == 4 && s->config.Long08 == 0x800 && s->config.Long0C == 4 && s->config.Long10 == 0x6000 && s->config.Long14 == 2 && s->config.Long18 == 0
           && g_nchannels == 3 && g_channels[0] == 0 && g_channels[1] == 1 && g_channels[2] == 3
           && m.m_pMain != 0 && m.m_pReceiver != 0 && g_enabled == 1 && g_played == 1 && m.m_nextEmitterID == 0;
    std::printf(ok ? "00a3ef00_TEST PASS\n" : "00a3ef00_TEST FAIL\n");
    return ok ? 0 : 1;
}