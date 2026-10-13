// CAIStateGroup_JackOfBladesStage2::CAIStateGroup_JackOfBladesStage2() -- base CBase_00902fe8(), this vtable, then the embedded CIntelligentPointer member at +0x4c
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00903095, oracle length 36 bytes.
// Grade: landed -- byte-exact (RELOCATION_MATCH) and behaviour PASS under the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); generated for a repeated body shape, see the batch README.
//
// Retail bytes:  push esi ; push edi ; mov esi,ecx ; call CBase_00902fe8::CBase_00902fe8 ; lea edi,[esi+0x4c] ; mov ecx,edi ; mov dword ptr [esi],offset CAIStateGroup_JackOfBladesStage2::`vftable' ; call CMemberBase_00a01b10::CMemberBase_00a01b10 ; mov dword ptr [edi],offset CIntelligentPointer::`vftable' ; pop edi ; mov eax,esi ; pop esi ; ret
// Default constructor: base CBase_00902fe8() @ 0x00902FE8 out of line, this class's vtable
// (0x0128D3C4), then the embedded member of type CIntelligentPointer at +0x4c is constructed with its
// own constructor inlined: its base CMemberBase_00a01b10() @ 0x00A01B10 and its vtable (0x01238C6C).
// Class names come from rtti-vtables.tsv; base-constructor names from unique manifest labels.
// One of 73 bodies of this shape.
struct CBase_00902fe8 {
    virtual void v00();
    CBase_00902fe8();                           // 0x00902FE8
};

struct CMemberBase_00a01b10 {
    virtual void v00();
    CMemberBase_00a01b10();                          // 0x00A01B10
};

struct CIntelligentPointer : CMemberBase_00a01b10 {
    virtual void v00();
    CIntelligentPointer() : CMemberBase_00a01b10() { }            // inlined into every constructor that embeds it
};

struct CAIStateGroup_JackOfBladesStage2 : CBase_00902fe8 {
    unsigned char _pad[0x48];        // base members / earlier members, not touched here
    CIntelligentPointer m_member;                       // +0x4c
    virtual void v00();
    CAIStateGroup_JackOfBladesStage2();
};

CAIStateGroup_JackOfBladesStage2::CAIStateGroup_JackOfBladesStage2()
    : CBase_00902fe8()
    , m_member()
{
}