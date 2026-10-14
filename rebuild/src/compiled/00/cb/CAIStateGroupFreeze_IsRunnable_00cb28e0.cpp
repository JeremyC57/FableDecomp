// CAIStateGroup_Freeze::IsRunnable() -- returns the global flag byte @ 0x0143E890
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00CB28E0, oracle length 6 bytes.
// Grade: candidate -- generated for a repeated body shape and graded in the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); the batch README records the lane verdict.
//
// Retail bytes:  mov al, byte ptr [0x143e890] ; ret 
// Tiny-body shape `GetGlobalFlag` synthesised from the instruction pattern (the class comes from the
// manifest label when it is a plain class name, else the method is landed under a descriptive
// placeholder); member names are descriptive. 
extern unsigned char g_flag_0143e890;         // 0x0143E890

struct CAIStateGroup_Freeze {
    unsigned char IsRunnable();
};

unsigned char CAIStateGroup_Freeze::IsRunnable()
{
    return g_flag_0143e890;
}