// CTCTavernGameCoinGolf::QuitMidGame() -- sets the flag bytes at +0x28 and +0x21 and calls the member method @ 0x005E9B20 with the constant 6
// Retail: Fable.exe (TLC, VC7.1) function @ 0x007D29F0, oracle length 16 bytes.
// Grade: candidate -- generated for a repeated body shape and graded in the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); the batch README records the lane verdict.
//
// Retail bytes:  push 6 ; mov byte ptr [ecx + 0x28], 1 ; mov byte ptr [ecx + 0x21], 0 ; call 0x5e9b20 ; ret 
// Tiny-body shape `BeginWith` synthesised from the instruction pattern (the class comes from the
// manifest label when it is a plain class name, else the method is landed under a descriptive
// placeholder); member names are descriptive. 
class CTCTavernGameCoinGolf { public:
    unsigned char _pad[0x21];
    unsigned char m_flagB;                  // +0x21
    unsigned char _pad2[0x6];
    unsigned char m_flagA;                  // +0x28
    void Begin(long kind);                 // 0x005E9B20
    void QuitMidGame();
};

void CTCTavernGameCoinGolf::QuitMidGame()
{
    m_flagA = 1;
    m_flagB = 0;
    Begin(6);
}