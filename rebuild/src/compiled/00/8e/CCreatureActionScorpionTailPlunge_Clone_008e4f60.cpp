// CCreatureAction_ScorpionTailPlunge::Clone() -- new CCreatureAction_ScorpionTailPlunge(*this)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008E4F60, oracle length 34 bytes.
// Grade: landed -- byte-exact (RELOCATION_MATCH) and behaviour PASS under the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); generated for a repeated body shape, see the batch README.
//
// Retail bytes:  push esi ; push 0x120 ; mov esi,ecx ; call operator new ; add esp,4 ; test eax,eax ; je N ; push esi ; mov ecx,eax ; call CCreatureAction_ScorpionTailPlunge::CCreatureAction_ScorpionTailPlunge(const CCreatureAction_ScorpionTailPlunge&) ; pop esi ; ret ; N: xor eax,eax ; pop esi ; ret
// Virtual clone: allocates sizeof(CCreatureAction_ScorpionTailPlunge) = 0x120, null-checks and copy-constructs from
// `*this` through the out-of-line copy constructor @ 0x008E4F90; the constructor's `this` return
// is the result. One of 104 bodies of this shape (the copy constructor's label names the class).
class CCreatureActionBase { public:
    virtual void v00();
};

class CCreatureAction_ScorpionTailPlunge : public CCreatureActionBase { public:
    unsigned char _body[0x120 - 4];    // sizeof(CCreatureAction_ScorpionTailPlunge) = 0x120
    CCreatureAction_ScorpionTailPlunge(const CCreatureAction_ScorpionTailPlunge& other);             // 0x008E4F90
    CCreatureActionBase* Clone();
};

CCreatureActionBase* CCreatureAction_ScorpionTailPlunge::Clone()
{
    return new CCreatureAction_ScorpionTailPlunge(*this);
}