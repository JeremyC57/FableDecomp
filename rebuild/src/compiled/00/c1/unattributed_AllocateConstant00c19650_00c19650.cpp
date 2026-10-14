// CClass_00c19650::AllocateConstant_00c19650() -- returns the `__cdecl` function @ 0x00BFEA1A applied to 0x14 (an allocation of 0x14 bytes)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C19650, oracle length 11 bytes.
// Grade: candidate -- generated for a repeated body shape and graded in the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); the batch README records the lane verdict.
//
// Retail bytes:  push 0x14 ; call 0xbfea1a ; add esp, 4 ; ret 
// Tiny-body shape `AllocateConstant` synthesised from the instruction pattern (the class comes from the
// manifest label when it is a plain class name, else the method is landed under a descriptive
// placeholder); member names are descriptive. 
void* __cdecl Call_00bfea1a(unsigned int size);   // 0x00BFEA1A

struct CClass_00c19650 {
    void* CallWithConstant_00c19650();                         // the result is returned: VC then cleans the slot with `add esp, 4` rather than `pop ecx`
};

void* CClass_00c19650::CallWithConstant_00c19650()
{
    return Call_00bfea1a(0x14);
}