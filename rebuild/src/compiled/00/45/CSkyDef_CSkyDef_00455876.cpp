// CSkyDef::CSkyDef() -- base CBase_00430370(), this vtable, then 3 members zeroed
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00455876, oracle length 29 bytes.
// Grade: landed -- byte-exact (RELOCATION_MATCH) and behaviour PASS under the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); generated for a repeated body shape, see the batch README.
//
// Retail bytes:  push esi ; mov esi,ecx ; call CBase_00430370::CBase_00430370 ; xor eax,eax ; mov dword ptr [esi],offset CSkyDef::`vftable' ; mov [esi+0x74],eax ; mov [esi+0x78],eax ; mov [esi+0x7c],eax ; mov eax,esi ; pop esi ; ret
// Default constructor: base CBase_00430370() @ 0x00430370, this class's vtable (0x0123353C, named
// through rtti-vtables.tsv), then the 3 consecutive dword members at +0x74..+0x7c
// are zeroed from one cleared register. The vtable store comes BEFORE the stores only when the zeroed
// members belong to an embedded member object whose inline constructor is inlined here (a plain
// init list or body assignment sinks the vtable store below them). Member types are not recoverable from the
// body (any 4-byte scalar or pointer); `long` models them. One of 27 bodies.
struct CBase_00430370 {
    virtual void v00();
    CBase_00430370();                           // 0x00430370
};

struct SZeroed {                        // the zeroed block is one embedded member object
    long m_00;                          // +0x74
    long m_01;                          // +0x78
    long m_02;                          // +0x7c
    SZeroed() : m_00(0), m_01(0), m_02(0) { }           // inlined into every constructor that embeds it
};

struct CSkyDef : CBase_00430370 {
    unsigned char _pad[0x70];
    SZeroed m_data;                        // +0x74
    virtual void v00();
    CSkyDef();
};

CSkyDef::CSkyDef()
    : CBase_00430370()
    , m_data()
{
}