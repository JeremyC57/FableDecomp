#include <cstdio>
// Behaviour test for NUISystem::CManager::PlaySoundW (retail 0x0041D0A8): nothing without a sound system; with the new
// front end running its own sample bank is used (bankId ignored, nothing when it has none); otherwise the registry
// bank bankId (nothing when unknown); the string overload gets loop = true, GainFaderBits 0x20, attached = false.
struct CCharString { long m_id; };
struct CASoundBank { long m_tag; };
struct CSoundInit { unsigned long GainFaderBits; CSoundInit() : GainFaderBits(0) {} };
static int g_calls = 0; static CASoundBank* g_bank = 0; static const CCharString* g_name = 0;
static bool g_loop = false, g_attached = true; static unsigned long g_gain = 0;
struct CSoundPlayer {
    unsigned long PlaySoundEvent(CASoundBank& bank, const CCharString& name, bool loop, CSoundInit& init, bool attached)
    { ++g_calls; g_bank = &bank; g_name = &name; g_loop = loop; g_gain = init.GainFaderBits; g_attached = attached; return 1; }
};
static CSoundPlayer g_player;
static CASoundBank g_banks[2];
struct CSoundSystem {
    CSoundPlayer* GetSoundPlayer() { return &g_player; }
    CASoundBank* GetBank(long id) { return id >= 0 && id < 2 ? &g_banks[id] : 0; }
};
static CSoundSystem g_system;
static CSoundSystem* g_pSoundSystem = 0;
struct CNewFrontendGameComponent { CASoundBank* PSampleBank; };
static CNewFrontendGameComponent* g_pNewFrontendGameComponent = 0;
struct CManager {
    void PlaySoundW(CCharString& symbol, long bankId)
    {
        CSoundSystem* soundSystem = g_pSoundSystem;
        if (soundSystem) {
            CSoundInit init;
            init.GainFaderBits = 0x20;
            if (g_pNewFrontendGameComponent) {
                CASoundBank* bank = g_pNewFrontendGameComponent->PSampleBank;
                if (bank)
                    soundSystem->GetSoundPlayer()->PlaySoundEvent(*bank, symbol, true, init, false);
            } else {
                CASoundBank* bank = soundSystem->GetBank(bankId);
                if (bank)
                    g_pSoundSystem->GetSoundPlayer()->PlaySoundEvent(*bank, symbol, true, init, false);
            }
        }
    }
};
int main()
{
    CManager m; CCharString s; s.m_id = 3;
    m.PlaySoundW(s, 0);
    int ok = g_calls == 0;
    g_pSoundSystem = &g_system;
    m.PlaySoundW(s, 1);
    ok = ok && g_calls == 1 && g_bank == &g_banks[1] && g_name == &s && g_loop && !g_attached && g_gain == 0x20;
    m.PlaySoundW(s, 9);
    ok = ok && g_calls == 1;
    CASoundBank frontEndBank; CNewFrontendGameComponent fe; fe.PSampleBank = 0; g_pNewFrontendGameComponent = &fe;
    m.PlaySoundW(s, 0);
    ok = ok && g_calls == 1;
    fe.PSampleBank = &frontEndBank;
    m.PlaySoundW(s, 9);
    ok = ok && g_calls == 2 && g_bank == &frontEndBank;
    if (!ok) { std::printf("0041d0a8_TEST FAIL\n"); return 1; }
    std::printf("0041d0a8_TEST PASS\n");
    return 0;
}