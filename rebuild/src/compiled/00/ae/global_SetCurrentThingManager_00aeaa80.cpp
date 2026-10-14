// SetCurrentThingManager(void*) -- stores the argument into the global @ 0x013D2E08 (fastcall)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00AEAA80, oracle length 7 bytes.
// Grade: candidate -- generated for a repeated body shape and graded in the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); the batch README records the lane verdict.
//
// Retail bytes:  mov dword ptr [0x13d2e08], ecx ; ret 
// Tiny-body shape `SetGlobal`: a `__fastcall` setter of one global pointer (the global's address
// is a relocation). The manifest label is propagated; the name is descriptive.
extern void* g_value;                      // 0x013D2E08

void __fastcall SetCurrentThingManager(void* value)
{
    g_value = value;
}