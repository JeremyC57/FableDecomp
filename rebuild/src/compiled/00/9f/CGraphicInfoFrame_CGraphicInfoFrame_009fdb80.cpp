// CGraphicInfoFrame::CGraphicInfoFrame() -- base CBase_0099a2d0(), this vtable, then 1 members zeroed
// Leaf: ??0CGraphicInfoFrame@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009FDB80, oracle length 25 bytes.
// Grade: landed -- byte-exact (RELOCATION_MATCH) and behaviour PASS under the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); generated for a repeated body shape, see the batch README.
//
// Retail bytes:  push esi ; mov esi,ecx ; call CBaseClass::CBaseClass ; mov dword ptr [esi],offset CGraphicInfoFrame::`vftable' ; mov [esi+0x8],0 ; mov eax,esi ; pop esi ; ret
// Default constructor: base CBaseClass() @ 0x0099A2D0, this class's vtable (0x0129C878, named
// through rtti-vtables.tsv), then the 1 consecutive dword members at +0x8..+0x8
// are zeroed. The vtable store comes BEFORE the stores only when the zeroed
// members belong to an embedded member object whose inline constructor is inlined here (a plain
// init list or body assignment sinks the vtable store below them). Member types are not recoverable from the
// body (any 4-byte scalar or pointer); `long` models them. One of 10 bodies.
class CBaseClass {
public:
    virtual ~CBaseClass();   // 0x0099A2E0
    CBaseClass();                           // 0x0099A2D0
};

struct SZeroed {                        // the zeroed block is one embedded member object
    long m_00;                          // +0x8
    SZeroed() : m_00(0) { }           // inlined into every constructor that embeds it
};

class CGraphicInfoFrame : public CBaseClass { public:
    unsigned char _pad[0x4];
    SZeroed m_data;                        // +0x8
    CGraphicInfoFrame();
};

CGraphicInfoFrame::CGraphicInfoFrame()
    : CBaseClass()
    , m_data()
{
}