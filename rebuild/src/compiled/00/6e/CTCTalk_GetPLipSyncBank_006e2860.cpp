// CTCTalk::GetPLipSyncBank() const -- the lip-sync data bank paired with the talker's speech bank (its own, else the default one) in the global speech-bank map, falling back to the map's first lip-sync bank; null when there is no map
// Leaf: ?GetPLipSyncBank@CTCTalk@@
// Retail: Fable.exe (TLC, VC7.1) @ 0x006E2860, 52 bytes (+ int3 padding), thiscall, plain `ret`.
// Identity: Ego_r.pdb name set `?GetPLipSyncBank@CTCTalk@@ABEPBVCLipSyncDataBank@@XZ` (private const); the body is the
//   lip-sync lookup the landed CTCTalk::SetUpSpeechSample 0x006E2A50 inlines (same map, same calls, same fallback).
//   Callers 0x006E3032 / 0x006E30AE / 0x006E313E (CTCTalk update code). The catalog's "CAnimateThing_FindMeshSymbol"
//   is descriptive.
// Callees: the default speech bank getter @ 0x00686D70 (on `this`; manifest "CAnimateThing::GetMeshBank" is propagated),
//   CSymbolMap::FindValueForSymbol(long) @ 0x009916F0, CSymbolMap::FindFirstValue() @ 0x00991720 (tail-jumped).
// Globals: the speech-bank -> lip-sync-bank symbol map @ 0x013B8394 (null while there is no sound).
// Layout: CTCTalk (Ego_r.pdb) PSpeechBank +0x24, as the landed SetUpSpeechSample model.
// Shape: a zero-initialised result returned once at the end (`xor eax,eax` preloaded beside the map test; an early
//   `return 0` returns the tested global instead); `PSpeechBank ? PSpeechBank : PeekSampleBank()` calls the default
//   getter with `this` still in ecx; the map is re-read from the global for each call; the fallback is the tail call.
class CASoundBank;
class CLipSyncDataBank;
class CThing;

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
    const CLipSyncDataBank* GetPLipSyncBank() const;
};

const CLipSyncDataBank* CTCTalk::GetPLipSyncBank() const
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