// C3DMeshObject::C3DMeshObject() -- base CBase_0099a2d0(), this vtable, then 3 members zeroed
// Leaf: ??0C3DMeshObject@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004C31E0, oracle length 29 bytes.
// Grade: landed -- byte-exact (RELOCATION_MATCH) and behaviour PASS under the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); generated for a repeated body shape, see the batch README.
//
// Retail bytes:  push esi ; mov esi,ecx ; call CBaseClass::CBaseClass ; xor eax,eax ; mov dword ptr [esi],offset C3DMeshObject::`vftable' ; mov [esi+0x8],eax ; mov [esi+0xc],eax ; mov [esi+0x40],eax ; mov eax,esi ; pop esi ; ret
// Default constructor: base CBaseClass() @ 0x0099A2D0, this class's vtable (0x01239860, named
// through rtti-vtables.tsv), then the 3 consecutive dword members at +0x8..+0x40
// are zeroed from one cleared register. The vtable store comes BEFORE the stores only when the zeroed
// members belong to an embedded member object whose inline constructor is inlined here (a plain
// init list or body assignment sinks the vtable store below them). Member types are not recoverable from the
// body (any 4-byte scalar or pointer); `long` models them. One of 7 bodies.
class CBaseClass {
public:
    virtual ~CBaseClass();   // 0x0099A2E0
    CBaseClass();                           // 0x0099A2D0
};

struct SZeroed {                        // the zeroed block is one embedded member object
    long m_00;                          // +0x8
    long m_01;                          // +0xc
    unsigned char _gap2[0x30];
    unsigned char m_02;                 // +0x40
    SZeroed() : m_00(0), m_01(0), m_02(0) { }           // inlined into every constructor that embeds it
};

class C3DMeshObject : public CBaseClass { public:
    unsigned char _pad[0x4];
    SZeroed m_data;                        // +0x8
    C3DMeshObject();
};

C3DMeshObject::C3DMeshObject()
    : CBaseClass()
    , m_data()
{
}