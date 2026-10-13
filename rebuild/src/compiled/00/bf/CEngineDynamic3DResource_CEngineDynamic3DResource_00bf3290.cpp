// CEngineDynamic3DResource::CEngineDynamic3DResource() -- base CBase_00ac1ec0(), this vtable, then 1 members zeroed
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00BF3290, oracle length 25 bytes.
// Grade: landed -- byte-exact (RELOCATION_MATCH) and behaviour PASS under the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); generated for a repeated body shape, see the batch README.
//
// Retail bytes:  push esi ; mov esi,ecx ; call CBase_00ac1ec0::CBase_00ac1ec0 ; mov dword ptr [esi],offset CEngineDynamic3DResource::`vftable' ; mov [esi+0x20],0 ; mov eax,esi ; pop esi ; ret
// Default constructor: base CBase_00ac1ec0() @ 0x00AC1EC0, this class's vtable (0x012A81B8, named
// through rtti-vtables.tsv), then the 1 consecutive dword members at +0x20..+0x20
// are zeroed. The vtable store comes BEFORE the stores only when the zeroed
// members belong to an embedded member object whose inline constructor is inlined here (a plain
// init list or body assignment sinks the vtable store below them). Member types are not recoverable from the
// body (any 4-byte scalar or pointer); `long` models them. One of 10 bodies.
struct CBase_00ac1ec0 {
    virtual void v00();
    CBase_00ac1ec0();                           // 0x00AC1EC0
};

struct SZeroed {                        // the zeroed block is one embedded member object
    long m_00;                          // +0x20
    SZeroed() : m_00(0) { }           // inlined into every constructor that embeds it
};

struct CEngineDynamic3DResource : CBase_00ac1ec0 {
    unsigned char _pad[0x1c];
    SZeroed m_data;                        // +0x20
    virtual void v00();
    CEngineDynamic3DResource();
};

CEngineDynamic3DResource::CEngineDynamic3DResource()
    : CBase_00ac1ec0()
    , m_data()
{
}