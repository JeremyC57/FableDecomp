// CCreatureAction_InterruptableMidAttackAutoTurn::Clone() -- new CCreatureAction_InterruptableMidAttackAutoTurn(*this)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0087E660, oracle length 34 bytes.
// Grade: landed -- byte-exact (RELOCATION_MATCH) and behaviour PASS under the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); generated for a repeated body shape, see the batch README.
//
// Retail bytes:  push esi ; push 0x12c ; mov esi,ecx ; call operator new ; add esp,4 ; test eax,eax ; je N ; push esi ; mov ecx,eax ; call CCreatureAction_InterruptableMidAttackAutoTurn::CCreatureAction_InterruptableMidAttackAutoTurn(const CCreatureAction_InterruptableMidAttackAutoTurn&) ; pop esi ; ret ; N: xor eax,eax ; pop esi ; ret
// Virtual clone: allocates sizeof(CCreatureAction_InterruptableMidAttackAutoTurn) = 0x12c, null-checks and copy-constructs from
// `*this` through the out-of-line copy constructor @ 0x0087E690; the constructor's `this` return
// is the result. One of 104 bodies of this shape (the copy constructor's label names the class).
struct CCreatureActionBase {
    virtual void v00();
};

struct CCreatureAction_InterruptableMidAttackAutoTurn : CCreatureActionBase {
    unsigned char _body[0x12c - 4];    // sizeof(CCreatureAction_InterruptableMidAttackAutoTurn) = 0x12c
    CCreatureAction_InterruptableMidAttackAutoTurn(const CCreatureAction_InterruptableMidAttackAutoTurn& other);             // 0x0087E690
    CCreatureActionBase* Clone();
};

CCreatureActionBase* CCreatureAction_InterruptableMidAttackAutoTurn::Clone()
{
    return new CCreatureAction_InterruptableMidAttackAutoTurn(*this);
}
