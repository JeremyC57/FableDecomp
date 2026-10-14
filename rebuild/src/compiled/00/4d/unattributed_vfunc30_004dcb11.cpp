// CClass_004dcb11::vfunc_30(long*) -- stores 0x2 through the out-pointer and returns true
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004DCB11, oracle length 15 bytes.
// Grade: candidate -- generated for a repeated body shape and graded in the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); the batch README records the lane verdict.
//
// Retail bytes:  mov eax, dword ptr [esp + 4] ; mov dword ptr [eax], 2 ; mov al, 1 ; ret 4
// Tiny-body shape `GetConstantOut` synthesised from the instruction pattern (the class comes from the
// manifest label when it is a plain class name, else the method is landed under a descriptive
// placeholder); member names are descriptive. 
struct CClass_004dcb11 {
    bool vfunc_30(long* out);                  // the constant may be a float's bits; it is stored as a dword
};

bool CClass_004dcb11::vfunc_30(long* out)
{
    *out = 0x2;
    return true;
}