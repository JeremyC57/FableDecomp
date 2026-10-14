// CManager::GetKeyPressed() -- returns the 16-bit member at +0x6
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C1BAA0, oracle length 5 bytes.
// Grade: candidate -- generated for a repeated body shape and graded in the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); the batch README records the lane verdict.
//
// Retail bytes:  mov ax, word ptr [ecx + 6] ; ret 
// Tiny-body shape `GetWord` synthesised from the instruction pattern (the class comes from the
// manifest label when it is a plain class name, else the method is landed under a descriptive
// placeholder); member names are descriptive. 
struct CManager {
    unsigned char _pad[0x6];
    short m_value;                         // +0x6
    short GetKeyPressed();
};

short CManager::GetKeyPressed()
{
    return m_value;
}