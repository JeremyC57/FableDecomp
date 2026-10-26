// CTCBoss::IncrementBossPhase() -- increments the counter at +0x14 when the flag byte at +0x20 is set
// Retail: Fable.exe (TLC, VC7.1) function @ 0x007AED30, oracle length 11 bytes.
// Grade: candidate -- generated for a repeated body shape and graded in the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); the batch README records the lane verdict.
//
// Retail bytes:  mov al, byte ptr [ecx + 0x20] ; test al, al ; je 0x7aed3a ; inc dword ptr [ecx + 0x14] ; ret 
// Tiny-body shape `CountIfEnabled` synthesised from the instruction pattern (the class comes from the
// manifest label when it is a plain class name, else the method is landed under a descriptive
// placeholder); member names are descriptive. 
class CTCBoss { public:
    unsigned char _pad1[0x14];
    long m_count;                           // +0x14
    unsigned char _pad2[0x8];
    unsigned char m_enabled;                // +0x20
    void IncrementBossPhase();
};

void CTCBoss::IncrementBossPhase()
{
    if (m_enabled)
        ++m_count;
}