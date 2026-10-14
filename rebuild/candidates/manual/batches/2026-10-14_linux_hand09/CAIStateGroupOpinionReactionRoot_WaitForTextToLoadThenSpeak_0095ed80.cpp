// CAIStateGroup_OpinionReactionRoot::WaitForTextToLoadThenSpeak(ulong, EDialogueLayer) -- retry the speech while its
// text is still loading, waiting a frame between tries.
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0095ED80, length 93 bytes (+ int3 padding), `ret 8`.
// Callees: CTCTalk::PlayAISpeech(ulong, bool, EDialogueLayer, bool, bool) @ 0x006E3B30 on PTCTalk (+0x14) with
// (text, AllowSpeechOnNonPureAISpeaker (+0x8a, zero-extended), layer, false, true), CAIStateGroupBase::
// AIAction_DoNothing @ 0x0097E200, CAIStateGroupBase::Terminate @ 0x008FCF40.
// Shape: a `do { } while (wait == 4)` loop (a `for (;;)` with the exit inside gets rotated and the speech call
// duplicated) whose head is 16-byte aligned (`mov edi,edi` filler): playback result 2 (text loading) waits one
// frame and loops while the wait reports 4, otherwise terminates and returns the wait's result; any other playback
// result maps to 2 (played, result 0) or 3 (failed): `return r == 0 ? 2 : 3` is `xor ecx,ecx ; test ; setne cl ;
// add ecx,2`. The two arguments ride in ebp/ebx (callee-saved) across the loop. Members from the engine header.
struct CTCTalk {
    long PlayAISpeech(unsigned long text, bool allowNonPure, long layer, bool a, bool b);   // 0x006E3B30
};

struct CAIStateGroupBase {
    virtual ~CAIStateGroupBase();
    unsigned char _pad[0x10];                   // +0x4..+0x13
    long AIAction_DoNothing();                  // 0x0097E200 (EAIActionReturn)
    void Terminate();                           // 0x008FCF40
};

#pragma pack(push, 1)
struct CAIStateGroup_OpinionReactionRoot : CAIStateGroupBase {
    CTCTalk* PTCTalk;                           // +0x14
    long CurrentReactionType;                   // +0x18
    bool ReactionFlags[79];                     // +0x1c
    unsigned char ThingReactingTo[0x9];         // +0x6b CIntelligentPointer<CThing>
    unsigned char _pad_0x74[0x4];               // +0x74
    unsigned char CurrentUpdateFunc[0x10];      // +0x78
    bool LastRunWasTryAndLook;                  // +0x88
    bool ReactingToPlayer;                      // +0x89
    bool AllowSpeechOnNonPureAISpeaker;         // +0x8a
    long WaitForTextToLoadThenSpeak(unsigned long text, long layer);
};
#pragma pack(pop)

long CAIStateGroup_OpinionReactionRoot::WaitForTextToLoadThenSpeak(unsigned long text, long layer)
{
    long wait;
    do {
        long result = PTCTalk->PlayAISpeech(text, AllowSpeechOnNonPureAISpeaker, layer, false, true);
        if (result != 2)
            return result == 0 ? 2 : 3;
        wait = AIAction_DoNothing();
    } while (wait == 4);
    Terminate();
    return wait;
}
