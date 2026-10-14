// SetCurrentThingManager(void*) -- stores the argument into the global @ 0x0143E8F8 (fastcall)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00CBE0C0, oracle length 7 bytes.
// Grade: candidate -- generated for a repeated body shape and graded in the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); the batch README records the lane verdict.
//
// Retail bytes:  mov dword ptr [0x143e8f8], ecx ; ret 
// Tiny-body shape `SetGlobal`: a `__fastcall` setter of one global pointer (the global's address
// is a relocation). The manifest label is propagated; the name is descriptive.
extern void* g_value;                      // 0x0143E8F8

void __fastcall SetCurrentThingManager(void* value)
{
    g_value = value;
}