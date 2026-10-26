// CCreatureAction_BriarRoseConcentrateInto::Clone() -- new CCreatureAction_BriarRoseConcentrateInto(*this)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008E8D30, oracle length 34 bytes.
// Grade: landed -- byte-exact (RELOCATION_MATCH) and behaviour PASS under the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); generated for a repeated body shape, see the batch README.
//
// Retail bytes:  push esi ; push 0x120 ; mov esi,ecx ; call operator new ; add esp,4 ; test eax,eax ; je N ; push esi ; mov ecx,eax ; call CCreatureAction_BriarRoseConcentrateInto::CCreatureAction_BriarRoseConcentrateInto(const CCreatureAction_BriarRoseConcentrateInto&) ; pop esi ; ret ; N: xor eax,eax ; pop esi ; ret
// Virtual clone: allocates sizeof(CCreatureAction_BriarRoseConcentrateInto) = 0x120, null-checks and copy-constructs from
// `*this` through the out-of-line copy constructor @ 0x008E8D60; the constructor's `this` return
// is the result. One of 104 bodies of this shape (the copy constructor's label names the class).
class CCreatureActionBase { public:
    virtual void v00();
};

class CCreatureAction_BriarRoseConcentrateInto : public CCreatureActionBase { public:
    unsigned char _body[0x120 - 4];    // sizeof(CCreatureAction_BriarRoseConcentrateInto) = 0x120
    CCreatureAction_BriarRoseConcentrateInto(const CCreatureAction_BriarRoseConcentrateInto& other);             // 0x008E8D60
    CCreatureActionBase* Clone();
};

CCreatureActionBase* CCreatureAction_BriarRoseConcentrateInto::Clone()
{
    return new CCreatureAction_BriarRoseConcentrateInto(*this);
}