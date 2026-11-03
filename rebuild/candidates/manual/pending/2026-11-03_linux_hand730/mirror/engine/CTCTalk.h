#ifndef FABLE_ENGINE_CTCTALK_H
#define FABLE_ENGINE_CTCTALK_H
#include "engine/CTCBase.h"
class CASoundBank;
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
    void StopCurrentSpeech();
};
FABLE_STATIC_ASSERT(sizeof(CTCTalk)==0x3c);

#endif
