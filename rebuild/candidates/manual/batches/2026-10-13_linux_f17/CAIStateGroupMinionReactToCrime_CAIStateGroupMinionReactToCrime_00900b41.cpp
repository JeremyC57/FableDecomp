// CAIStateGroup_MinionReactToCrime::CAIStateGroup_MinionReactToCrime() -- base CBase_008fd255(), this vtable, then the embedded CIntelligentPointer member at +0x1b
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00900B41, oracle length 36 bytes.
// Grade: candidate -- still DIFFER(36v36) in the Linux lane (Wine + DDK 13.10.4035 cl.exe); generated for a
//        repeated body shape, see the batch README for the open question.
//
// Retail bytes:  push esi ; push edi ; mov esi,ecx ; call CBase_008fd255::CBase_008fd255 ; lea edi,[esi+0x1b] ; mov ecx,edi ; mov dword ptr [esi],offset CAIStateGroup_MinionReactToCrime::`vftable' ; call CMemberBase_00a01b10::CMemberBase_00a01b10 ; mov dword ptr [edi],offset CIntelligentPointer::`vftable' ; pop edi ; mov eax,esi ; pop esi ; ret
// Default constructor: base CBase_008fd255() @ 0x008FD255 out of line, this class's vtable
// (0x0128C558), then the embedded member of type CIntelligentPointer at +0x1b is constructed with its
// own constructor inlined: its base CMemberBase_00a01b10() @ 0x00A01B10 and its vtable (0x01244344).
// Class names come from rtti-vtables.tsv; base-constructor names from unique manifest labels.
// One of 73 bodies of this shape.
struct CBase_008fd255 {
    virtual void v00();
    CBase_008fd255();                           // 0x008FD255
};

struct CMemberBase_00a01b10 {
    virtual void v00();
    CMemberBase_00a01b10();                          // 0x00A01B10
};

struct CIntelligentPointer : CMemberBase_00a01b10 {
    virtual void v00();
    CIntelligentPointer() : CMemberBase_00a01b10() { }            // inlined into every constructor that embeds it
};

struct CAIStateGroup_MinionReactToCrime : CBase_008fd255 {
    unsigned char _pad[0x17];        // base members / earlier members, not touched here
    CIntelligentPointer m_member;                       // +0x1b
    virtual void v00();
    CAIStateGroup_MinionReactToCrime();
};

CAIStateGroup_MinionReactToCrime::CAIStateGroup_MinionReactToCrime()
    : CBase_008fd255()
    , m_member()
{
}
