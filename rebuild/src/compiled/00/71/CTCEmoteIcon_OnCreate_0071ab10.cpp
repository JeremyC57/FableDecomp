// CTCEmoteIcon::OnCreate() -- stores 0 at +0xc and 2 at +0x1c
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0071AB10, oracle length 15 bytes.
// Grade: candidate -- generated for a repeated body shape and graded in the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); the batch README records the lane verdict.
//
// Retail bytes:  mov dword ptr [ecx + 0xc], 0 ; mov dword ptr [ecx + 0x1c], 2 ; ret 
// Tiny-body shape `ResetPair` synthesised from the instruction pattern (the class comes from the
// manifest label when it is a plain class name, else the method is landed under a descriptive
// placeholder); member names are descriptive. 
class CTCEmoteIcon { public:
    unsigned char _pad1[0xc];
    long m_first;                          // +0xc
    unsigned char _pad2[0xc];
    long m_second;                          // +0x1c
    void OnCreate();
};

void CTCEmoteIcon::OnCreate()
{
    m_first = 0;
    m_second = 2;
}