// CBBBSoundAllocator::vfunc_1(void*) -- returns the result of the `__cdecl` function @ 0x00BFEA0E applied to the argument
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A3F960, oracle length 16 bytes.
// Grade: candidate -- generated for a repeated body shape and graded in the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); the batch README records the lane verdict.
//
// Retail bytes:  mov eax, dword ptr [esp + 4] ; push eax ; call 0xbfea0e ; add esp, 4 ; ret 4
// Tiny-body shape `ForwardArgResult` synthesised from the instruction pattern (the class comes from the
// manifest label when it is a plain class name, else the method is landed under a descriptive
// placeholder); member names are descriptive. 
void* __cdecl Call_00bfea0e(void* argument);   // 0x00BFEA0E

struct CBBBSoundAllocator {
    void* vfunc_1(void* argument);            // the result is returned: that is what makes VC clean the slot with `add esp, 4` rather than `pop ecx`
};

void* CBBBSoundAllocator::vfunc_1(void* argument)
{
    return Call_00bfea0e(argument);
}