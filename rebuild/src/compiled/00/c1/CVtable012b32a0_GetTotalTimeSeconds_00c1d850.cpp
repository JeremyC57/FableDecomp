// CVtable_012b32a0::GetTotalTimeSeconds() -- the module's total frame count divided by the owner's frames-per-second
// Leaf: GetTotalTimeSeconds
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C1D850, length 33 bytes (+15 int3). The manifest label
// "CTCCreatureNavigation::IsPosNavLineOfSightTo" is propagated (the real one is the landed 0x006CBA20). This body belongs to
// the no-RTTI script-module timer class whose vtable is 0x012B32A0 (slot 1 = 0x00C1D8C0 UpdateState, the only caller
// besides 0x00C1D979); its siblings are FramesRemaining 0x00C1D7D0, the frame sum 0x00C1D7F0 and the seconds-remaining
// twin 0x00C1D820 (same divide over FramesRemaining).
// Callees: the frame sum @ 0x00C1D7F0 (two frame queries + the count at +0x08), the frame-rate source's virtual slot 2
// (`call [edx+8]`) reached through the owner (+0x04) -> +0x04.
// Shape: the unsigned quotient (`xor edx,edx ; div ecx`); the frame count is a named local so it is computed first and
// parked in edi (the bare `a() / b()` expression calls the divisor first).
struct CFrameRateSource {
    virtual void v00();
    virtual void v01();
    virtual unsigned long GetFramesPerSecond();     // slot 2
};

struct CScriptModuleOwner {
    void* _pad00;
    CFrameRateSource* PFrameRate;           // +0x04
};

struct CVtable_012b32a0 {
    virtual void v00();
    CScriptModuleOwner* POwner;             // +0x04
    long BaseFrames;                        // +0x08
    unsigned long GetTotalFrames();         // 0x00C1D7F0
    unsigned long GetTotalTimeSeconds();
};

unsigned long CVtable_012b32a0::GetTotalTimeSeconds()
{
    unsigned long frames = GetTotalFrames();
    return frames / POwner->PFrameRate->GetFramesPerSecond();
}