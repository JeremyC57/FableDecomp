// CIME::FinaliseString(char param) -- tell the IME UI to commit or cancel the composition, honouring CancelOnOKClick
// Retail: Fable.exe (TLC, VC7.1) @ 0x00404B50, 37 bytes. Callees: IME UI helper (bool).
//
// CIME wraps the Windows Input Method Editor so players using Japanese/Chinese/Korean keyboards can
// compose text in our edit boxes (hero name, save names). Nothing happens unless the IME was set up at
// start-up. The argument asks for a "finalise" (true) or plain close (false); when the profile option
// CancelOnOKClick is set we turn a finalise into a cancel, because on those systems pressing OK should
// discard the half-composed string rather than commit it. The resulting flag is passed to the UI
// helper that actually ends the composition.
//
// Matching: the argument is copied into a local that the inner branch overwrites with 0, which is what
// gives the `mov al,param; test; jz; cmp flag50; jz; xor al,al` shape; the helper takes the bool in cl.
struct CIME {
    char pad0[0x8];
    char enabled;     /* IsInitialised: the IME was set up at start-up */
    char pad9[0x47];  /* window handle, appearance, edit box */
    char flag50;      /* CancelOnOKClick profile option */
};

// IME UI: end the composition (true = finalise, false = cancel).
extern void __fastcall CIME_sink(bool b);

void __fastcall CIME_FinaliseString(CIME* self, void* /*edx*/, char param)
{
    if (self->enabled) {
        char v = param;
        // Finalise becomes cancel when the player asked for OK to cancel.
        if (param) {
            if (self->flag50)
                v = 0;
        }
        CIME_sink(v != 0);
    }
}
