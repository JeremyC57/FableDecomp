// CIME::Enable(char enable) -- switch IME composition on or off for the current edit box
// Retail: Fable.exe (TLC, VC7.1) @ 0x00404F40, 31 bytes. Callees: two IME UI toggles.
//
// Edit boxes call this when they gain or lose focus. When the IME was initialised we first clear the
// first IME UI state (closing any composition in progress) and then set the enabled state to what the
// caller asked for, so toggling always starts from a clean composition. Without an initialised IME this
// does nothing, which is what keeps the IME layer invisible on systems without one.
//
// Matching: `enable` is the stack argument after the unused edx register; the two helpers take their
// bool in cl, the first as a literal 0, the second as `enable != 0` (`setne`).
struct CIME {
    unsigned char _pad_0x0[0x8];
    char enabled;               // IsInitialised
    unsigned char _pad_0x9[0x47];
    char flag50;                // CancelOnOKClick (not read here)
};

// IME UI: clear the composition state / set the enabled state.
extern void __fastcall CIME_cb_a(char b);
extern void __fastcall CIME_cb_b(char b);

void __fastcall CIME_Enable(CIME* self, int edx, char enable)
{
    if (self->enabled) {
        CIME_cb_a(0);
        CIME_cb_b(enable != 0);
    }
}
