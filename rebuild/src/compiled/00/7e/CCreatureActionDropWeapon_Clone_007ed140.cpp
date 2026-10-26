// CCreatureAction_DropWeapon::Clone() -- new CCreatureAction_DropWeapon(*this) with the implicit copy constructor inlined
// Retail: Fable.exe (TLC, VC7.1) function @ 0x007ED140, oracle length 47 bytes.
// Grade: landed -- byte-exact (RELOCATION_MATCH) and behaviour PASS under the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); generated for a repeated body shape, see the batch README.
//
// Retail bytes:  push esi ; push edi ; push 0xa8 ; mov edi,ecx ; call operator new ; mov esi,eax ; add esp,4 ; test esi,esi ; je N ; push edi ; mov ecx,esi ; call CCreatureActionBase::CCreatureActionBase(const CCreatureActionBase&) ; pop edi ; mov dword ptr [esi],offset CCreatureAction_DropWeapon::`vftable' ; mov eax,esi ; pop esi ; ret ; N: pop edi ; xor eax,eax ; pop esi ; ret
// Virtual clone of a class that adds no data to its base: the implicit copy constructor is
// inlined as the base copy constructor @ 0x0062EBC0 plus this class's vtable store
// (0x01271274, named through rtti-vtables.tsv). sizeof = 0xa8. One of 196 bodies of this shape.
class CCreatureActionBase { public:
    virtual void v00();
    unsigned char _body[0xa8 - 4];    // sizeof(CCreatureActionBase) = sizeof(CCreatureAction_DropWeapon) = 0xa8
    CCreatureActionBase(const CCreatureActionBase& other);     // 0x0062EBC0
};

class CCreatureAction_DropWeapon : public CCreatureActionBase { public:
    virtual void v00();                    // own vtable; copy constructor implicit
    CCreatureActionBase* Clone();
};

CCreatureActionBase* CCreatureAction_DropWeapon::Clone()
{
    return new CCreatureAction_DropWeapon(*this);
}