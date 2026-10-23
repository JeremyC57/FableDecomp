// SetCurrentThingManager(void*) -- stores the argument into the global @ 0x0143BB38 (fastcall)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C1BEB0, oracle length 7 bytes.
// Grade: candidate -- generated for a repeated body shape and graded in the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); the batch README records the lane verdict.
//
// Retail bytes:  mov dword ptr [0x143bb38], ecx ; ret 
// Tiny-body shape `SetGlobal`: a `__fastcall` setter of one global pointer (the global's address
// is a relocation). The manifest label is propagated; the name is descriptive.
extern void* g_value_0143bb38;                      // 0x0143BB38

void __fastcall SetCurrentThingManager(void* value)
{
    g_value_0143bb38 = value;
}