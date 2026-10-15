// CIME::FinaliseString(void* a, int b) -- forward a two-argument request to the IME UI when the IME is initialised
// Retail: Fable.exe (TLC, VC7.1) @ 0x00404B80, 23 bytes. Callees: IME UI helper (a, b).
//
// Guarded pass-through: every CIME entry point checks IsInitialised first so the whole IME layer is a
// no-op on machines where it was never set up (no IME installed, or the option off). When it is, the two
// arguments go straight to the UI helper. The FinaliseString name on this address is shared with
// 0x00404B50; the two bodies differ in signature, so this appears to be a sibling forwarding member.
//
// Matching: `this` in ecx, the two arguments on the stack are reloaded into ecx/edx for the __fastcall
// callee; `ret 8`.
struct CIME { char pad[8]; char enabled; };   // +8: IsInitialised

// The IME UI routine being forwarded to.
extern void __fastcall CIME_helper(void* p, int q);

void __fastcall CIME_FinaliseString(CIME* self, int edx_ignore, void* a, int b)
{
    if (self->enabled)
        CIME_helper(a, b);
}
