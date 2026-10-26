// CCreatureAction_BattleChargeImpact::Clone() -- new CCreatureAction_BattleChargeImpact(*this)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008CF0C0, oracle length 34 bytes.
// Grade: landed -- byte-exact (RELOCATION_MATCH) and behaviour PASS under the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); generated for a repeated body shape, see the batch README.
//
// Retail bytes:  push esi ; push 0x124 ; mov esi,ecx ; call operator new ; add esp,4 ; test eax,eax ; je N ; push esi ; mov ecx,eax ; call CCreatureAction_BattleChargeImpact::CCreatureAction_BattleChargeImpact(const CCreatureAction_BattleChargeImpact&) ; pop esi ; ret ; N: xor eax,eax ; pop esi ; ret
// Virtual clone: allocates sizeof(CCreatureAction_BattleChargeImpact) = 0x124, null-checks and copy-constructs from
// `*this` through the out-of-line copy constructor @ 0x008CF0F0; the constructor's `this` return
// is the result. One of 104 bodies of this shape (the copy constructor's label names the class).
class CCreatureActionBase { public:
    virtual void v00();
};

class CCreatureAction_BattleChargeImpact : public CCreatureActionBase { public:
    unsigned char _body[0x124 - 4];    // sizeof(CCreatureAction_BattleChargeImpact) = 0x124
    CCreatureAction_BattleChargeImpact(const CCreatureAction_BattleChargeImpact& other);             // 0x008CF0F0
    CCreatureActionBase* Clone();
};

CCreatureActionBase* CCreatureAction_BattleChargeImpact::Clone()
{
    return new CCreatureAction_BattleChargeImpact(*this);
}