// CCombatAction_GenericStrikeResponseKnockdown::Clone() -- new CCombatAction_GenericStrikeResponseKnockdown(*this)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00868400, oracle length 34 bytes.
// Grade: landed -- byte-exact (RELOCATION_MATCH) and behaviour PASS under the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); generated for a repeated body shape, see the batch README.
//
// Retail bytes:  push esi ; push 0x13c ; mov esi,ecx ; call operator new ; add esp,4 ; test eax,eax ; je N ; push esi ; mov ecx,eax ; call CCombatAction_GenericStrikeResponseKnockdown::CCombatAction_GenericStrikeResponseKnockdown(const CCombatAction_GenericStrikeResponseKnockdown&) ; pop esi ; ret ; N: xor eax,eax ; pop esi ; ret
// Virtual clone: allocates sizeof(CCombatAction_GenericStrikeResponseKnockdown) = 0x13c, null-checks and copy-constructs from
// `*this` through the out-of-line copy constructor @ 0x00868430; the constructor's `this` return
// is the result. One of 104 bodies of this shape (the copy constructor's label names the class).
struct CCreatureActionBase {
    virtual void v00();
};

struct CCombatAction_GenericStrikeResponseKnockdown : CCreatureActionBase {
    unsigned char _body[0x13c - 4];    // sizeof(CCombatAction_GenericStrikeResponseKnockdown) = 0x13c
    CCombatAction_GenericStrikeResponseKnockdown(const CCombatAction_GenericStrikeResponseKnockdown& other);             // 0x00868430
    CCreatureActionBase* Clone();
};

CCreatureActionBase* CCombatAction_GenericStrikeResponseKnockdown::Clone()
{
    return new CCombatAction_GenericStrikeResponseKnockdown(*this);
}
