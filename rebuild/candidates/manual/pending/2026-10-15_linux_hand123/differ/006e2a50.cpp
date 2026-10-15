// CTCTalk::SetUpSpeechSample() -- reset the talker's speech bank and sample, resolve the lip-sync bank for the default speech bank, look the speaker's sound symbol up in the matching sound bank, start loading that lip-sync entry, then stamp the mime start time and the sample duration
// Grade: DIFFER(152v152) -- register choice only: the two re-reads of SpeechSample for BeginLoadingEntry and the duration call load into ecx/edx where retail uses eax/ecx (tried: float local for the time, GetSampleDurationFrames defined above under auto_inline(off))
// Leaf: SetUpSpeechSample
// Retail: Fable.exe (TLC, VC7.1) @ 0x006E2A50, 152 bytes (+8 int3), __thiscall, no arguments.
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
struct CCharStringData;
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
            SpeechSample = soundBank->GetSoundIndexFromSymbol(GetOwnerSystem()->PSpeakerInfo->SpeechSymbol);
        lipSync->BeginLoadingEntry(SpeechSample);
    }
    MimeStartTime = (float)GFGetTime();
    SampleDurationFrames = GetSampleDurationFrames(SpeechSample);
}
