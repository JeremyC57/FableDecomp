// CIME::IsIMEActive() -- is the IME initialised and currently switched on by the player?
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00404C50, length 22 bytes.
//
// The edit boxes ask this to decide whether keystrokes go through composition or straight into the
// text. Two conditions: we set the IME up at start-up (IsInitialised), and the IME UI library reports
// that the player has it enabled right now (they can toggle it with the usual IME hot keys).
//
// Matching: the callee at 0x00CB28E0 carries the propagated label "CAIStateGroup_Freeze::IsRunnable";
// `this` in ecx; the result is `mov al,1` / `xor al,al` arms.
struct CIME {
    unsigned char _pad[8];
    bool          m_active;                        // IsInitialised
    bool IsImeEnabled();                           // IME UI: is the IME switched on right now (0x00CB28E0)
    bool IsIMEActive();
};

bool CIME::IsIMEActive()
{
    if (m_active && IsImeEnabled())
        return true;
    return false;
}
