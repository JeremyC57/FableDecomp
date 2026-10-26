// CCreatureAction_DragonFireBlastLeft::Clone() -- new CCreatureAction_DragonFireBlastLeft(*this) with the implicit copy constructor inlined
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00953D30, oracle length 47 bytes.
// Grade: landed -- byte-exact (RELOCATION_MATCH) and behaviour PASS under the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); generated for a repeated body shape, see the batch README.
//
// Retail bytes:  push esi ; push edi ; push 0x124 ; mov edi,ecx ; call operator new ; mov esi,eax ; add esp,4 ; test esi,esi ; je N ; push edi ; mov ecx,esi ; call CCreatureAction_DragonFireBlastBase::CCreatureAction_DragonFireBlastBase(const CCreatureAction_DragonFireBlastBase&) ; pop edi ; mov dword ptr [esi],offset CCreatureAction_DragonFireBlastLeft::`vftable' ; mov eax,esi ; pop esi ; ret ; N: pop edi ; xor eax,eax ; pop esi ; ret
// Virtual clone of a class that adds no data to its base: the implicit copy constructor is
// inlined as the base copy constructor @ 0x00953D60 plus this class's vtable store
// (0x0129422C, named through rtti-vtables.tsv). sizeof = 0x124. One of 196 bodies of this shape.
class CCreatureAction_DragonFireBlastBase { public:
    virtual void v00();
    unsigned char _body[0x124 - 4];    // sizeof(CCreatureAction_DragonFireBlastBase) = sizeof(CCreatureAction_DragonFireBlastLeft) = 0x124
    CCreatureAction_DragonFireBlastBase(const CCreatureAction_DragonFireBlastBase& other);     // 0x00953D60
};

class CCreatureAction_DragonFireBlastLeft : public CCreatureAction_DragonFireBlastBase { public:
    virtual void v00();                    // own vtable; copy constructor implicit
    CCreatureAction_DragonFireBlastBase* Clone();
};

CCreatureAction_DragonFireBlastBase* CCreatureAction_DragonFireBlastLeft::Clone()
{
    return new CCreatureAction_DragonFireBlastLeft(*this);
}