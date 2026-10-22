// CPlayer::GetCurrentMode() -- the dword at +0x24 of the object at +0x1c of the object at +0x4
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008FCFA0, oracle length 10 bytes.
// Grade: candidate -- generated for a repeated body shape and graded in the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); the batch README records the lane verdict.
//
// Retail bytes:  mov eax, dword ptr [ecx + 4] ; mov ecx, dword ptr [eax + 0x1c] ; mov eax, dword ptr [ecx + 0x24] ; ret 
// Tiny-body shape `GetThroughTwo` synthesised from the instruction pattern (the class comes from the
// manifest label when it is a plain class name, else the method is landed under a descriptive
// placeholder); member names are descriptive. 
struct CSecond {
    unsigned char _pad[0x24];
    void* m_value;                         // +0x24
};

struct CFirst {
    unsigned char _pad[0x1c];
    CSecond* m_pSecond;                    // +0x1c
};

struct CPlayer {
    unsigned char _pad[0x4];
    CFirst* m_pFirst;                      // +0x4
    void* GetCurrentMode();
};

void* CPlayer::GetCurrentMode()
{
    return m_pFirst->m_pSecond->m_value;
}