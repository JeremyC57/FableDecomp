// Behaviour test for 0x006E29A0 (CTCTalk lip-sync helpers: GetPLipSyncBank / GetSpeechDurationInFrames / PlayLipMovement).
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
#define FAIL { std::printf("006e29a0_TEST FAIL\n"); return 1; }
int main()
{
    Bank mine; LipBank a; a.count = 1; a.loads = 0; a.entries[0].Duration = 1.0f;
    SymbolMap m; m.n = 1; m.keys[0] = &mine; m.values[0] = &a; g_pSpeechBankMap = &m;
    Talk t; t.MimeStartTime = 9.0f; t.PSpeechBank = 0;
    t.PlayLipMovement(mine, 0);
    if (t.PSpeechBank != &mine || t.SpeechSample != 0 || t.MimeStartTime != 0.0f || a.loads != 1 || a.lastLoaded != 0 || t.SampleDurationFrames != 30) FAIL;
    g_pSpeechBankMap = 0;
    t.PlayLipMovement(mine, 5);
    if (a.loads != 1 || t.SampleDurationFrames != 150 || t.SpeechSample != 5) FAIL;
    std::printf("006e29a0_TEST PASS\n");
    return 0;
}