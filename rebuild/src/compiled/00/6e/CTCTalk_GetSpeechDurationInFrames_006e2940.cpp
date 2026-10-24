// CTCTalk::GetSpeechDurationInFrames(long sample) const -- a speech sample's length in frames: the lip-sync entry's duration (seconds) times the frame rate, or the default speech duration when there is no lip-sync bank or entry
// Leaf: ?GetSpeechDurationInFrames@CTCTalk@@
// Retail: Fable.exe (TLC, VC7.1) @ 0x006E2940, 93 bytes (+ int3 padding), thiscall `ret 4`.
// Identity: Ego_r.pdb name set `?GetSpeechDurationInFrames@CTCTalk@@ABEJJ@Z` (private const, long(long)); the landed
//   CTCTalk::SetUpSpeechSample 0x006E2A50 calls it as "GetSampleDurationFrames" (call site 0x006E2ADE), the bank setter
//   0x006E29A0 at 0x006E29FD. The catalog's "CAnimateThing_FindAndAddMeshSymbol" is descriptive.
// Callees: the default speech bank getter @ 0x00686D70, CSymbolMap::FindValueForSymbol @ 0x009916F0 / FindFirstValue
//   @ 0x00991720 (the inlined GetPLipSyncBank 0x006E2860), CLipSyncDataBank::GetPEntry(unsigned long) @ 0x00A42830 (landed),
//   __ftol2 @ 0x00BFEA70 (the float-to-long conversion).
// Globals: the speech-bank map @ 0x013B8394, g_framesPerSecond @ 0x01375550, the default speech duration in frames
//   @ 0x013BAB90 (statically initialised to 5 * g_framesPerSecond, landed initialiser 0x0121E3D0).
// Layout: CTCTalk (Ego_r.pdb) PSpeechBank +0x24; the 8-byte lip-sync entry holds its duration (float seconds) at +4.
// Shape: GetPLipSyncBank inlined (its zero result threads straight to the default exit); `duration * fps` loads the
//   converted global first (`fild ; fmul [eax+4]`); two `ret 4` exits.
class CASoundBank;
class CThing;

struct CLipSyncEntry {                                      // 8 bytes
    void* m_pData;                                          // +0x00
    float Duration;                                         // +0x04 seconds
};

class CLipSyncDataBank {
public:
    const CLipSyncEntry* GetPEntry(unsigned long index) const;   // 0x00A42830
};

class CSymbolMap {
public:
    void* FindValueForSymbol(long symbol);                  // 0x009916F0
    void* FindFirstValue();                                 // 0x00991720
};
extern CSymbolMap* g_pSpeechBankMap;                        // 0x013B8394
extern long g_framesPerSecond;                              // 0x01375550
extern long g_DefaultSpeechDurationFrames;                  // 0x013BAB90

class CTCTalk {
public:
    void* m_pVtable;                                        // +0x00 CTCBase
    CThing* ParentThing;                                    // +0x04
    bool ToBeRemoved;                                       // +0x08
    bool IsInGlobalUpdateTCs;                               // +0x09
    long SpeechSample;                                      // +0x0C
    long SampleStartedFrame;                                // +0x10
    long SampleDurationFrames;                              // +0x14
    long Attitude;                                          // +0x18
    float MimeStartTime;                                    // +0x1C
    float SampleLoadedTime;                                 // +0x20
    CASoundBank* PSpeechBank;                               // +0x24

    CASoundBank* GetDefaultSpeechBank() const;              // 0x00686D70
private:
    const CLipSyncDataBank* GetPLipSyncBank() const         // out of line @ 0x006E2860
    {
        const CLipSyncDataBank* lipSync = 0;
        if (g_pSpeechBankMap) {
            CASoundBank* bank = PSpeechBank ? PSpeechBank : GetDefaultSpeechBank();
            lipSync = (const CLipSyncDataBank*)g_pSpeechBankMap->FindValueForSymbol((long)bank);
            if (!lipSync)
                lipSync = (const CLipSyncDataBank*)g_pSpeechBankMap->FindFirstValue();
        }
        return lipSync;
    }
    long GetSpeechDurationInFrames(long sample) const;
};

long CTCTalk::GetSpeechDurationInFrames(long sample) const
{
    const CLipSyncDataBank* lipSync = GetPLipSyncBank();
    if (lipSync) {
        const CLipSyncEntry* entry = lipSync->GetPEntry(sample);
        if (entry)
            return (long)(entry->Duration * g_framesPerSecond);
    }
    return g_DefaultSpeechDurationFrames;
}