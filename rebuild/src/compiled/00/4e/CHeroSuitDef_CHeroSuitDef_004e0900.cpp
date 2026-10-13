// CHeroSuitDef::CHeroSuitDef() -- base CBase_0044c0c0(), this vtable, then the embedded CMember_004df7ff member at +0x28
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004E0900, oracle length 26 bytes.
// Grade: landed -- byte-exact (RELOCATION_MATCH) and behaviour PASS under the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); generated for a repeated body shape, see the batch README.
//
// Retail bytes:  push esi ; mov esi,ecx ; call CBase_0044c0c0::CBase_0044c0c0 ; lea ecx,[esi+0x28] ; mov dword ptr [esi],offset CHeroSuitDef::`vftable' ; call CMember_004df7ff::CMember_004df7ff ; mov eax,esi ; pop esi ; ret
// Default constructor: base CBase_0044c0c0() @ 0x0044C0C0, this class's vtable (0x0124216C, named
// through rtti-vtables.tsv), then the member object at +0x28 is constructed out of line
// @ 0x004DF7FF. Base and member class names come from unique manifest labels, else from the
// callee address. One of 16 bodies of this shape.
struct CBase_0044c0c0 {
    virtual void v00();
    CBase_0044c0c0();                           // 0x0044C0C0
};

struct CMember_004df7ff {
    CMember_004df7ff();                              // 0x004DF7FF
};

struct CHeroSuitDef : CBase_0044c0c0 {
    unsigned char _pad[0x24];
    CMember_004df7ff m_member;                       // +0x28
    virtual void v00();
    CHeroSuitDef();
};

CHeroSuitDef::CHeroSuitDef()
    : CBase_0044c0c0()
    , m_member()
{
}