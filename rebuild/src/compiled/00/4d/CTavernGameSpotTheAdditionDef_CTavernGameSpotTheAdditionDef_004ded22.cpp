// CTavernGameSpotTheAdditionDef::CTavernGameSpotTheAdditionDef() -- base CBase_0044c0c0(), this vtable, then 6 members zeroed
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004DED22, oracle length 38 bytes.
// Grade: landed -- byte-exact (RELOCATION_MATCH) and behaviour PASS under the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); generated for a repeated body shape, see the batch README.
//
// Retail bytes:  push esi ; mov esi,ecx ; call CBase_0044c0c0::CBase_0044c0c0 ; xor eax,eax ; mov dword ptr [esi],offset CTavernGameSpotTheAdditionDef::`vftable' ; mov [esi+0x38],eax ; mov [esi+0x3c],eax ; mov [esi+0x40],eax ; mov [esi+0x44],eax ; mov [esi+0x48],eax ; mov [esi+0x4c],eax ; mov eax,esi ; pop esi ; ret
// Default constructor: base CBase_0044c0c0() @ 0x0044C0C0, this class's vtable (0x01241A4C, named
// through rtti-vtables.tsv), then the 6 consecutive dword members at +0x38..+0x4c
// are zeroed from one cleared register. The vtable store comes BEFORE the stores only when the zeroed
// members belong to an embedded member object whose inline constructor is inlined here (a plain
// init list or body assignment sinks the vtable store below them). Member types are not recoverable from the
// body (any 4-byte scalar or pointer); `long` models them. One of 9 bodies.
struct CBase_0044c0c0 {
    virtual void v00();
    CBase_0044c0c0();                           // 0x0044C0C0
};

struct SZeroed {                        // the zeroed block is one embedded member object
    long m_00;                          // +0x38
    long m_01;                          // +0x3c
    long m_02;                          // +0x40
    long m_03;                          // +0x44
    long m_04;                          // +0x48
    long m_05;                          // +0x4c
    SZeroed() : m_00(0), m_01(0), m_02(0), m_03(0), m_04(0), m_05(0) { }           // inlined into every constructor that embeds it
};

class CTavernGameSpotTheAdditionDef : public CBase_0044c0c0 { public:
    unsigned char _pad[0x34];
    SZeroed m_data;                        // +0x38
    virtual void v00();
    CTavernGameSpotTheAdditionDef();
};

CTavernGameSpotTheAdditionDef::CTavernGameSpotTheAdditionDef()
    : CBase_0044c0c0()
    , m_data()
{
}