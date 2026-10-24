// Behaviour test for 0x006E2860 (CTCTalk lip-sync helpers: GetPLipSyncBank / GetSpeechDurationInFrames / PlayLipMovement).
#include <cstdio>

struct Bank { int id; };
struct Entry { void* data; float Duration; };
struct LipBank {
    int loads; unsigned int lastLoaded; Entry entries[4]; int count;
    const Entry* GetPEntry(unsigned int i) const { return i < (unsigned int)count ? &entries[i] : 0; }
    void BeginLoadingEntry(unsigned int i) { ++loads; lastLoaded = i; }
};
struct SymbolMap {
    Bank* keys[4]; LipBank* values[4]; int n;
    void* FindValueForSymbol(long s) { for (int i = 0; i < n; ++i) if ((long)(size_t)keys[i] == s) return values[i]; return 0; }
    void* FindFirstValue() { return n ? values[0] : 0; }
};
static SymbolMap* g_pSpeechBankMap = 0;
static long g_framesPerSecond = 30;
static long g_DefaultSpeechDurationFrames = 150;
static Bank g_defaultBank;

struct Talk {
    long SpeechSample; long SampleDurationFrames; float MimeStartTime; Bank* PSpeechBank;
    Bank* GetDefaultSpeechBank() const { return &g_defaultBank; }
    const LipBank* GetPLipSyncBank() const
    {
        const LipBank* lip = 0;
        if (g_pSpeechBankMap) {
            Bank* bank = PSpeechBank ? PSpeechBank : GetDefaultSpeechBank();
            lip = (const LipBank*)g_pSpeechBankMap->FindValueForSymbol((long)(size_t)bank);
            if (!lip) lip = (const LipBank*)g_pSpeechBankMap->FindFirstValue();
        }
        return lip;
    }
    long GetSpeechDurationInFrames(long sample) const
    {
        const LipBank* lip = GetPLipSyncBank();
        if (lip) { const Entry* e = lip->GetPEntry(sample); if (e) return (long)(e->Duration * g_framesPerSecond); }
        return g_DefaultSpeechDurationFrames;
    }
    void PlayLipMovement(Bank& bank, long sample)
    {
        PSpeechBank = &bank; SpeechSample = sample; MimeStartTime = 0.0f;
        const LipBank* lip = GetPLipSyncBank();
        if (lip) const_cast<LipBank*>(lip)->BeginLoadingEntry(SpeechSample);
        SampleDurationFrames = GetSpeechDurationInFrames(SpeechSample);
    }
};
#define FAIL { std::printf("006e2860_TEST FAIL\n"); return 1; }
int main()
{
    Talk t; t.PSpeechBank = 0;
    if (t.GetPLipSyncBank() != 0) FAIL;
    Bank mine; LipBank a, b; a.count = b.count = 0;
    SymbolMap m; m.n = 2; m.keys[0] = &g_defaultBank; m.values[0] = &a; m.keys[1] = &mine; m.values[1] = &b;
    g_pSpeechBankMap = &m;
    if (t.GetPLipSyncBank() != &a) FAIL;
    t.PSpeechBank = &mine;
    if (t.GetPLipSyncBank() != &b) FAIL;
    Bank other; t.PSpeechBank = &other;
    if (t.GetPLipSyncBank() != &a) FAIL;
    std::printf("006e2860_TEST PASS\n");
    return 0;
}