// CTimedState_00878b00::ResetDuration() -- m_frames = (long)(g_framesPerSecond * 3.0f)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00878B00, length 28 bytes.
// Callees: __ftol2 @ 0x00BFEA70. Globals: the frame-rate long @ 0x01375550 (`fild`, the same
// divisor CAIStateGroup_BuyFromSeller::IsRunnable uses), the literal 3.0f in the float pool
// @ 0x012777F4. `fild [g] ; fmul [pool]` is `g * 3.0f` (the literal is the right operand);
// the truncated result lands at +0xa4. The manifest label (BoughtSomething) is propagated
// from an unrelated body; the class is named by what it does.
extern long g_framesPerSecond;              // 0x01375550

struct CTimedState_00878b00 {
    unsigned char _pad[0xa4];
    long m_frames;                          // +0xa4
    void ResetDuration();
};

void CTimedState_00878b00::ResetDuration()
{
    m_frames = (long)(g_framesPerSecond * 3.0f);
}
