// CTCTalk::PlayRandomLipMovement() -- reset the talker's speech bank and sample, resolve the lip-sync bank for the default speech bank, look the speaker's sound symbol up in the matching sound bank, start loading that lip-sync entry, then stamp the mime start time and the sample duration
// Leaf: ?PlayRandomLipMovement@CTCTalk@@QAEXXZ
// Retry 10b (getsw sweep, FIXES B-15): the speaker info is read through an inline getter `GetSpeakerInfo()`; the plain member read gives the parked register/scheduling difference.
// Identity (agentNF13, 2026-11-11; was CTCTalk::SetUpSpeechSample, in no name set): FableWin.pdb / Ego_r.pdb
//   ?PlayRandomLipMovement@CTCTalk@@QAEXXZ; the Ego_r donor order DoesSpeechExist / GetSpeechDurationInFrames / PlayLipMovement /
//   PlayRandomLipMovement = retail 0x006E2910 / 0x006E2940 / 0x006E29A0 / 0x006E2A50, and CActionTalkBase::SetUpTextAndSpeech
//   0x00716B30 calls it (landed binding) for a line without speech. GetSpeechDurationInFrames 0x006E2940 is the private const
//   ?GetSpeechDurationInFrames@CTCTalk@@ABEJJ@Z (was spelt GetSampleDurationFrames).
// Retail: Fable.exe (TLC, VC7.1) @ 0x006E2A50, 152 bytes (+8 int3), __fastcall, no arguments.
// Sound manager (agentSND2, 2026-11-10): the "symbol map" at 0x013B8394 is PSoundManager (engine/CASoundManager.h); its bank lookups carry the Fable Anniversary 360 PDB names under FABLE_RETAIL_CASOUNDMANAGER_BANKLOOKUP (GetPLipSyncBank 0x009916F0, GetAnyPLipSyncBank 0x00991720, GetPSoundBank(const CLipSyncDataBank&) 0x00991690 descriptive); was a local CSymbolMap.
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
// Name fix (agentNF6 2026-11-10): GFGetTime 0x009E1BC0 is ?GFGetTime@@YINXZ (Ego_r.pdb / FableWin.pdb: global __fastcall, double, no arguments).
class CCharStringData;
class CCharString {
public:
    CCharStringData* m_pData;
};

class CASoundBank {
public:
    long GetSoundIndexFromSymbol(const CCharString& symbol) const;    // 0x00A01920
};

class CLipSyncDataBank {
public:
    void BeginLoadingEntry(unsigned long index);                 // 0x00A42AC0
};

#define FABLE_RETAIL_CASOUNDMANAGER_BANKLOOKUP
#include "engine/CASoundManager.h"

struct CSpeakerInfo {
    unsigned char _pad_0x00[0xAC];
    CCharString SpeechSymbol;                                    // +0xAC
};
class CGameDefinitionManager { public:
    unsigned char _pad_0x00[0xD0];
    CSpeakerInfo* PSpeakerInfo;                                  // +0xD0
    CSpeakerInfo* GetSpeakerInfo() const { return PSpeakerInfo; }
};

double __fastcall GFGetTime();                                   // 0x009E1BC0

class CThing;

class CTCBase
{
protected:
    const CASoundBank& PeekSampleBank() const;   // 0x00686D70
    const CGameDefinitionManager& GetDefinitionManager() const;   // 0x006869D0
};

class CTCTalk : public CTCBase {
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

private:
    long GetSpeechDurationInFrames(long sample) const;           // 0x006E2940 (Ego_r.pdb, private)
public:

    CLipSyncDataBank* GetLipSyncBank()
    {
        if (!PSoundManager)
            return 0;
        const CASoundBank* bank = PSpeechBank ? PSpeechBank : (&PeekSampleBank());
        CLipSyncDataBank* lipSync = const_cast<CLipSyncDataBank*>(PSoundManager->GetPLipSyncBank(*bank));
        if (!lipSync)
            lipSync = const_cast<CLipSyncDataBank*>(PSoundManager->GetAnyPLipSyncBank());
        return lipSync;
    }

    void PlayRandomLipMovement();
};

void CTCTalk::PlayRandomLipMovement()
{
    PSpeechBank = 0;
    SpeechSample = 0;
    CLipSyncDataBank* lipSync = GetLipSyncBank();
    if (lipSync) {
        CASoundBank* soundBank = PSoundManager->GetPSoundBank(*lipSync);
        if (soundBank)
            SpeechSample = soundBank->GetSoundIndexFromSymbol(GetDefinitionManager().GetSpeakerInfo()->SpeechSymbol);
        lipSync->BeginLoadingEntry(SpeechSample);
    }
    MimeStartTime = (float)GFGetTime();
    SampleDurationFrames = GetSpeechDurationInFrames(SpeechSample);
}