// CAIStateGroupBase::CAIStateGroupBase() -- base CBase_0099a2d0(), this vtable, then the embedded CMember_0099e4b0 member at +0xc
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008FD255, oracle length 26 bytes.
// Grade: landed -- byte-exact (RELOCATION_MATCH) and behaviour PASS under the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); generated for a repeated body shape, see the batch README.
//
// Retail bytes:  push esi ; mov esi,ecx ; call CBase_0099a2d0::CBase_0099a2d0 ; lea ecx,[esi+0xc] ; mov dword ptr [esi],offset CAIStateGroupBase::`vftable' ; call CMember_0099e4b0::CMember_0099e4b0 ; mov eax,esi ; pop esi ; ret
// Default constructor: base CBase_0099a2d0() @ 0x0099A2D0, this class's vtable (0x01288C60, named
// through rtti-vtables.tsv), then the member object at +0xc is constructed out of line
// @ 0x0099E4B0. Base and member class names come from unique manifest labels, else from the
// callee address. One of 16 bodies of this shape.
struct CBase_0099a2d0 {
    virtual void v00();
    CBase_0099a2d0();                           // 0x0099A2D0
};

struct CMember_0099e4b0 {
    CMember_0099e4b0();                              // 0x0099E4B0
};

struct CAIStateGroupBase : CBase_0099a2d0 {
    unsigned char _pad[0x8];
    CMember_0099e4b0 m_member;                       // +0xc
    virtual void v00();
    CAIStateGroupBase();
};

CAIStateGroupBase::CAIStateGroupBase()
    : CBase_0099a2d0()
    , m_member()
{
}