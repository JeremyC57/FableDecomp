// CFlammableDef::CFlammableDef() -- base CBase_0044c0c0(), this vtable, then the embedded CMember_004e0a05 member at +0x38
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004E284B, oracle length 26 bytes.
// Grade: landed -- byte-exact (RELOCATION_MATCH) and behaviour PASS under the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); generated for a repeated body shape, see the batch README.
//
// Retail bytes:  push esi ; mov esi,ecx ; call CBase_0044c0c0::CBase_0044c0c0 ; lea ecx,[esi+0x38] ; mov dword ptr [esi],offset CFlammableDef::`vftable' ; call CMember_004e0a05::CMember_004e0a05 ; mov eax,esi ; pop esi ; ret
// Default constructor: base CBase_0044c0c0() @ 0x0044C0C0, this class's vtable (0x01242984, named
// through rtti-vtables.tsv), then the member object at +0x38 is constructed out of line
// @ 0x004E0A05. Base and member class names come from unique manifest labels, else from the
// callee address. One of 16 bodies of this shape.
struct CBase_0044c0c0 {
    virtual void v00();
    CBase_0044c0c0();                           // 0x0044C0C0
};

struct CMember_004e0a05 {
    CMember_004e0a05();                              // 0x004E0A05
};

class CFlammableDef : public CBase_0044c0c0 { public:
    unsigned char _pad[0x34];
    CMember_004e0a05 m_member;                       // +0x38
    virtual void v00();
    CFlammableDef();
};

CFlammableDef::CFlammableDef()
    : CBase_0044c0c0()
    , m_member()
{
}