// CCreatureModeDef::CCreatureModeDef() -- base CBase_0044c0c0(), this vtable, then 3 members zeroed
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004DE7DC, oracle length 29 bytes.
// Grade: landed -- byte-exact (RELOCATION_MATCH) and behaviour PASS under the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); generated for a repeated body shape, see the batch README.
//
// Retail bytes:  push esi ; mov esi,ecx ; call CBase_0044c0c0::CBase_0044c0c0 ; xor eax,eax ; mov dword ptr [esi],offset CCreatureModeDef::`vftable' ; mov [esi+0x34],eax ; mov [esi+0x38],eax ; mov [esi+0x3c],eax ; mov eax,esi ; pop esi ; ret
// Default constructor: base CBase_0044c0c0() @ 0x0044C0C0, this class's vtable (0x01241704, named
// through rtti-vtables.tsv), then the 3 consecutive dword members at +0x34..+0x3c
// are zeroed from one cleared register. The vtable store comes BEFORE the stores only when the zeroed
// members belong to an embedded member object whose inline constructor is inlined here (a plain
// init list or body assignment sinks the vtable store below them). Member types are not recoverable from the
// body (any 4-byte scalar or pointer); `long` models them. One of 27 bodies.
struct CBase_0044c0c0 {
    virtual void v00();
    CBase_0044c0c0();                           // 0x0044C0C0
};

struct SZeroed {                        // the zeroed block is one embedded member object
    long m_00;                          // +0x34
    long m_01;                          // +0x38
    long m_02;                          // +0x3c
    SZeroed() : m_00(0), m_01(0), m_02(0) { }           // inlined into every constructor that embeds it
};

struct CCreatureModeDef : CBase_0044c0c0 {
    unsigned char _pad[0x30];
    SZeroed m_data;                        // +0x34
    virtual void v00();
    CCreatureModeDef();
};

CCreatureModeDef::CCreatureModeDef()
    : CBase_0044c0c0()
    , m_data()
{
}