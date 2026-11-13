#ifndef FABLE_ENGINE_CTCTALK_H
#define FABLE_ENGINE_CTCTALK_H
#include "engine/CTCBase.h"
class CASoundBank;
#ifdef FABLE_RETAIL_CTCTALK
namespace NSpeechGainManager { enum EDialogueLayer; }                    // MS forward enum (the row defines the enumerators it uses)
#endif
#ifdef FABLE_RETAIL_CTCTALK_CONVERSATION
enum EConversationAttitude;                                              // MS forward enum (Ego_r.pdb; enumerators in no name set on disk)
#endif
// TPI0x23414 full original fields and base; no container-width changes here.
class CTCTalk : public CTCBase {
private:
    long SpeechSample,SampleStartedFrame,SampleDurationFrames;
#ifdef FABLE_RETAIL_CTCTALK_CONVERSATION
    EConversationAttitude Attitude;                                       // +0x18 (Ego_r.pdb type)
#else
    unsigned char Attitude[4]; // EConversationAttitude.
#endif
    float MimeStartTime,SampleLoadedTime;CASoundBank* PSpeechBank;
    unsigned long SampleID;long LastTextIndex;CCharString LastNarrator;
    bool IsPureAISpeaker,LipSynching,UseLipSynching;CCharString DialogueVoice;
public:
    static ETCInterfaceType __fastcall StatGetInterfaceType() { return TCI_TALK; }
    bool IsTalking() const;
#ifdef FABLE_RETAIL_CTCTALK
    // Retail opt-in (chunk 755, row 0x008AF500 CombatDialogueManager::AttemptToSpeak): Ego_r.pdb 0x0070E0C0
    // ?PlayAISpeech@CTCTalk@@QAE?AW4ESpeechPlaybackResult@1@K_NW4EDialogueLayer@NSpeechGainManager@@00@Z (retail 0x006E3B60). The
    // nested result enum's enumerator names are descriptive (the landed callers' spellings): 0 started, 1 failed for good.
    enum ESpeechPlaybackResult { SPEECH_PLAYBACK_STARTED, SPEECH_PLAYBACK_FAILED_FINAL, SPEECH_PLAYBACK_FAILED_RETRY };
    ESpeechPlaybackResult PlayAISpeech(unsigned long textGroup, bool, NSpeechGainManager::EDialogueLayer layer, bool, bool);   // 0x006E3B60
#ifdef FABLE_RETAIL_CTCTALK_AISPEECHTEXT
    // Nested opt-in (chunk 802, row 0x00974AA0 CAIStateGroup_JackOfBladesStage2::Update): Ego_r.pdb
    // ?PlayAISpeech@CTCTalk@@QAE?AW4ESpeechPlaybackResult@1@ABVCCharString@@_NW4EDialogueLayer@NSpeechGainManager@@11@Z, the text-symbol
    // overload, out of line in retail at 0x006E3B30 (the caller pushes 1, 0, 0, 1 and the CCharString address). Declaration only.
    ESpeechPlaybackResult PlayAISpeech(const CCharString& text, bool, NSpeechGainManager::EDialogueLayer layer, bool, bool);   // 0x006E3B30
#endif
#endif
    void StopCurrentSpeech();
#ifdef FABLE_RETAIL_CTCTALK_SPEECHSAMPLEPLAYING
    // Opt-in (chunk 2010, row 0x00716DE0 CActionTalkBase::Update): Ego_r.pdb / FableWin.pdb ?IsSpeechSamplePlaying@CTCTalk@@QBE_NXZ (retail
    // 0x006E2D30, landed) and ?IsSpeechSampleLoadedAndPlaying@CTCTalk@@QBE_NXZ (retail 0x006E2DC0, landed), out of line. Declarations only.
    bool IsSpeechSamplePlaying() const;                                  // 0x006E2D30
    bool IsSpeechSampleLoadedAndPlaying() const;                         // 0x006E2DC0
#endif
#ifdef FABLE_RETAIL_CTCTALK_CONVERSATION
    // Retail opt-in (chunk 1026, row 0x00716B30 CActionTalkBase::SetUpTextAndSpeech): FableWin.pdb
    // ?PlaySpeech@CTCTalk@@QAE?AW4ESpeechPlaybackResult@1@AAVCASoundBank@@J_NABVCCharString@@W4EDialogueLayer@NSpeechGainManager@@11@Z
    // (0x006E3580: the caller pushes seven words), ?PlayRandomLipMovement@CTCTalk@@QAEXXZ (0x006E2A50, the silent-line call),
    // ?GetCurrentSpeechDuration@CTCTalk@@QBEMXZ (0x006E2840) and the inline ?SetAttitude@CTCTalk@@QAEXW4EConversationAttitude@@@Z
    // (expanded: `mov [edi+18h],ecx` at 0x00716D38). Needs FABLE_RETAIL_CTCTALK.
    ESpeechPlaybackResult PlaySpeech(CASoundBank& bank, long sample, bool b, const CCharString& narrator,
                                     NSpeechGainManager::EDialogueLayer layer, bool in2D, bool c);   // 0x006E3580
    void PlayRandomLipMovement();                                        // 0x006E2A50
    float GetCurrentSpeechDuration() const;                              // 0x006E2840
    void SetAttitude(EConversationAttitude attitude) { Attitude = attitude; }
#endif
#ifdef FABLE_RETAIL_CTCTALK_LIPSYNC
    // Opt-in (chunk 1024, row 0x006E2ED0): Ego_r.pdb / FableWin.pdb private members ?GetSpeechBank@CTCTalk@@ABEABVCASoundBank@@XZ
    // (inline in retail: `mov eax,[esi+24h] ; test eax,eax ; jne` else CTCBase::PeekSampleBank 0x00686D70), ?GetPLipSyncBank@CTCTalk@@ABEPBVCLipSyncDataBank@@XZ
    // (out of line in retail at 0x006E2860, after GetCurrentSpeechDuration 0x006E2840 as in the Ego_r donor order; the row defines
    // its body, the CTCTalk TU's) and ?GetCurrentLipSyncFrameData@CTCTalk@@AAE_NAAVCLipSyncFrameData@@@Z (retail 0x006E2ED0, called by
    // the talk update at 0x006E3C65 with a CLipSyncFrameData local). Needs FABLE_RETAIL_CTCBASE_PEEKSAMPLEBANK.
private:
    const CASoundBank& GetSpeechBank() const { return PSpeechBank ? *PSpeechBank : PeekSampleBank(); }
    const class CLipSyncDataBank* GetPLipSyncBank() const;               // 0x006E2860
    bool GetCurrentLipSyncFrameData(class CLipSyncFrameData& frame_data);   // 0x006E2ED0
public:
#endif
#ifdef FABLE_RETAIL_CTCTALK
    // Chunk 756 (row 0x008AF6E0): Ego_r.pdb ?DoesSpeechExist@CTCTalk@@QAE_NK@Z (0x006E2910).
    bool DoesSpeechExist(unsigned long textGroup);
#endif
#ifdef FABLE_RETAIL_CTCTALK_ONCREATE
    // Nested opt-in (chunk 760, row 0x006E2C00): the CTCBase slot-4 override ?OnCreate@CTCTalk@@UAEXXZ (CTCTalk vtable slot 4,
    // retail 0x006E2C00). Declaration only (a vtable-emitting includer then names it).
    virtual void OnCreate();                                             // 0x006E2C00
#endif
};
FABLE_STATIC_ASSERT(sizeof(CTCTalk)==0x3c);

#endif
