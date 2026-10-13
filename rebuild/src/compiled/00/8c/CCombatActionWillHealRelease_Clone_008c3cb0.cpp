// CCombatAction_WillHealRelease::Clone() -- new CCombatAction_WillHealRelease(*this) with the implicit copy constructor inlined
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008C3CB0, oracle length 47 bytes.
// Grade: landed -- byte-exact (RELOCATION_MATCH) and behaviour PASS under the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); generated for a repeated body shape, see the batch README.
//
// Retail bytes:  push esi ; push edi ; push 0x114 ; mov edi,ecx ; call operator new ; mov esi,eax ; add esp,4 ; test esi,esi ; je N ; push edi ; mov ecx,esi ; call CCombatActionBase::CCombatActionBase(const CCombatActionBase&) ; pop edi ; mov dword ptr [esi],offset CCombatAction_WillHealRelease::`vftable' ; mov eax,esi ; pop esi ; ret ; N: pop edi ; xor eax,eax ; pop esi ; ret
// Virtual clone of a class that adds no data to its base: the implicit copy constructor is
// inlined as the base copy constructor @ 0x0062F3B0 plus this class's vtable store
// (0x01280644, named through rtti-vtables.tsv). sizeof = 0x114. One of 196 bodies of this shape.
struct CCombatActionBase {
    virtual void v00();
    unsigned char _body[0x114 - 4];    // sizeof(CCombatActionBase) = sizeof(CCombatAction_WillHealRelease) = 0x114
    CCombatActionBase(const CCombatActionBase& other);     // 0x0062F3B0
};

struct CCombatAction_WillHealRelease : CCombatActionBase {
    virtual void v00();                    // own vtable; copy constructor implicit
    CCombatActionBase* Clone();
};

CCombatActionBase* CCombatAction_WillHealRelease::Clone()
{
    return new CCombatAction_WillHealRelease(*this);
}