// CCreatureAction_BanditKingStuck::Clone() -- new CCreatureAction_BanditKingStuck(*this)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008C5D10, oracle length 34 bytes.
// Grade: landed -- byte-exact (RELOCATION_MATCH) and behaviour PASS under the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); generated for a repeated body shape, see the batch README.
//
// Retail bytes:  push esi ; push 0x120 ; mov esi,ecx ; call operator new ; add esp,4 ; test eax,eax ; je N ; push esi ; mov ecx,eax ; call CCreatureAction_BanditKingStuck::CCreatureAction_BanditKingStuck(const CCreatureAction_BanditKingStuck&) ; pop esi ; ret ; N: xor eax,eax ; pop esi ; ret
// Virtual clone: allocates sizeof(CCreatureAction_BanditKingStuck) = 0x120, null-checks and copy-constructs from
// `*this` through the out-of-line copy constructor @ 0x008C5D40; the constructor's `this` return
// is the result. One of 104 bodies of this shape (the copy constructor's label names the class).
struct CCreatureActionBase {
    virtual void v00();
};

struct CCreatureAction_BanditKingStuck : CCreatureActionBase {
    unsigned char _body[0x120 - 4];    // sizeof(CCreatureAction_BanditKingStuck) = 0x120
    CCreatureAction_BanditKingStuck(const CCreatureAction_BanditKingStuck& other);             // 0x008C5D40
    CCreatureActionBase* Clone();
};

CCreatureActionBase* CCreatureAction_BanditKingStuck::Clone()
{
    return new CCreatureAction_BanditKingStuck(*this);
}