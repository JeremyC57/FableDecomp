// CAIStateGroup_Freeze::IsRunnable() -- returns the global flag byte @ 0x013BB68C
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009224E0, oracle length 6 bytes.
// Grade: candidate -- generated for a repeated body shape and graded in the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); the batch README records the lane verdict.
//
// Retail bytes:  mov al, byte ptr [0x13bb68c] ; ret 
// Tiny-body shape `GetGlobalFlag` synthesised from the instruction pattern (the class comes from the
// manifest label when it is a plain class name, else the method is landed under a descriptive
// placeholder); member names are descriptive. 
extern unsigned char g_flag_013bb68c;         // 0x013BB68C

struct CAIStateGroup_Freeze {
    unsigned char IsRunnable();
};

unsigned char CAIStateGroup_Freeze::IsRunnable()
{
    return g_flag_013bb68c;
}