// CVertexStreamFormat::CVertexStreamFormat() -- base CBase_0099a2d0(), this vtable, then 3 members zeroed
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00AB37F0, oracle length 29 bytes.
// Grade: landed -- byte-exact (RELOCATION_MATCH) and behaviour PASS under the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); generated for a repeated body shape, see the batch README.
//
// Retail bytes:  push esi ; mov esi,ecx ; call CBase_0099a2d0::CBase_0099a2d0 ; xor eax,eax ; mov dword ptr [esi],offset CVertexStreamFormat::`vftable' ; mov [esi+0x4],eax ; mov [esi+0x8],eax ; mov [esi+0xc],eax ; mov eax,esi ; pop esi ; ret
// Default constructor: base CBase_0099a2d0() @ 0x0099A2D0, this class's vtable (0x0129D578, named
// through rtti-vtables.tsv), then the 3 consecutive dword members at +0x4..+0xc
// are zeroed from one cleared register. The vtable store comes BEFORE the stores only when the zeroed
// members belong to an embedded member object whose inline constructor is inlined here (a plain
// init list or body assignment sinks the vtable store below them). Member types are not recoverable from the
// body (any 4-byte scalar or pointer); `long` models them. One of 7 bodies.
struct CBase_0099a2d0 {
    virtual void v00();
    CBase_0099a2d0();                           // 0x0099A2D0
};

struct SZeroed {                        // the zeroed block is one embedded member object
    long m_00;                          // +0x4
    long m_01;                          // +0x8
    long m_02;                          // +0xc
    SZeroed() : m_00(0), m_01(0), m_02(0) { }           // inlined into every constructor that embeds it
};

struct CVertexStreamFormat : CBase_0099a2d0 {
    SZeroed m_data;                        // +0x4
    virtual void v00();
    CVertexStreamFormat();
};

CVertexStreamFormat::CVertexStreamFormat()
    : CBase_0099a2d0()
    , m_data()
{
}
