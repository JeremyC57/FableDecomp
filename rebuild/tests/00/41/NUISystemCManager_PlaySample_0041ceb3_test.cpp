#include <cstdio>
// Behaviour test for NUISystem::CManager::PlaySample (retail 0x0041CEB3): 0 without a sound system, while a definition
// manager is set, for an unknown bank or an unknown symbol (index 0); otherwise the sound player's index overload is
// called with the bank, the index, loop = true, a CSoundInit with GainFaderBits 0x40 and attached = false.
struct CCharString { long m_id; };
struct CASoundBank { long m_base; long GetSoundIndexFromSymbol(const CCharString& s) { return s.m_id ? m_base + s.m_id : 0; } };
struct CSoundInit { unsigned long GainFaderBits; CSoundInit() : GainFaderBits(0) {} };
static int g_calls = 0; static CASoundBank* g_bank = 0; static long g_index = 0; static bool g_loop = false, g_attached = true;
static unsigned long g_gain = 0;
struct CSoundPlayer {
    unsigned long PlaySoundEvent(CASoundBank& bank, long index, bool loop, CSoundInit& init, bool attached)
    { ++g_calls; g_bank = &bank; g_index = index; g_loop = loop; g_gain = init.GainFaderBits; g_attached = attached; return 0x1234; }
};
static CSoundPlayer g_player;
static CASoundBank g_banks[2];
struct CSoundSystem {
    CSoundPlayer* GetSoundPlayer() { return &g_player; }
    CASoundBank* GetBank(long id) { return id >= 0 && id < 2 ? &g_banks[id] : 0; }
};
static CSoundSystem g_system;
static CSoundSystem* g_pSoundSystem = 0;
struct CManager {
    void* m_pDefinitionManager;
    unsigned long PlaySample(CCharString& symbol, long bankId)
    {
        if (g_pSoundSystem && !m_pDefinitionManager) {
            CASoundBank* bank = g_pSoundSystem->GetBank(bankId);
            if (bank) {
                long index = bank->GetSoundIndexFromSymbol(symbol);
                if (index) {
                    CSoundInit init;
                    init.GainFaderBits = 0x40;
                    return g_pSoundSystem->GetSoundPlayer()->PlaySoundEvent(*bank, index, true, init, false);
                }
            }
        }
        return 0;
    }
};
int main()
{
    g_banks[0].m_base = 100; g_banks[1].m_base = 200;
    CManager m; m.m_pDefinitionManager = 0;
    CCharString known; known.m_id = 5; CCharString unknown; unknown.m_id = 0;
    int ok = m.PlaySample(known, 1) == 0 && g_calls == 0;            // no sound system yet
    g_pSoundSystem = &g_system;
    ok = ok && m.PlaySample(known, 1) == 0x1234 && g_calls == 1 && g_bank == &g_banks[1] && g_index == 205
            && g_loop && !g_attached && g_gain == 0x40;
    ok = ok && m.PlaySample(unknown, 0) == 0 && m.PlaySample(known, 7) == 0 && g_calls == 1;
    int dummy; m.m_pDefinitionManager = &dummy;
    ok = ok && m.PlaySample(known, 0) == 0 && g_calls == 1;
    if (!ok) { std::printf("0041ceb3_TEST FAIL\n"); return 1; }
    std::printf("0041ceb3_TEST PASS\n");
    return 0;
}