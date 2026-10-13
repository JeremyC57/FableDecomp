// CFrameRateSmoother::CFrameRateSmoother() -- base CInitBaseClass(), this vtable, then 3 members zeroed
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00419747, oracle length 29 bytes.
// Grade: landed -- byte-exact (RELOCATION_MATCH) and behaviour PASS under the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); generated for a repeated body shape, see the batch README.
//
// Retail bytes:  push esi ; mov esi,ecx ; call CInitBaseClass::CInitBaseClass ; xor eax,eax ; mov dword ptr [esi],offset CFrameRateSmoother::`vftable' ; mov [esi+0x8],eax ; mov [esi+0xc],eax ; mov [esi+0x10],eax ; mov eax,esi ; pop esi ; ret
// Default constructor: base CInitBaseClass() @ 0x0099A310, this class's vtable (0x0122F168, named
// through rtti-vtables.tsv), then the 3 consecutive dword members at +0x8..+0x10
// are zeroed from one cleared register. The vtable store comes BEFORE the stores only when the zeroed
// members belong to an embedded member object whose inline constructor is inlined here (a plain
// init list or body assignment sinks the vtable store below them). Member types are not recoverable from the
// body (any 4-byte scalar or pointer); `long` models them. One of 27 bodies.
struct CInitBaseClass {
    virtual void v00();
    CInitBaseClass();                           // 0x0099A310
};

struct SZeroed {                        // the zeroed block is one embedded member object
    long m_00;                          // +0x8
    long m_01;                          // +0xc
    long m_02;                          // +0x10
    SZeroed() : m_00(0), m_01(0), m_02(0) { }           // inlined into every constructor that embeds it
};

struct CFrameRateSmoother : CInitBaseClass {
    unsigned char _pad[0x4];
    SZeroed m_data;                        // +0x8
    virtual void v00();
    CFrameRateSmoother();
};

CFrameRateSmoother::CFrameRateSmoother()
    : CInitBaseClass()
    , m_data()
{
}
