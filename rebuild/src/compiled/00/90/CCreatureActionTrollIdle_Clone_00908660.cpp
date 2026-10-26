// CCreatureAction_TrollIdle::Clone() -- new CCreatureAction_TrollIdle(*this) with the implicit copy constructor inlined
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00908660, oracle length 47 bytes.
// Grade: landed -- byte-exact (RELOCATION_MATCH) and behaviour PASS under the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); generated for a repeated body shape, see the batch README.
//
// Retail bytes:  push esi ; push edi ; push 0x114 ; mov edi,ecx ; call operator new ; mov esi,eax ; add esp,4 ; test esi,esi ; je N ; push edi ; mov ecx,esi ; call CCombatActionBase::CCombatActionBase(const CCombatActionBase&) ; pop edi ; mov dword ptr [esi],offset CCreatureAction_TrollIdle::`vftable' ; mov eax,esi ; pop esi ; ret ; N: pop edi ; xor eax,eax ; pop esi ; ret
// Virtual clone of a class that adds no data to its base: the implicit copy constructor is
// inlined as the base copy constructor @ 0x0062F3B0 plus this class's vtable store
// (0x0128E79C, named through rtti-vtables.tsv). sizeof = 0x114. One of 196 bodies of this shape.
class CCombatActionBase { public:
    virtual void v00();
    unsigned char _body[0x114 - 4];    // sizeof(CCombatActionBase) = sizeof(CCreatureAction_TrollIdle) = 0x114
    CCombatActionBase(const CCombatActionBase& other);     // 0x0062F3B0
};

class CCreatureAction_TrollIdle : public CCombatActionBase { public:
    virtual void v00();                    // own vtable; copy constructor implicit
    CCombatActionBase* Clone();
};

CCombatActionBase* CCreatureAction_TrollIdle::Clone()
{
    return new CCreatureAction_TrollIdle(*this);
}