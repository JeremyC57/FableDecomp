// CAIStateGroup_SkillPractice::IsRunnable() -- once a minute, runnable when a practice dummy is available
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0092B360, length 53 bytes (+ int3 padding).
// Callees: GFGetFrameCount @ 0x0049D870 (__fastcall, no arguments; reads the frame counter),
// GetDummy @ 0x0092A0C0 (bool: the propagated "ApprenticeHero" label names the same helper).
// Globals: frames-per-second long @ 0x01375550.
// Shape: the next-check frame at +0x14 is compared signed against the frame counter (`jge` skips);
// when due, a dummy makes it runnable, otherwise the check is deferred by sixty seconds of frames.
// The fps global is read into a local BEFORE the second frame-count call (retail loads esi first),
// so the source names it. Bool result (`mov al,1` / shared `xor al,al`).
extern long g_framesPerSecond;                  // 0x01375550
long __fastcall GFGetFrameCount();              // 0x0049D870

struct CAIStateGroupBase {
    virtual ~CAIStateGroupBase();
    unsigned char _pad[0x10];                   // PBrain, ScriptAIPriority, Name, flags (+0x4..+0x13)
};

struct CAIStateGroup_SkillPractice : CAIStateGroupBase {
    long FrameToCheckForDummy;                  // +0x14
    virtual bool IsRunnable();
    bool GetDummy();                            // 0x0092A0C0
};

bool CAIStateGroup_SkillPractice::IsRunnable()
{
    if (FrameToCheckForDummy < GFGetFrameCount()) {
        if (GetDummy())
            return true;
        long fps = g_framesPerSecond;
        FrameToCheckForDummy = GFGetFrameCount() + fps * 60;
    }
    return false;
}
