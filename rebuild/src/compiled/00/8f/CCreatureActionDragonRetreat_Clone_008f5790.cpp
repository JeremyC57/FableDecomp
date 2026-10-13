// CCreatureAction_DragonRetreat::Clone() -- new CCreatureAction_DragonRetreat(*this) with the implicit copy constructor inlined
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008F5790, oracle length 47 bytes.
// Grade: landed -- byte-exact (RELOCATION_MATCH) and behaviour PASS under the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); generated for a repeated body shape, see the batch README.
//
// Retail bytes:  push esi ; push edi ; push 0x114 ; mov edi,ecx ; call operator new ; mov esi,eax ; add esp,4 ; test esi,esi ; je N ; push edi ; mov ecx,esi ; call CCombatActionBase::CCombatActionBase(const CCombatActionBase&) ; pop edi ; mov dword ptr [esi],offset CCreatureAction_DragonRetreat::`vftable' ; mov eax,esi ; pop esi ; ret ; N: pop edi ; xor eax,eax ; pop esi ; ret
// Virtual clone of a class that adds no data to its base: the implicit copy constructor is
// inlined as the base copy constructor @ 0x0062F3B0 plus this class's vtable store
// (0x0128868C, named through rtti-vtables.tsv). sizeof = 0x114. One of 196 bodies of this shape.
struct CCombatActionBase {
    virtual void v00();
    unsigned char _body[0x114 - 4];    // sizeof(CCombatActionBase) = sizeof(CCreatureAction_DragonRetreat) = 0x114
    CCombatActionBase(const CCombatActionBase& other);     // 0x0062F3B0
};

struct CCreatureAction_DragonRetreat : CCombatActionBase {
    virtual void v00();                    // own vtable; copy constructor implicit
    CCombatActionBase* Clone();
};

CCombatActionBase* CCreatureAction_DragonRetreat::Clone()
{
    return new CCreatureAction_DragonRetreat(*this);
}