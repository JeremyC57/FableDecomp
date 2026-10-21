// Behaviour test for 0088f570: src/0088f570.cpp copied verbatim (shared headers expanded inline), callees stubbed below so calls
// and arguments are observable. CGameScriptInterface::Play2DSound(const CCharString& symbol) const
#include <cstdio>
#include <cstring>
static int fails = 0;
#define CHECK(c) do { if (!(c)) { ++fails; printf("FAIL %d: %s\n", __LINE__, #c); } } while (0)
// ---- source (verbatim) ----
// CGameScriptInterface::Play2DSound(const CCharString& symbol) const -- plays the engine sound bank's sample for the symbol once, unpositioned, through the game state's sound player; the invalid sound handle when the symbol has no sample
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0088F570, length 145 bytes.
// Callees: CASoundBank::GetSoundIndexFromSymbol @ 0x00A01920 on the engine's sound bank (g_pEngine @ 0x013B86A0, +0x10), the game state's (0x013B8394) virtual slot 9 (+0x24, the sound player),
// the player's virtual slot 3 (+0x0c, PlaySample(bank, sample, loop, init, attached)); the invalid handle global @ 0x0129D25C.
// Shape: the CSoundInit default constructor of the landed PlaySoundAtPos 0x00891170 (Volume/Pitch/Pan/NoPlays/
// PitchFaderBits/OverrideID in its list, PlaybackType and GainFaderBits assigned in its body), the caller then sets
// GainFaderBits = 0x8010 (retail stores +0,+4,+8,+c,+18,+1c,+10,+14 and CSEs the constant 1 into edi for NoPlays,
// PlaybackType and the `attached` push); the sound bank comes through an inline GetSampleBank() (the engine global is
// loaded before the symbol argument) and is re-read from the global for the PlaySample argument.
// Leaf: Play2DSound
#include <string.h>
struct CThing_Retail;
struct CScriptThing;
// CCharString (Ego_r.pdb layout, as the landed CCharString::operator!= 0x0099E960): a 4-byte handle on a 17-byte block.
#pragma pack(push, 1)
struct CBasicStringChar {                   // CBasicString<char>, 13 bytes
    char*         PData;                    // +0x00
    unsigned long StringLength;             // +0x04
    unsigned long DataLength : 31;          // +0x08
    bool          UseFastExtend : 1;        // +0x0c
    bool operator==(const char* text) const;    // 0x004115A0
};
struct CCharStringData {                    // 17 bytes
    CBasicStringChar Data;                  // +0x00
    long             NoRefs;                // +0x0d
};
#pragma pack(pop)
class CCharString {
public:
    CCharStringData* PStringData;           // +0x00
    CCharString(const char* text, long length);         // 0x0099EBF0
    ~CCharString();                                     // 0x0099EAE0
    CCharString& operator=(const CCharString& other);   // 0x0099EFB0
    bool operator==(const char* text) const
    {
        if (PStringData == 0)
            return strcmp(text, "") == 0;
        return PStringData->Data == text;
    }
    bool IsEmpty() const { return PStringData == 0 || PStringData->Data.StringLength == 0; }
};
struct CSoundInit {                                 // Ego_r.pdb CSoundInit, 0x20 bytes
    float         Volume;                           // +0x00
    float         Pitch;                            // +0x04
    float         Pan;                              // +0x08
    long          NoPlays;                          // +0x0c
    long          PlaybackType;                     // +0x10 (ESoundPlaybackType)
    unsigned long GainFaderBits;                    // +0x14
    unsigned long PitchFaderBits;                   // +0x18
    long          OverrideID;                       // +0x1c
    CSoundInit() : Volume(1.0f), Pitch(1.0f), Pan(0.0f), NoPlays(1), PitchFaderBits(0), OverrideID(-1)
    {
        PlaybackType = 1;
        GainFaderBits = 0;
    }
};
struct CASoundBank_Retail {
    long GetSoundIndexFromSymbol(const CCharString& symbol);   // 0x00A01920
};
struct CSoundPlayer_Retail {
    virtual void v000(); virtual void v001(); virtual void v002();
    virtual long PlaySample(CASoundBank_Retail& bank, long sample, bool loop, const CSoundInit& init, bool attached);   // slot 3 (+0x0c)
};
struct CGameState_Retail {
    virtual void s000(); virtual void s001(); virtual void s002(); virtual void s003(); virtual void s004(); virtual void s005(); virtual void s006(); virtual void s007();
    virtual void s008();
    virtual CSoundPlayer_Retail* GetSoundPlayer();  // slot 9 (+0x24)
};
struct CEngine_Retail {
    unsigned char       _pad00[0x10];
    CASoundBank_Retail* m_pSoundBank;               // +0x10
    CASoundBank_Retail& GetSampleBank() const { return *m_pSoundBank; }
};
extern CEngine_Retail*    g_pEngine;                // 0x013B86A0
extern CGameState_Retail* g_pGameState;             // 0x013B8394
extern long               g_InvalidSoundHandle;     // 0x0129D25C
struct CWorld;
struct CPlayerManager_Retail;
struct CDefinitionManager_Retail;
struct CGameScriptInterface {
    virtual void v000();
    CWorld*                    World;               // +0x04 (engine/CGameScriptInterface.h)
    void*                      Component;           // +0x08
    void*                      DisplayEngine;       // +0x0c
    CDefinitionManager_Retail* DefinitionManager;   // +0x10
    CPlayerManager_Retail*     PlayerManager;       // +0x14
    long Play2DSound(const CCharString& symbol) const;
};

long CGameScriptInterface::Play2DSound(const CCharString& symbol) const
{
    long sample = g_pEngine->GetSampleBank().GetSoundIndexFromSymbol(symbol);
    if (sample) {
        CSoundInit init;
        init.GainFaderBits = 0x8010;
        return g_pGameState->GetSoundPlayer()->PlaySample(g_pEngine->GetSampleBank(), sample, false, init, true);
    }
    return g_InvalidSoundHandle;
}

// ---- stubs and driver ----
void CSoundPlayer_Retail::v000() { } void CSoundPlayer_Retail::v001() { } void CSoundPlayer_Retail::v002() { }
long CSoundPlayer_Retail::PlaySample(CASoundBank_Retail&, long, bool, const CSoundInit&, bool) { return 0; }
void CGameState_Retail::s000() { } void CGameState_Retail::s001() { } void CGameState_Retail::s002() { } void CGameState_Retail::s003() { }
void CGameState_Retail::s004() { } void CGameState_Retail::s005() { } void CGameState_Retail::s006() { } void CGameState_Retail::s007() { }
void CGameState_Retail::s008() { } CSoundPlayer_Retail* CGameState_Retail::GetSoundPlayer() { return 0; }
CCharString::CCharString(const char*, long) : PStringData(0) { }
CCharString::~CCharString() { }
CCharString& CCharString::operator=(const CCharString&) { return *this; }
bool CBasicStringChar::operator==(const char*) const { return false; }
static long g_index = 0; static const CCharString* g_symbol = 0;
long CASoundBank_Retail::GetSoundIndexFromSymbol(const CCharString& s) { g_symbol = &s; return g_index; }
static CASoundBank_Retail g_bank;
static CEngine_Retail g_engine;
CEngine_Retail* g_pEngine = &g_engine;
long g_InvalidSoundHandle = -77;
static int g_plays = 0; static CASoundBank_Retail* g_pbBank; static long g_pbSample; static bool g_pbLoop, g_pbAttached; static CSoundInit g_pbInit;
struct TestPlayer : CSoundPlayer_Retail {
    virtual void v000() { } virtual void v001() { } virtual void v002() { }
    virtual long PlaySample(CASoundBank_Retail& bank, long sample, bool loop, const CSoundInit& init, bool attached)
    { ++g_plays; g_pbBank = &bank; g_pbSample = sample; g_pbLoop = loop; g_pbInit = init; g_pbAttached = attached; return 1234; }
};
static TestPlayer g_player; static int g_getPlayer = 0;
struct TestState : CGameState_Retail {
    virtual void s000() { } virtual void s001() { } virtual void s002() { } virtual void s003() { } virtual void s004() { }
    virtual void s005() { } virtual void s006() { } virtual void s007() { } virtual void s008() { }
    virtual CSoundPlayer_Retail* GetSoundPlayer() { ++g_getPlayer; return &g_player; }
};
static TestState g_state;
CGameState_Retail* g_pGameState = &g_state;
void CGameScriptInterface::v000() { }
int main()
{
    g_engine.m_pSoundBank = &g_bank;
    CGameScriptInterface gsi;
    CCharString sym("X", -1);
    g_index = 0;
    CHECK(gsi.Play2DSound(sym) == -77);
    CHECK(g_symbol == &sym && g_plays == 0 && g_getPlayer == 0);
    g_index = 9;
    CHECK(gsi.Play2DSound(sym) == 1234);
    CHECK(g_plays == 1 && g_getPlayer == 1 && g_pbBank == &g_bank && g_pbSample == 9 && !g_pbLoop && g_pbAttached);
    CHECK(g_pbInit.Volume == 1.0f && g_pbInit.Pitch == 1.0f && g_pbInit.Pan == 0.0f && g_pbInit.NoPlays == 1);
    CHECK(g_pbInit.PlaybackType == 1 && g_pbInit.GainFaderBits == 0x8010 && g_pbInit.PitchFaderBits == 0 && g_pbInit.OverrideID == -1);
    if (fails == 0) printf("0088f570_TEST PASS\n");
    return fails;
}
