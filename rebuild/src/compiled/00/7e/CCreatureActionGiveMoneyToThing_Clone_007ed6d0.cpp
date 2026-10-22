// CCreatureAction_GiveMoneyToThing::Clone() -- new CCreatureAction_GiveMoneyToThing(*this)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x007ED6D0, oracle length 34 bytes.
// Grade: landed -- byte-exact (RELOCATION_MATCH) and behaviour PASS under the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); generated for a repeated body shape, see the batch README.
//
// Retail bytes:  push esi ; push 0xb4 ; mov esi,ecx ; call operator new ; add esp,4 ; test eax,eax ; je N ; push esi ; mov ecx,eax ; call CCreatureAction_GiveMoneyToThing::CCreatureAction_GiveMoneyToThing(const CCreatureAction_GiveMoneyToThing&) ; pop esi ; ret ; N: xor eax,eax ; pop esi ; ret
// Virtual clone: allocates sizeof(CCreatureAction_GiveMoneyToThing) = 0xb4, null-checks and copy-constructs from
// `*this` through the out-of-line copy constructor @ 0x007ED700; the constructor's `this` return
// is the result. One of 104 bodies of this shape (the copy constructor's label names the class).
struct CCreatureActionBase {
    virtual void v00();
};

struct CCreatureAction_GiveMoneyToThing : CCreatureActionBase {
    unsigned char _body[0xb4 - 4];    // sizeof(CCreatureAction_GiveMoneyToThing) = 0xb4
    CCreatureAction_GiveMoneyToThing(const CCreatureAction_GiveMoneyToThing& other);             // 0x007ED700
    CCreatureActionBase* Clone();
};

CCreatureActionBase* CCreatureAction_GiveMoneyToThing::Clone()
{
    return new CCreatureAction_GiveMoneyToThing(*this);
}