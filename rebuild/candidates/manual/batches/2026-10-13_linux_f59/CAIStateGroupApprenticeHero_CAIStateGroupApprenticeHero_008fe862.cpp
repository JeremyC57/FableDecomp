// CAIStateGroup_ApprenticeHero::CAIStateGroup_ApprenticeHero() -- base CBase_008fd255(), this vtable, then the embedded CMember_0099e4b0 member at +0x24
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008FE862, oracle length 26 bytes.
// Grade: landed -- byte-exact (RELOCATION_MATCH) and behaviour PASS under the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); generated for a repeated body shape, see the batch README.
//
// Retail bytes:  push esi ; mov esi,ecx ; call CBase_008fd255::CBase_008fd255 ; lea ecx,[esi+0x24] ; mov dword ptr [esi],offset CAIStateGroup_ApprenticeHero::`vftable' ; call CMember_0099e4b0::CMember_0099e4b0 ; mov eax,esi ; pop esi ; ret
// Default constructor: base CBase_008fd255() @ 0x008FD255, this class's vtable (0x0128AEC4, named
// through rtti-vtables.tsv), then the member object at +0x24 is constructed out of line
// @ 0x0099E4B0. Base and member class names come from unique manifest labels, else from the
// callee address. One of 16 bodies of this shape.
struct CBase_008fd255 {
    virtual void v00();
    CBase_008fd255();                           // 0x008FD255
};

struct CMember_0099e4b0 {
    CMember_0099e4b0();                              // 0x0099E4B0
};

struct CAIStateGroup_ApprenticeHero : CBase_008fd255 {
    unsigned char _pad[0x20];
    CMember_0099e4b0 m_member;                       // +0x24
    virtual void v00();
    CAIStateGroup_ApprenticeHero();
};

CAIStateGroup_ApprenticeHero::CAIStateGroup_ApprenticeHero()
    : CBase_008fd255()
    , m_member()
{
}
