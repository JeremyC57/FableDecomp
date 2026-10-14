// SetLeftAlignText(bool) -- stores the argument into the global byte @ 0x013CA7EB (fastcall)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009BC8A0, oracle length 7 bytes.
// Grade: candidate -- generated for a repeated body shape and graded in the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); the batch README records the lane verdict.
//
// Retail bytes:  mov byte ptr [0x13ca7eb], cl ; ret 
// Tiny-body shape `SetGlobalByte`: a `__fastcall` setter of one global byte flag (the global's
// address is a relocation). The manifest label is propagated; the name is descriptive.
extern bool g_flag;                        // 0x013CA7EB

void __fastcall SetLeftAlignText(bool value)
{
    g_flag = value;
}