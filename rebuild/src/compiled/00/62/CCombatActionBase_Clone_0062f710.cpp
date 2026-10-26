// CCombatActionBase::Clone() -- new CCombatActionBase(*this)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0062F710, oracle length 34 bytes.
// Grade: landed -- byte-exact (RELOCATION_MATCH) and behaviour PASS under the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); generated for a repeated body shape, see the batch README.
//
// Retail bytes:  push esi ; push 0x114 ; mov esi,ecx ; call operator new ; add esp,4 ; test eax,eax ; je N ; push esi ; mov ecx,eax ; call CCombatActionBase::CCombatActionBase(const CCombatActionBase&) ; pop esi ; ret ; N: xor eax,eax ; pop esi ; ret
// Virtual clone: allocates sizeof(CCombatActionBase) = 0x114, null-checks and copy-constructs from
// `*this` through the out-of-line copy constructor @ 0x0062F3B0; the constructor's `this` return
// is the result. One of 104 bodies of this shape (the copy constructor's label names the class).
class CCreatureActionBase { public:
    virtual void v00();
};

class CCombatActionBase : public CCreatureActionBase { public:
    unsigned char _body[0x114 - 4];    // sizeof(CCombatActionBase) = 0x114
    CCombatActionBase(const CCombatActionBase& other);             // 0x0062F3B0
    CCreatureActionBase* Clone();
};

CCreatureActionBase* CCombatActionBase::Clone()
{
    return new CCombatActionBase(*this);
}