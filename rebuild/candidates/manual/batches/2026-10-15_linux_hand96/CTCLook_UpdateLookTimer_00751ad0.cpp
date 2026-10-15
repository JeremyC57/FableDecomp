// CTCLook::UpdateLookTimer() -- re-arm the look timer: LookTimer = current frame number + 90
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00751AD0, length 23 bytes.
// Callees: the CTCBase accessor @ 0x00686D60 (out of line, `this` unused: returns [g_pMainGameComponent 0x013B8A1C]+0x1c;
// the manifest's CViewBase::GetDisplayEngine is propagated), the frame-number getter @ 0x004167C8 on that object
// (an incremental-link thunk to `return g_frame`, as the landed callers of 0x004167C8 record).
// Layout (local model, no engine header): LookTimer at +0x3c (PDB offset, before the 12-byte vector shift).
// Shape: `this` parked in esi across both calls, the frame source used directly as `this` of the getter, `add eax,5Ah`.
struct CFrameSource {
    long GetFrameNumber();                         // 0x004167C8
};

struct CTCLook {
    unsigned char _pad_0x00[0x3c];
    long          LookTimer;                       // +0x3c
    CFrameSource* GetFrameSource();                // 0x00686D60 (CTCBase accessor)
    void UpdateLookTimer();
};

void CTCLook::UpdateLookTimer()
{
    LookTimer = GetFrameSource()->GetFrameNumber() + 90;
}
