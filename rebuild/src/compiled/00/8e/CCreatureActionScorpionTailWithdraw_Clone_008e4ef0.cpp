// CCreatureAction_ScorpionTailWithdraw::Clone() -- new CCreatureAction_ScorpionTailWithdraw(*this)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008E4EF0, oracle length 34 bytes.
// Grade: landed -- byte-exact (RELOCATION_MATCH) and behaviour PASS under the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); generated for a repeated body shape, see the batch README.
//
// Retail bytes:  push esi ; push 0x120 ; mov esi,ecx ; call operator new ; add esp,4 ; test eax,eax ; je N ; push esi ; mov ecx,eax ; call CCreatureAction_ScorpionTailWithdraw::CCreatureAction_ScorpionTailWithdraw(const CCreatureAction_ScorpionTailWithdraw&) ; pop esi ; ret ; N: xor eax,eax ; pop esi ; ret
// Virtual clone: allocates sizeof(CCreatureAction_ScorpionTailWithdraw) = 0x120, null-checks and copy-constructs from
// `*this` through the out-of-line copy constructor @ 0x008E4F20; the constructor's `this` return
// is the result. One of 104 bodies of this shape (the copy constructor's label names the class).
class CCreatureActionBase { public:
    virtual void v00();
};

class CCreatureAction_ScorpionTailWithdraw : public CCreatureActionBase { public:
    unsigned char _body[0x120 - 4];    // sizeof(CCreatureAction_ScorpionTailWithdraw) = 0x120
    CCreatureAction_ScorpionTailWithdraw(const CCreatureAction_ScorpionTailWithdraw& other);             // 0x008E4F20
    CCreatureActionBase* Clone();
};

CCreatureActionBase* CCreatureAction_ScorpionTailWithdraw::Clone()
{
    return new CCreatureAction_ScorpionTailWithdraw(*this);
}