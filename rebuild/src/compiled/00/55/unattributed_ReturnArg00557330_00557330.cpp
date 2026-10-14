// CClass_00557330::ReturnArg_00557330(long) -- returns its argument (the object is unused)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00557330, oracle length 7 bytes.
// Grade: candidate -- generated for a repeated body shape and graded in the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); the batch README records the lane verdict.
//
// Retail bytes:  mov eax, dword ptr [esp + 4] ; ret 4
// Tiny-body shape `ReturnArg` synthesised from the instruction pattern (the class comes from the
// manifest label when it is a plain class name, else the method is landed under a descriptive
// placeholder); member names are descriptive. 
struct CClass_00557330 {
    long ReturnArg_00557330(long value);
};

long CClass_00557330::ReturnArg_00557330(long value)
{
    return value;
}