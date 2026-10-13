// CCombatAction_GenericResponseToExplosion::Clone() -- new CCombatAction_GenericResponseToExplosion(*this)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0066A7B0, oracle length 34 bytes.
// Grade: landed -- byte-exact (RELOCATION_MATCH) and behaviour PASS under the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); generated for a repeated body shape, see the batch README.
//
// Retail bytes:  push esi ; push 0x138 ; mov esi,ecx ; call operator new ; add esp,4 ; test eax,eax ; je N ; push esi ; mov ecx,eax ; call CCombatAction_GenericResponseToExplosion::CCombatAction_GenericResponseToExplosion(const CCombatAction_GenericResponseToExplosion&) ; pop esi ; ret ; N: xor eax,eax ; pop esi ; ret
// Virtual clone: allocates sizeof(CCombatAction_GenericResponseToExplosion) = 0x138, null-checks and copy-constructs from
// `*this` through the out-of-line copy constructor @ 0x0066A7E0; the constructor's `this` return
// is the result. One of 104 bodies of this shape (the copy constructor's label names the class).
struct CCreatureActionBase {
    virtual void v00();
};

struct CCombatAction_GenericResponseToExplosion : CCreatureActionBase {
    unsigned char _body[0x138 - 4];    // sizeof(CCombatAction_GenericResponseToExplosion) = 0x138
    CCombatAction_GenericResponseToExplosion(const CCombatAction_GenericResponseToExplosion& other);             // 0x0066A7E0
    CCreatureActionBase* Clone();
};

CCreatureActionBase* CCombatAction_GenericResponseToExplosion::Clone()
{
    return new CCombatAction_GenericResponseToExplosion(*this);
}