// CCreatureAction_UseLamp::Clone() -- new CCreatureAction_UseLamp(*this)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0084C040, oracle length 34 bytes.
// Grade: landed -- byte-exact (RELOCATION_MATCH) and behaviour PASS under the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); generated for a repeated body shape, see the batch README.
//
// Retail bytes:  push esi ; push 0xb0 ; mov esi,ecx ; call operator new ; add esp,4 ; test eax,eax ; je N ; push esi ; mov ecx,eax ; call CCreatureAction_UseLamp::CCreatureAction_UseLamp(const CCreatureAction_UseLamp&) ; pop esi ; ret ; N: xor eax,eax ; pop esi ; ret
// Virtual clone: allocates sizeof(CCreatureAction_UseLamp) = 0xb0, null-checks and copy-constructs from
// `*this` through the out-of-line copy constructor @ 0x0084C070; the constructor's `this` return
// is the result. One of 104 bodies of this shape (the copy constructor's label names the class).
class CCreatureActionBase { public:
    virtual void v00();
};

class CCreatureAction_UseLamp : public CCreatureActionBase { public:
    unsigned char _body[0xb0 - 4];    // sizeof(CCreatureAction_UseLamp) = 0xb0
    CCreatureAction_UseLamp(const CCreatureAction_UseLamp& other);             // 0x0084C070
    CCreatureActionBase* Clone();
};

CCreatureActionBase* CCreatureAction_UseLamp::Clone()
{
    return new CCreatureAction_UseLamp(*this);
}