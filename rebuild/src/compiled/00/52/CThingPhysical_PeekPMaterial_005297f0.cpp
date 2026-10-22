// CThingPhysical::vfunc_58(long*) -- stores 0x0 through the out-pointer
// Retail: Fable.exe (TLC, VC7.1) function @ 0x005297F0, oracle length 13 bytes.
// Grade: candidate -- generated for a repeated body shape and graded in the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); the batch README records the lane verdict.
//
// Retail bytes:  mov eax, dword ptr [esp + 4] ; mov dword ptr [eax], 0 ; ret 4
// Tiny-body shape `SetOut` synthesised from the instruction pattern (the class comes from the
// manifest label when it is a plain class name, else the method is landed under a descriptive
// placeholder); member names are descriptive. 
struct CThingPhysical {
    void PeekPMaterial(long* out);
};

void CThingPhysical::PeekPMaterial(long* out)
{
    *out = 0x0;
}