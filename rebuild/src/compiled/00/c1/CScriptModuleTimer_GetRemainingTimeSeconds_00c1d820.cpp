// CScriptModuleTimer::GetRemainingTimeSeconds() -- the frames still to run divided by the owner's frames-per-second
// Leaf: GetRemainingTimeSeconds
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C1D820, 33 bytes (`ret`). Descriptive name (no PDB covers this library): the
//   manifest's "CScriptModuleSelfTerminate_GetDurationMsec" is propagated. Twin of the landed GetTotalTimeSeconds
//   0x00C1D850 (same divide over the frame sum 0x00C1D7F0); this one divides FramesRemaining 0x00C1D7D0. Both feed
//   the slot-1 dispatch of UpdateState 0x00C1D8C0 (vtable 0x012B32A0).
// Callees: FramesRemaining @ 0x00C1D7D0 (thiscall, no arguments), the frame-rate source's virtual slot 2
//   (`call [edx+8]`) reached through the owner (+0x04) -> +0x04.
// Shape: the frame count is a named local so it is computed first and parked in edi; the unsigned divide
//   (`xor edx,edx ; div ecx`).
struct CFrameRateSource {
    virtual void v00();
    virtual void v01();
    virtual unsigned long GetFramesPerSecond();     // slot 2
};

struct CScriptModuleOwner {
    void* _pad00;
    CFrameRateSource* PFrameRate;           // +0x04
};

struct CScriptModuleTimer {                 // vtable 0x012B32A0
    virtual void v00();
    CScriptModuleOwner* POwner;             // +0x04
    unsigned long FramesRemaining();        // 0x00C1D7D0
    unsigned long GetRemainingTimeSeconds();
};

unsigned long CScriptModuleTimer::GetRemainingTimeSeconds()
{
    unsigned long frames = FramesRemaining();
    return frames / POwner->PFrameRate->GetFramesPerSecond();
}