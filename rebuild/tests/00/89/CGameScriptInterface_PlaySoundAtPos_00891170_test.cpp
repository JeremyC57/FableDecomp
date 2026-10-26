// Behaviour test for 0x00891170 CGameScriptInterface::PlaySoundAtPos: unknown sample, emitter creation failure, dead
// emitter, and the played sample with the script CSoundInit.
#include <cstdio>
#include <cstring>
class CCharString { public: const char* text; };
class C3DVector { public: float X, Y, Z; };
class CThing;
class CASoundBank { public:
    long GetSoundIndexFromSymbol(const CCharString& symbol) { return std::strcmp(symbol.text, "SND_DOOR") == 0 ? 12 : 0; }
};
class CSoundInit { public:
    float Volume, Pitch, Pan; long NoPlays, PlaybackType; unsigned long GainFaderBits, PitchFaderBits; long OverrideID;
    CSoundInit() : Volume(1.0f), Pitch(1.0f), Pan(0.0f), NoPlays(1), PitchFaderBits(0), OverrideID(-1) { PlaybackType = 1; GainFaderBits = 0; }
};
static int g_created = 0, g_failCreate = 0, g_makeDead = 0;
static CSoundInit g_lastInit; static long g_lastSample; static C3DVector g_lastPos; static CASoundBank* g_lastBank; static bool g_lastFlags[2];
class CThingSoundEmitter { public:
    unsigned char Dead : 1;
    unsigned char _flags91_1 : 7;
    static CThingSoundEmitter* Create(const C3DVector& position, const CThing* parent, bool unattached)
    {
        static CThingSoundEmitter e;
        ++g_created; g_lastPos = position;
        if (g_failCreate || parent || !unattached) return 0;
        e.Dead = g_makeDead ? 1 : 0; return &e;
    }
    unsigned long PlaySample(CASoundBank& bank, long sample, bool loop, const CSoundInit& init, bool attached)
    { g_lastBank = &bank; g_lastSample = sample; g_lastInit = init; g_lastFlags[0] = loop; g_lastFlags[1] = attached; return 0x1234; }
};
class CMainGameComponent { public: CASoundBank* PSampleBank; CASoundBank& GetSampleBank() const { return *PSampleBank; } };
static CMainGameComponent* g_pMainGameComponent;
static unsigned long g_InvalidSoundHandle = 0xffffffff;
class CGameScriptInterface { public: unsigned long PlaySoundAtPos(const C3DVector& position, const CCharString& sound) const; };
unsigned long CGameScriptInterface::PlaySoundAtPos(const C3DVector& position, const CCharString& sound) const
{
    long sample = g_pMainGameComponent->GetSampleBank().GetSoundIndexFromSymbol(sound);
    if (sample) {
        CThingSoundEmitter* emitter = CThingSoundEmitter::Create(position, 0, true);
        if (emitter && !emitter->Dead) {
            CSoundInit init;
            init.GainFaderBits = 0x8010;
            return emitter->PlaySample(g_pMainGameComponent->GetSampleBank(), sample, false, init, false);
        }
    }
    return g_InvalidSoundHandle;
}
int main()
{
    CASoundBank bank; CMainGameComponent mgc = { &bank }; g_pMainGameComponent = &mgc;
    CGameScriptInterface gsi; C3DVector pos = { 1.0f, 2.0f, 3.0f };
    CCharString bad = { "SND_NOPE" }, good = { "SND_DOOR" };
    int fail = 0;
    if (gsi.PlaySoundAtPos(pos, bad) != 0xffffffff || g_created != 0) fail |= 1;
    g_failCreate = 1;
    if (gsi.PlaySoundAtPos(pos, good) != 0xffffffff || g_created != 1) fail |= 2;
    g_failCreate = 0; g_makeDead = 1;
    if (gsi.PlaySoundAtPos(pos, good) != 0xffffffff) fail |= 4;
    g_makeDead = 0;
    if (gsi.PlaySoundAtPos(pos, good) != 0x1234 || g_lastSample != 12 || g_lastBank != &bank || g_lastPos.Z != 3.0f) fail |= 8;
    if (g_lastInit.Volume != 1.0f || g_lastInit.Pitch != 1.0f || g_lastInit.Pan != 0.0f || g_lastInit.NoPlays != 1 ||
        g_lastInit.PlaybackType != 1 || g_lastInit.GainFaderBits != 0x8010 || g_lastInit.PitchFaderBits != 0 ||
        g_lastInit.OverrideID != -1 || g_lastFlags[0] || g_lastFlags[1]) fail |= 16;
    if (fail) { std::printf("00891170_TEST FAIL\n"); return 1; }
    std::printf("00891170_TEST PASS\n");
    return 0;
}