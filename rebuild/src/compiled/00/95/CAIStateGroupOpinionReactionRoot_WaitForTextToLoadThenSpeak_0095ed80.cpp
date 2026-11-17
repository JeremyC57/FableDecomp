// CAIStateGroup_OpinionReactionRoot::WaitForTextToLoadThenSpeak(const CCharString&, EDialogueLayer) -- retry the speech while its
// text is still loading, waiting a frame between tries.
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0095ED80, length 93 bytes (+ int3 padding), `ret 8`.
// Leaf: ?WaitForTextToLoadThenSpeak@CAIStateGroup_OpinionReactionRoot@@AAE?AW4EAIActionReturn@@ABVCCharString@@W4EDialogueLayer@NSpeechGainManager@@@Z
//   (private, as Ego_r.pdb 0x00983300 and FableWin.pdb 0x02C28520 spell it; coordinator, landing nf56: its caller 0x00982F70
//   now binds this spelling through engine/CAIStateGroup_OpinionReactionRoot.h WIFEUPDATE).
// Callees: CTCTalk::PlayAISpeech(const CCharString&, bool, EDialogueLayer, bool, bool) @ 0x006E3B30 on PTCTalk (+0x14) with
// (text, AllowSpeechOnNonPureAISpeaker (+0x8a, zero-extended), layer, false, true), CAIStateGroupBase::
// AIAction_DoNothing @ 0x0097E200, CAIStateGroupBase::Terminate @ 0x008FCF40.
// Shape: a `do { } while (wait == 4)` loop (a `for (;;)` with the exit inside gets rotated and the speech call
// duplicated) whose head is 16-byte aligned (`mov edi,edi` filler): playback result 2 (text loading) waits one
// frame and loops while the wait reports 4, otherwise terminates and returns the wait's result; any other playback
// result maps to 2 (played, result 0) or 3 (failed): `return r == 0 ? 2 : 3` is `xor ecx,ecx ; test ; setne cl ;
// add ecx,2`. The two arguments ride in ebp/ebx (callee-saved) across the loop. Members from the engine header.
// NF55 (2026-11-17): CTCTalk is engine/CTCTalk.h (AISPEECHTEXT: Ego_r.pdb
//   ?PlayAISpeech@CTCTalk@@QAE?AW4ESpeechPlaybackResult@1@ABVCCharString@@_NW4EDialogueLayer@NSpeechGainManager@@11@Z, the text overload at
//   0x006E3B30; the sample-id overload is 0x006E3B60). It was a local CTCTalk with a made-up PlayAISpeech(unsigned long, ...).
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTCTALK
#define FABLE_RETAIL_CTCTALK_AISPEECHTEXT
#include "fable_string.h"
#include "engine/ETCInterfaceType.h"
#include "engine/CTCTalk.h"

enum EAIActionReturn { };
class CAIStateGroupBase { public:
    virtual ~CAIStateGroupBase();
    unsigned char _pad[0x10];                   protected: // +0x4..+0x13
    EAIActionReturn AIAction_DoNothing(); public:                  // 0x0097E200 (EAIActionReturn)
    void Terminate();                           // 0x008FCF40
};

#pragma pack(push, 1)
class CAIStateGroup_OpinionReactionRoot : public CAIStateGroupBase { public:
    CTCTalk* PTCTalk;                           // +0x14
    long CurrentReactionType;                   // +0x18
    bool ReactionFlags[79];                     // +0x1c
    unsigned char ThingReactingTo[0x9];         // +0x6b CIntelligentPointer<CThing>
    unsigned char _pad_0x74[0x4];               // +0x74
    unsigned char CurrentUpdateFunc[0x10];      // +0x78
    bool LastRunWasTryAndLook;                  // +0x88
    bool ReactingToPlayer;                      // +0x89
    bool AllowSpeechOnNonPureAISpeaker;         // +0x8a
private:
    EAIActionReturn WaitForTextToLoadThenSpeak(const CCharString& text, NSpeechGainManager::EDialogueLayer layer);
};
#pragma pack(pop)

EAIActionReturn CAIStateGroup_OpinionReactionRoot::WaitForTextToLoadThenSpeak(const CCharString& text, NSpeechGainManager::EDialogueLayer layer)
{
    EAIActionReturn wait;
    do {
        CTCTalk::ESpeechPlaybackResult result = PTCTalk->PlayAISpeech(text, AllowSpeechOnNonPureAISpeaker, layer, false, true);
        if (result != CTCTalk::SPEECH_PLAYBACK_FAILED_RETRY)
            return (EAIActionReturn)(result == 0 ? 2 : 3);
        wait = AIAction_DoNothing();
    } while (wait == 4);
    Terminate();
    return wait;
}