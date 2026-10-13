// CCreatureAction_BodgeSideStrike::CCreatureAction_BodgeSideStrike() -- base CBase_00857e40(0) then this class's vtable
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008BFE00, oracle length 20 bytes.
// Grade: landed -- byte-exact (RELOCATION_MATCH) and behaviour PASS under the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); generated for a repeated body shape, see the batch README.
//
// Retail bytes:  push esi ; push 0x0 ; mov esi,ecx ; call CBase_00857e40::CBase_00857e40 ; mov dword ptr [esi],offset CCreatureAction_BodgeSideStrike::`vftable' ; mov eax,esi ; pop esi ; ret
// Default constructor that passes the constant 0 (0x0) to the base constructor @ 0x00857E40
// (an identifier/type code of this class in the base's scheme) and installs this class's vtable
// (0x0128005C, named through rtti-vtables.tsv). One of 12 bodies of this shape.
struct CBase_00857e40 {
    virtual void v00();
    CBase_00857e40(long typeCode);              // 0x00857E40
};

struct CCreatureAction_BodgeSideStrike : CBase_00857e40 {
    virtual void v00();
    CCreatureAction_BodgeSideStrike();
};

CCreatureAction_BodgeSideStrike::CCreatureAction_BodgeSideStrike()
    : CBase_00857e40(0)
{
}