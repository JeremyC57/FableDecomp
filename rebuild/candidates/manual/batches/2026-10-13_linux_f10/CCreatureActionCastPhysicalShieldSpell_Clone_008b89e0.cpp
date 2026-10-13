// CCreatureAction_CastPhysicalShieldSpell::Clone() -- new CCreatureAction_CastPhysicalShieldSpell(*this)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008B89E0, oracle length 34 bytes.
// Grade: landed -- byte-exact (RELOCATION_MATCH) and behaviour PASS under the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); generated for a repeated body shape, see the batch README.
//
// Retail bytes:  push esi ; push 0x124 ; mov esi,ecx ; call operator new ; add esp,4 ; test eax,eax ; je N ; push esi ; mov ecx,eax ; call CCreatureAction_CastPhysicalShieldSpell::CCreatureAction_CastPhysicalShieldSpell(const CCreatureAction_CastPhysicalShieldSpell&) ; pop esi ; ret ; N: xor eax,eax ; pop esi ; ret
// Virtual clone: allocates sizeof(CCreatureAction_CastPhysicalShieldSpell) = 0x124, null-checks and copy-constructs from
// `*this` through the out-of-line copy constructor @ 0x008B8A10; the constructor's `this` return
// is the result. One of 104 bodies of this shape (the copy constructor's label names the class).
struct CCreatureActionBase {
    virtual void v00();
};

struct CCreatureAction_CastPhysicalShieldSpell : CCreatureActionBase {
    unsigned char _body[0x124 - 4];    // sizeof(CCreatureAction_CastPhysicalShieldSpell) = 0x124
    CCreatureAction_CastPhysicalShieldSpell(const CCreatureAction_CastPhysicalShieldSpell& other);             // 0x008B8A10
    CCreatureActionBase* Clone();
};

CCreatureActionBase* CCreatureAction_CastPhysicalShieldSpell::Clone()
{
    return new CCreatureAction_CastPhysicalShieldSpell(*this);
}
