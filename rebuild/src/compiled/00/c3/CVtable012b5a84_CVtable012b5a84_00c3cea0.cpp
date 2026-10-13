// CVtable_012b5a84::CVtable_012b5a84() -- base CBase_00c4cf20(), this vtable, then 1 members zeroed
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C3CEA0, oracle length 25 bytes.
// Grade: landed -- byte-exact (RELOCATION_MATCH) and behaviour PASS under the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); generated for a repeated body shape, see the batch README.
//
// Retail bytes:  push esi ; mov esi,ecx ; call CBase_00c4cf20::CBase_00c4cf20 ; mov dword ptr [esi],offset CVtable_012b5a84::`vftable' ; mov [esi+0x7c],0 ; mov eax,esi ; pop esi ; ret
// Default constructor: base CBase_00c4cf20() @ 0x00C4CF20, this class's vtable (0x012B5A84, named
// through rtti-vtables.tsv), then the 1 consecutive dword members at +0x7c..+0x7c
// are zeroed. The vtable store comes BEFORE the stores only when the zeroed
// members belong to an embedded member object whose inline constructor is inlined here (a plain
// init list or body assignment sinks the vtable store below them). Member types are not recoverable from the
// body (any 4-byte scalar or pointer); `long` models them. One of 10 bodies.
struct CBase_00c4cf20 {
    virtual void v00();
    CBase_00c4cf20();                           // 0x00C4CF20
};

struct SZeroed {                        // the zeroed block is one embedded member object
    long m_00;                          // +0x7c
    SZeroed() : m_00(0) { }           // inlined into every constructor that embeds it
};

struct CVtable_012b5a84 : CBase_00c4cf20 {
    unsigned char _pad[0x78];
    SZeroed m_data;                        // +0x7c
    virtual void v00();
    CVtable_012b5a84();
};

CVtable_012b5a84::CVtable_012b5a84()
    : CBase_00c4cf20()
    , m_data()
{
}