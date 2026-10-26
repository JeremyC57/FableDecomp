// CTCTalk::PlayLipMovement(CASoundBank& bank, long sample) -- start a lip movement from a given speech bank and sample: record them, reset the mime start time, start loading the sample's lip-sync entry in the paired lip-sync bank, and set the sample duration in frames
// Leaf: ?PlayLipMovement@CTCTalk@@
// Retail: Fable.exe (TLC, VC7.1) @ 0x006E29A0, 105 bytes, thiscall `ret 8`.
// Identity: Ego_r.pdb name set `?PlayLipMovement@CTCTalk@@QAEXAAVCASoundBank@@J@Z` (public, void(CASoundBank&, long)); the
//   body is the bank/sample twin of the landed CTCTalk::SetUpSpeechSample 0x006E2A50 (same lip-sync lookup, same
//   BeginLoadingEntry, same duration call). Only caller 0x006E372A. The catalog's "CAnimateThing_SetMeshBankAndSymbol"
//   is descriptive.
// Callees: the default speech bank getter @ 0x00686D70, CSymbolMap::FindValueForSymbol @ 0x009916F0 / FindFirstValue
//   @ 0x00991720 (the inlined GetPLipSyncBank 0x006E2860), CLipSyncDataBank::BeginLoadingEntry(unsigned long) @ 0x00A42AC0,
//   CTCTalk::GetSpeechDurationInFrames(long) const @ 0x006E2940.
// Globals: the speech-bank map @ 0x013B8394.
// Layout: CTCTalk (Ego_r.pdb) SpeechSample +0x0C, SampleDurationFrames +0x14, MimeStartTime +0x1C, PSpeechBank +0x24.
// Shape: the three member stores in source order (the float zero as an immediate); GetPLipSyncBank inlined, its bank
//   test reusing the just-stored argument register; the sample re-read from the member for both calls.
class CASoundBank;
class CThing;

class CLipSyncDataBank {
public:
    void BeginLoadingEntry(unsigned long index);            // 0x00A42AC0
};

class CSymbolMap {
public:
    void* FindValueForSymbol(long symbol);                  // 0x009916F0
    void* FindFirstValue();                                 // 0x00991720
};
extern CSymbolMap* g_pSpeechBankMap;                        // 0x013B8394

class CTCBase
{
protected:
    const CASoundBank& PeekSampleBank() const;   // 0x00686D70
};

class CTCTalk : public CTCBase {
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

private:
    const CLipSyncDataBank* GetPLipSyncBank() const         // out of line @ 0x006E2860
    {
        const CLipSyncDataBank* lipSync = 0;
        if (g_pSpeechBankMap) {
            const CASoundBank* bank = PSpeechBank ? PSpeechBank : (&PeekSampleBank());
            lipSync = (const CLipSyncDataBank*)g_pSpeechBankMap->FindValueForSymbol((long)bank);
            if (!lipSync)
                lipSync = (const CLipSyncDataBank*)g_pSpeechBankMap->FindFirstValue();
        }
        return lipSync;
    }
    long GetSpeechDurationInFrames(long sample) const;      // 0x006E2940
public:
    void PlayLipMovement(CASoundBank& bank, long sample);
};

void CTCTalk::PlayLipMovement(CASoundBank& bank, long sample)
{
    PSpeechBank = &bank;
    SpeechSample = sample;
    MimeStartTime = 0.0f;
    const CLipSyncDataBank* lipSync = GetPLipSyncBank();
    if (lipSync)
        const_cast<CLipSyncDataBank*>(lipSync)->BeginLoadingEntry(SpeechSample);
    SampleDurationFrames = GetSpeechDurationInFrames(SpeechSample);
}