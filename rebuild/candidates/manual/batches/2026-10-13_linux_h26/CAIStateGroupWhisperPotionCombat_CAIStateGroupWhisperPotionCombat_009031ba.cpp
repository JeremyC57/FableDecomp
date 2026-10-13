// CAIStateGroup_WhisperPotionCombat::CAIStateGroup_WhisperPotionCombat() -- base CBase_008ff720(), this vtable, then 3 members zeroed
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009031BA, oracle length 29 bytes.
// Grade: landed -- byte-exact (RELOCATION_MATCH) and behaviour PASS under the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); generated for a repeated body shape, see the batch README.
//
// Retail bytes:  push esi ; mov esi,ecx ; call CBase_008ff720::CBase_008ff720 ; xor eax,eax ; mov dword ptr [esi],offset CAIStateGroup_WhisperPotionCombat::`vftable' ; mov [esi+0x1c],eax ; mov [esi+0x20],eax ; mov [esi+0x24],eax ; mov eax,esi ; pop esi ; ret
// Default constructor: base CBase_008ff720() @ 0x008FF720, this class's vtable (0x0128D5EC, named
// through rtti-vtables.tsv), then the 3 consecutive dword members at +0x1c..+0x24
// are zeroed from one cleared register. The vtable store comes BEFORE the stores only when the zeroed
// members belong to an embedded member object whose inline constructor is inlined here (a plain
// init list or body assignment sinks the vtable store below them). Member types are not recoverable from the
// body (any 4-byte scalar or pointer); `long` models them. One of 27 bodies.
struct CBase_008ff720 {
    virtual void v00();
    CBase_008ff720();                           // 0x008FF720
};

struct SZeroed {                        // the zeroed block is one embedded member object
    long m_00;                          // +0x1c
    long m_01;                          // +0x20
    long m_02;                          // +0x24
    SZeroed() : m_00(0), m_01(0), m_02(0) { }           // inlined into every constructor that embeds it
};

struct CAIStateGroup_WhisperPotionCombat : CBase_008ff720 {
    unsigned char _pad[0x18];
    SZeroed m_data;                        // +0x1c
    virtual void v00();
    CAIStateGroup_WhisperPotionCombat();
};

CAIStateGroup_WhisperPotionCombat::CAIStateGroup_WhisperPotionCombat()
    : CBase_008ff720()
    , m_data()
{
}
