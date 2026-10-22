// CCreatureAction_PlayerInteractionThreaten::Clone() -- new CCreatureAction_PlayerInteractionThreaten(*this)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0062F030, oracle length 34 bytes.
// Grade: landed -- byte-exact (RELOCATION_MATCH) and behaviour PASS under the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); generated for a repeated body shape, see the batch README.
//
// Retail bytes:  push esi ; push 0xb0 ; mov esi,ecx ; call operator new ; add esp,4 ; test eax,eax ; je N ; push esi ; mov ecx,eax ; call CCreatureAction_PlayerInteractionThreaten::CCreatureAction_PlayerInteractionThreaten(const CCreatureAction_PlayerInteractionThreaten&) ; pop esi ; ret ; N: xor eax,eax ; pop esi ; ret
// Virtual clone: allocates sizeof(CCreatureAction_PlayerInteractionThreaten) = 0xb0, null-checks and copy-constructs from
// `*this` through the out-of-line copy constructor @ 0x0062F060; the constructor's `this` return
// is the result. One of 104 bodies of this shape (the copy constructor's label names the class).
struct CCreatureActionBase {
    virtual void v00();
};

struct CCreatureAction_PlayerInteractionThreaten : CCreatureActionBase {
    unsigned char _body[0xb0 - 4];    // sizeof(CCreatureAction_PlayerInteractionThreaten) = 0xb0
    CCreatureAction_PlayerInteractionThreaten(const CCreatureAction_PlayerInteractionThreaten& other);             // 0x0062F060
    CCreatureActionBase* Clone();
};

CCreatureActionBase* CCreatureAction_PlayerInteractionThreaten::Clone()
{
    return new CCreatureAction_PlayerInteractionThreaten(*this);
}