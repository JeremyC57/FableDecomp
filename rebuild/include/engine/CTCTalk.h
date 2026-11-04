#ifndef FABLE_ENGINE_CTCTALK_H
#define FABLE_ENGINE_CTCTALK_H
#include "engine/CTCBase.h"
class CASoundBank;
#ifdef FABLE_RETAIL_CTCTALK
namespace NSpeechGainManager { enum EDialogueLayer; }                    // MS forward enum (the row defines the enumerators it uses)
#endif
// TPI0x23414 full original fields and base; no container-width changes here.
class CTCTalk : public CTCBase {
private:
    long SpeechSample,SampleStartedFrame,SampleDurationFrames;
    unsigned char Attitude[4]; // EConversationAttitude.
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
#endif
    void StopCurrentSpeech();
#ifdef FABLE_RETAIL_CTCTALK
    // Chunk 756 (row 0x008AF6E0): Ego_r.pdb ?DoesSpeechExist@CTCTalk@@QAE_NK@Z (0x006E2910).
    bool DoesSpeechExist(unsigned long textGroup);
#endif
};
FABLE_STATIC_ASSERT(sizeof(CTCTalk)==0x3c);

#endif
