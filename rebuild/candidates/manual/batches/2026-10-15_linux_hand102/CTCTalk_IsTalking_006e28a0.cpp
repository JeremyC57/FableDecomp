// CTCTalk::IsTalking() -- true while the current speech sample is still inside its frame window
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006E28A0, length 33 bytes.
// Callee: the world's frame-counter getter @ 0x00416225 (`mov eax,[ecx+48h] ; ret`, called out of line on the
// global world pointer @ 0x013B86A0; its manifest label "DrawGetEnvironment" is a propagated name).
// Ego_r.pdb CTCTalk: SampleStartedFrame +0x10, SampleDurationFrames +0x14 (both long). The test is the signed
// `frame - started < duration`; the `xor ecx,ecx ; setl cl ; mov al,cl` result is the plain `return cond;` bool
// form (GOTCHAS 2026-10-13: a bool written `return cond;` goes through a full register).
struct CWorld {
    long GetFrame();                         // 0x00416225
};
extern CWorld* g_pWorld;                     // 0x013B86A0

struct CTCTalk {
    unsigned char _base[0x0c];               // CTCBase: vptr, ParentThing, flags
    long          SpeechSample;              // +0x0c
    long          SampleStartedFrame;        // +0x10
    long          SampleDurationFrames;      // +0x14
    bool IsTalking();
};

bool CTCTalk::IsTalking()
{
    return g_pWorld->GetFrame() - SampleStartedFrame < SampleDurationFrames;
}
