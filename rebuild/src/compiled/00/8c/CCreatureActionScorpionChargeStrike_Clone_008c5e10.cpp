// CCreatureAction_ScorpionChargeStrike::Clone() -- new CCreatureAction_ScorpionChargeStrike(*this)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008C5E10, oracle length 34 bytes.
// Grade: landed -- byte-exact (RELOCATION_MATCH) and behaviour PASS under the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); generated for a repeated body shape, see the batch README.
//
// Retail bytes:  push esi ; push 0x124 ; mov esi,ecx ; call operator new ; add esp,4 ; test eax,eax ; je N ; push esi ; mov ecx,eax ; call CCreatureAction_ScorpionChargeStrike::CCreatureAction_ScorpionChargeStrike(const CCreatureAction_ScorpionChargeStrike&) ; pop esi ; ret ; N: xor eax,eax ; pop esi ; ret
// Virtual clone: allocates sizeof(CCreatureAction_ScorpionChargeStrike) = 0x124, null-checks and copy-constructs from
// `*this` through the out-of-line copy constructor @ 0x008C5E40; the constructor's `this` return
// is the result. One of 104 bodies of this shape (the copy constructor's label names the class).
class CCreatureActionBase { public:
    virtual void v00();
};

class CCreatureAction_ScorpionChargeStrike : public CCreatureActionBase { public:
    unsigned char _body[0x124 - 4];    // sizeof(CCreatureAction_ScorpionChargeStrike) = 0x124
    CCreatureAction_ScorpionChargeStrike(const CCreatureAction_ScorpionChargeStrike& other);             // 0x008C5E40
    CCreatureActionBase* Clone();
};

CCreatureActionBase* CCreatureAction_ScorpionChargeStrike::Clone()
{
    return new CCreatureAction_ScorpionChargeStrike(*this);
}