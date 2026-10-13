// CCreatureAction_PickUpTankard::Clone() -- new CCreatureAction_PickUpTankard(*this) with the implicit copy constructor inlined
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0094B6A0, oracle length 47 bytes.
// Grade: landed -- byte-exact (RELOCATION_MATCH) and behaviour PASS under the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); generated for a repeated body shape, see the batch README.
//
// Retail bytes:  push esi ; push edi ; push 0xb4 ; mov edi,ecx ; call operator new ; mov esi,eax ; add esp,4 ; test esi,esi ; je N ; push edi ; mov ecx,esi ; call CCreatureAction_PickUpJug::CCreatureAction_PickUpJug(const CCreatureAction_PickUpJug&) ; pop edi ; mov dword ptr [esi],offset CCreatureAction_PickUpTankard::`vftable' ; mov eax,esi ; pop esi ; ret ; N: pop edi ; xor eax,eax ; pop esi ; ret
// Virtual clone of a class that adds no data to its base: the implicit copy constructor is
// inlined as the base copy constructor @ 0x0084C9A0 plus this class's vtable store
// (0x01292D6C, named through rtti-vtables.tsv). sizeof = 0xb4. One of 196 bodies of this shape.
struct CCreatureAction_PickUpJug {
    virtual void v00();
    unsigned char _body[0xb4 - 4];    // sizeof(CCreatureAction_PickUpJug) = sizeof(CCreatureAction_PickUpTankard) = 0xb4
    CCreatureAction_PickUpJug(const CCreatureAction_PickUpJug& other);     // 0x0084C9A0
};

struct CCreatureAction_PickUpTankard : CCreatureAction_PickUpJug {
    virtual void v00();                    // own vtable; copy constructor implicit
    CCreatureAction_PickUpJug* Clone();
};

CCreatureAction_PickUpJug* CCreatureAction_PickUpTankard::Clone()
{
    return new CCreatureAction_PickUpTankard(*this);
}