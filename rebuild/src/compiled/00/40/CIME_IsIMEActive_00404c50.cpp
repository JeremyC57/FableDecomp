// CIME::IsIMEActive() -- the IME is active (+8) and the UI library reports it enabled (0x00CB28E0)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00404C50, length 22 bytes.
// The callee label "CAIStateGroup_Freeze::IsRunnable" is propagated. Shape: `mov al,1` / `xor al,al`.
struct CIME {
    unsigned char _pad[8];
    bool          m_active;                        // +0x08
    bool IsImeEnabled();                           // 0x00CB28E0 (labelled "IsRunnable", propagated; `this` in ecx)
    bool IsIMEActive();
};

bool CIME::IsIMEActive()
{
    if (m_active && IsImeEnabled())
        return true;
    return false;
}