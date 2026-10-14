// CNavigationLayer::CNavigationLayer() -- zeroes three pointer members and the flag byte at +0xc
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C20390, oracle length 16 bytes.
// Grade: candidate -- generated for a repeated body shape and graded in the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); the batch README records the lane verdict.
//
// Retail bytes:  mov eax, ecx ; xor ecx, ecx ; mov dword ptr [eax], ecx ; mov dword ptr [eax + 4], ecx ; mov dword ptr [eax + 8], ecx ; mov byte ptr [eax + 0xc], cl ; ret 
// Tiny-body shape `CtorZeroThreeAndByte`: a default constructor over three dword members and one
// byte flag (the dword members are initialised in declaration order from a zeroed ecx; `this` is
// returned). The class comes from the manifest label; member names are descriptive.
struct CNavigationLayer {
    void* m_pFirst;                        // +0
    void* m_pSecond;                       // +4
    void* m_pThird;                        // +8
    unsigned char m_flag;                  // +0xc
    CNavigationLayer();
};

CNavigationLayer::CNavigationLayer()
    : m_pFirst(0), m_pSecond(0), m_pThird(0), m_flag(0)
{
}