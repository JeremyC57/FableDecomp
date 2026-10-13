// CCreatureAction_ShopkeeperGetOut::Clone() -- new CCreatureAction_ShopkeeperGetOut(*this)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0084C140, oracle length 34 bytes.
// Grade: landed -- byte-exact (RELOCATION_MATCH) and behaviour PASS under the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); generated for a repeated body shape, see the batch README.
//
// Retail bytes:  push esi ; push 0xd4 ; mov esi,ecx ; call operator new ; add esp,4 ; test eax,eax ; je N ; push esi ; mov ecx,eax ; call CCreatureAction_ShopkeeperGetOut::CCreatureAction_ShopkeeperGetOut(const CCreatureAction_ShopkeeperGetOut&) ; pop esi ; ret ; N: xor eax,eax ; pop esi ; ret
// Virtual clone: allocates sizeof(CCreatureAction_ShopkeeperGetOut) = 0xd4, null-checks and copy-constructs from
// `*this` through the out-of-line copy constructor @ 0x0084C170; the constructor's `this` return
// is the result. One of 104 bodies of this shape (the copy constructor's label names the class).
struct CCreatureActionBase {
    virtual void v00();
};

struct CCreatureAction_ShopkeeperGetOut : CCreatureActionBase {
    unsigned char _body[0xd4 - 4];    // sizeof(CCreatureAction_ShopkeeperGetOut) = 0xd4
    CCreatureAction_ShopkeeperGetOut(const CCreatureAction_ShopkeeperGetOut& other);             // 0x0084C170
    CCreatureActionBase* Clone();
};

CCreatureActionBase* CCreatureAction_ShopkeeperGetOut::Clone()
{
    return new CCreatureAction_ShopkeeperGetOut(*this);
}
