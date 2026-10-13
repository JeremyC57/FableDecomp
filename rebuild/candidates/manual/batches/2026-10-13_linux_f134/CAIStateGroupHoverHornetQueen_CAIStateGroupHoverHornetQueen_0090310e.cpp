// CAIStateGroup_HoverHornetQueen::CAIStateGroup_HoverHornetQueen() -- base CBase_008ff720(), this vtable, then 6 members zeroed
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0090310E, oracle length 38 bytes.
// Grade: landed -- byte-exact (RELOCATION_MATCH) and behaviour PASS under the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); generated for a repeated body shape, see the batch README.
//
// Retail bytes:  push esi ; mov esi,ecx ; call CBase_008ff720::CBase_008ff720 ; xor eax,eax ; mov dword ptr [esi],offset CAIStateGroup_HoverHornetQueen::`vftable' ; mov [esi+0x40],eax ; mov [esi+0x44],eax ; mov [esi+0x48],eax ; mov [esi+0x50],eax ; mov [esi+0x54],eax ; mov [esi+0x58],eax ; mov eax,esi ; pop esi ; ret
// Default constructor: base CBase_008ff720() @ 0x008FF720, this class's vtable (0x0128D484, named
// through rtti-vtables.tsv), then the 6 consecutive dword members at +0x40..+0x58
// are zeroed from one cleared register. The vtable store comes BEFORE the stores only when the zeroed
// members belong to an embedded member object whose inline constructor is inlined here (a plain
// init list or body assignment sinks the vtable store below them). Member types are not recoverable from the
// body (any 4-byte scalar or pointer); `long` models them. One of 9 bodies.
struct CBase_008ff720 {
    virtual void v00();
    CBase_008ff720();                           // 0x008FF720
};

struct SZeroed {                        // the zeroed block is one embedded member object
    long m_00;                          // +0x40
    long m_01;                          // +0x44
    long m_02;                          // +0x48
    unsigned char _gap3[0x4];
    long m_03;                          // +0x50
    long m_04;                          // +0x54
    long m_05;                          // +0x58
    SZeroed() : m_00(0), m_01(0), m_02(0), m_03(0), m_04(0), m_05(0) { }           // inlined into every constructor that embeds it
};

struct CAIStateGroup_HoverHornetQueen : CBase_008ff720 {
    unsigned char _pad[0x3c];
    SZeroed m_data;                        // +0x40
    virtual void v00();
    CAIStateGroup_HoverHornetQueen();
};

CAIStateGroup_HoverHornetQueen::CAIStateGroup_HoverHornetQueen()
    : CBase_008ff720()
    , m_data()
{
}
