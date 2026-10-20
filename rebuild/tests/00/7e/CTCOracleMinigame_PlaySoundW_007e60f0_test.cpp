// Behaviour test for CTCOracleMinigame::PlaySoundW (0x007E60F0): without a sound player or with an unknown symbol the invalid
// handle comes back; otherwise the player plays {index, index} with the default CSoundInit and (true, false, false).
#include <cstdio>
#include <cstring>
struct CTCBase;
struct CCharString { const char* PStringData; };
unsigned long g_InvalidSoundHandle = 0xFFFFFFFFUL;
struct CASoundBank {
    long GetSoundIndexFromSymbol(const CCharString& symbol) const { return std::strcmp(symbol.PStringData, "ORACLE") == 0 ? 42 : 0; }
};
struct CMainGameComponent { CASoundBank* PSampleBank; };
static CASoundBank g_bank; static CMainGameComponent g_mgc = { &g_bank };
CMainGameComponent* g_pMainGameComponent = &g_mgc;
struct CSoundPair {
    long FirstSound;
    long LastSound;
    CSoundPair(long first, long last) : FirstSound(first), LastSound(last) { }
};
enum ESoundPlaybackType { SOUND_PLAYBACK_1 = 1 };
struct CSoundInit {
    float Volume; float Pitch; float Pan; long NoPlays; ESoundPlaybackType PlaybackType;
    unsigned long GainFaderBits; unsigned long PitchFaderBits; long OverrideID;
    CSoundInit() : Volume(1.0f), Pitch(1.0f), Pan(0.0f), NoPlays(1), PlaybackType(SOUND_PLAYBACK_1), GainFaderBits(0),
                   PitchFaderBits(0), OverrideID(-1) { }
};
static int g_plays = 0; static long g_first = 0, g_last = 0; static int g_initOk = 0; static int g_flags = -1;
struct CTCSoundPlayer {
    unsigned long PlayAttachedSample(const CSoundPair& sound, const CSoundInit& init, bool a, bool b, bool c)
    {
        ++g_plays; g_first = sound.FirstSound; g_last = sound.LastSound;
        g_initOk = init.Volume == 1.0f && init.Pitch == 1.0f && init.Pan == 0.0f && init.NoPlays == 1 && init.PlaybackType == 1 &&
                   init.GainFaderBits == 0 && init.PitchFaderBits == 0 && init.OverrideID == -1;
        g_flags = (a ? 4 : 0) | (b ? 2 : 0) | (c ? 1 : 0);
        return 1234;
    }
};
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type) { SEntry* it = first; while (it != last && it->type < *type) ++it; return it; }
};
struct CThing {
    unsigned long m_presentBits[9];
    CInterfaceMap m_interfaces;
    template <class T> bool GetInterface(T** out, long type)
    {
        if (m_presentBits[type >> 5] & (1UL << (type & 31))) {
            long key = type;
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::SEntry* it = map->LowerBound(&key);
            if (it == map->last || type < it->type)
                it = map->last;
            *out = (T*)it->component;
            return true;
        }
        return false;
    }
};
struct CTCOracleMinigame {
    CThing* ParentThing;
    unsigned long PlaySoundW(const CCharString& sound);
};

unsigned long CTCOracleMinigame::PlaySoundW(const CCharString& sound)
{
    CTCSoundPlayer* player;
    if (ParentThing->GetInterface(&player, 0x10)) {
        CASoundBank* bank = g_pMainGameComponent->PSampleBank;
        long index = bank->GetSoundIndexFromSymbol(sound);
        if (index != 0) {
            return player->PlayAttachedSample(CSoundPair(index, index), CSoundInit(), true, false, false);
        }
    }
    return g_InvalidSoundHandle;
}

int main()
{
    int bad = 0;
    CTCSoundPlayer player;
    CInterfaceMap::SEntry entries[1] = { { 0x10, (CTCBase*)&player } };
    CThing thing; for (int i = 0; i < 9; ++i) thing.m_presentBits[i] = 0; thing.m_interfaces.first = entries; thing.m_interfaces.last = entries + 1;
    CTCOracleMinigame game; game.ParentThing = &thing;
    CCharString known = { "ORACLE" }, unknown = { "NOPE" };
    if (game.PlaySoundW(known) != g_InvalidSoundHandle || g_plays != 0) bad |= 1;    // no sound player
    thing.m_presentBits[0] = 1UL << 16;
    if (game.PlaySoundW(unknown) != g_InvalidSoundHandle || g_plays != 0) bad |= 2;
    if (game.PlaySoundW(known) != 1234 || g_plays != 1 || g_first != 42 || g_last != 42 || !g_initOk || g_flags != 4) bad |= 4;
    if (bad) { std::printf("007e60f0_TEST FAIL\n"); return 1; }
    std::printf("007e60f0_TEST PASS\n");
    return 0;
}