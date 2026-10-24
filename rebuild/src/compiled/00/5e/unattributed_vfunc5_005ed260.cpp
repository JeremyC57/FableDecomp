// CClass_005ed260::vfunc_5(long*) -- stores 0x3e4ccccd through the out-pointer and returns true
// Leaf: vfunc_5
// Retail: Fable.exe (TLC, VC7.1) function @ 0x005ED260, oracle length 15 bytes.
// Grade: candidate -- generated for a repeated body shape and graded in the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); the batch README records the lane verdict.
//
// Retail bytes:  mov eax, dword ptr [esp + 4] ; mov dword ptr [eax], 0x3e4ccccd ; mov al, 1 ; ret 4
// Tiny-body shape `GetConstantOut` synthesised from the instruction pattern (the class comes from the
// manifest label when it is a plain class name, else the method is landed under a descriptive
// placeholder); member names are descriptive. 
struct CClass_005ed260 {
    bool vfunc_5(long* out);                  // the constant may be a float's bits; it is stored as a dword
};

bool CClass_005ed260::vfunc_5(long* out)
{
    *out = 0x3e4ccccd;
    return true;
}