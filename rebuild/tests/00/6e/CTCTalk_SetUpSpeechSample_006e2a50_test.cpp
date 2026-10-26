// Behaviour test for 006e2a50: src/006e2a50.cpp copied verbatim; the speech bank map, the banks, the owner system and
// the clock stubbed so the sample lookup (by the speaker's symbol), the lip-sync preload and the timing fields are checked.
#include <cstdio>
#include <cstring>
static int fails = 0;
#define CHECK(c) do { if (!(c)) { ++fails; printf("FAIL %d: %s\n", __LINE__, #c); } } while (0)
// ---- source (verbatim) ----
// CTCTalk::SetUpSpeechSample() -- reset the talker's speech bank and sample, resolve the lip-sync bank for the default speech bank, look the speaker's sound symbol up in the matching sound bank, start loading that lip-sync entry, then stamp the mime start time and the sample duration
// Retry 10b (getsw sweep, FIXES B-15): the speaker info is read through an inline getter `GetSpeakerInfo()`; the plain member read gives the parked register/scheduling difference.
// Leaf: SetUpSpeechSample
// Retail: Fable.exe (TLC, VC7.1) @ 0x006E2A50, 152 bytes (+8 int3), __fastcall, no arguments.
// The catalog label (CAnimateThing::SetUpAnimation) is propagated: the members touched are CTCTalk's
// (Ego_r.pdb: SpeechSample +0x0C, SampleDurationFrames +0x14, MimeStartTime +0x1C, PSpeechBank +0x24, cf.
// the landed CTCTalk rows 0x006E28D0 / 0x006E2910) and the row sits between CTCTalk::DoesSpeechExist
// (0x006E2910) and its siblings 0x006E2940 (duration of a sample) and 0x006E29A0 (set bank and sample).
// Callees: 0x00686D70 (default speech bank getter on `this`, labelled CAnimateThing::GetMeshBank),
// CSymbolMap lookups 0x009916F0 (value for a key), 0x00991720 (first non-empty value), 0x00991690 (key for a
// value) on the global bank map @ 0x013B8394, 0x006869D0 (owner system getter on `this`),
// CASoundBank::GetSoundIndexFromSymbol(const CCharString&) 0x00A01920, CLipSyncDataBank::BeginLoadingEntry
// 0x00A42AC0, GFGetTime 0x009E1BC0 (__fastcall, double), 0x006E2940 (sample duration in frames).
// Shape: the bank lookup is an inline helper shared with 0x006E2940/0x006E29A0 (`bank ? bank : default`,
// the map re-read from the global for every call, the first-value fallback); with PSpeechBank just zeroed
// VC folds the test and calls the default getter directly with `this` still in ecx. The sample index is
// stored to the member and re-read for BeginLoadingEntry and the duration call (member, not local).
class CCharStringData;
class CCharString {
public:
    CCharStringData* m_pData;
};

class CASoundBank {
public:
    long GetSoundIndexFromSymbol(const CCharString& symbol);    // 0x00A01920
};

class CLipSyncDataBank {
public:
    void BeginLoadingEntry(unsigned long index);                 // 0x00A42AC0
};

class CSymbolMap {
public:
    void* FindValueForSymbol(long symbol);                       // 0x009916F0
    void* FindFirstValue();                                      // 0x00991720
    void* FindSymbolForValue(long value);                        // 0x00991690
};
extern CSymbolMap* g_pSpeechBankMap;                             // 0x013B8394

struct CSpeakerInfo {
    unsigned char _pad_0x00[0xAC];
    CCharString SpeechSymbol;                                    // +0xAC
};
struct COwnerSystem {
    unsigned char _pad_0x00[0xD0];
    CSpeakerInfo* PSpeakerInfo;                                  // +0xD0
    CSpeakerInfo* GetSpeakerInfo() const { return PSpeakerInfo; }
};

extern "C" double __fastcall GFGetTime();                        // 0x009E1BC0

class CThing;

class CTCTalk {
public:
    void* m_pVtable;                         // +0x00 CTCBase
    CThing* ParentThing;                     // +0x04
    bool ToBeRemoved;                        // +0x08
    bool IsInGlobalUpdateTCs;                // +0x09
    long SpeechSample;                       // +0x0C
    long SampleStartedFrame;                 // +0x10
    long SampleDurationFrames;               // +0x14
    long Attitude;                           // +0x18
    float MimeStartTime;                     // +0x1C
    float SampleLoadedTime;                  // +0x20
    CASoundBank* PSpeechBank;                // +0x24

    CASoundBank* GetDefaultSpeechBank();                         // 0x00686D70
    COwnerSystem* GetOwnerSystem();                              // 0x006869D0
    long GetSampleDurationFrames(long sample);                   // 0x006E2940

    CLipSyncDataBank* GetLipSyncBank()
    {
        if (!g_pSpeechBankMap)
            return 0;
        CASoundBank* bank = PSpeechBank ? PSpeechBank : GetDefaultSpeechBank();
        CLipSyncDataBank* lipSync = (CLipSyncDataBank*)g_pSpeechBankMap->FindValueForSymbol((long)bank);
        if (!lipSync)
            lipSync = (CLipSyncDataBank*)g_pSpeechBankMap->FindFirstValue();
        return lipSync;
    }

    void SetUpSpeechSample();
};

void CTCTalk::SetUpSpeechSample()
{
    PSpeechBank = 0;
    SpeechSample = 0;
    CLipSyncDataBank* lipSync = GetLipSyncBank();
    if (lipSync) {
        CASoundBank* soundBank = (CASoundBank*)g_pSpeechBankMap->FindSymbolForValue((long)lipSync);
        if (soundBank)
            SpeechSample = soundBank->GetSoundIndexFromSymbol(GetOwnerSystem()->GetSpeakerInfo()->SpeechSymbol);
        lipSync->BeginLoadingEntry(SpeechSample);
    }
    MimeStartTime = (float)GFGetTime();
    SampleDurationFrames = GetSampleDurationFrames(SpeechSample);
}
// ---- end source ----
static CASoundBank g_defaultBank, g_soundBank; static CLipSyncDataBank g_lipSync;
static bool g_mapHasLipSync = true, g_mapHasSound = true;
static const CCharString* g_symbol = 0; static unsigned long g_loaded = 0xffffffff; static long g_durationFor = -1;
long CASoundBank::GetSoundIndexFromSymbol(const CCharString& s) { g_symbol = &s; return 42; }
void CLipSyncDataBank::BeginLoadingEntry(unsigned long index) { g_loaded = index; }
void* CSymbolMap::FindValueForSymbol(long symbol) { return (g_mapHasLipSync && symbol == (long)&g_defaultBank) ? &g_lipSync : 0; }
void* CSymbolMap::FindFirstValue() { return g_mapHasLipSync ? &g_lipSync : 0; }
void* CSymbolMap::FindSymbolForValue(long value) { return (g_mapHasSound && value == (long)&g_lipSync) ? &g_soundBank : 0; }
static CSymbolMap g_map;
CSymbolMap* g_pSpeechBankMap = &g_map;
extern "C" double __fastcall GFGetTime() { return 12.5; }
static CSpeakerInfo g_speaker; static COwnerSystem g_owner;
CASoundBank* CTCTalk::GetDefaultSpeechBank() { return &g_defaultBank; }
COwnerSystem* CTCTalk::GetOwnerSystem() { return &g_owner; }
long CTCTalk::GetSampleDurationFrames(long sample) { g_durationFor = sample; return sample * 2; }
int main()
{
    g_owner.PSpeakerInfo = &g_speaker;
    CTCTalk talk; memset(&talk, 0, sizeof(talk)); talk.SpeechSample = 9; talk.PSpeechBank = &g_soundBank;
    talk.SetUpSpeechSample();
    CHECK(talk.PSpeechBank == 0 && talk.SpeechSample == 42 && g_symbol == &g_speaker.SpeechSymbol && g_loaded == 42);
    CHECK(talk.MimeStartTime == 12.5f && talk.SampleDurationFrames == 84 && g_durationFor == 42);
    g_mapHasSound = false; g_loaded = 0xffffffff;
    talk.SetUpSpeechSample();
    CHECK(talk.SpeechSample == 0 && g_loaded == 0 && talk.SampleDurationFrames == 0);
    g_mapHasLipSync = false; g_loaded = 0xffffffff;
    talk.SetUpSpeechSample();
    CHECK(talk.SpeechSample == 0 && g_loaded == 0xffffffff);
    g_pSpeechBankMap = 0;
    talk.SetUpSpeechSample();
    CHECK(talk.SpeechSample == 0 && talk.MimeStartTime == 12.5f);
    if (fails == 0) printf("006e2a50_TEST PASS\n"); else printf("006e2a50_TEST FAIL\n");
    return fails;
}