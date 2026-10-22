// CCreatureAction_SheatheItemToInventory::Clone() -- new CCreatureAction_SheatheItemToInventory(*this)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006A0C20, oracle length 34 bytes.
// Grade: landed -- byte-exact (RELOCATION_MATCH) and behaviour PASS under the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); generated for a repeated body shape, see the batch README.
//
// Retail bytes:  push esi ; push 0xb0 ; mov esi,ecx ; call operator new ; add esp,4 ; test eax,eax ; je N ; push esi ; mov ecx,eax ; call CCreatureAction_SheatheItemToInventory::CCreatureAction_SheatheItemToInventory(const CCreatureAction_SheatheItemToInventory&) ; pop esi ; ret ; N: xor eax,eax ; pop esi ; ret
// Virtual clone: allocates sizeof(CCreatureAction_SheatheItemToInventory) = 0xb0, null-checks and copy-constructs from
// `*this` through the out-of-line copy constructor @ 0x006A0C50; the constructor's `this` return
// is the result. One of 104 bodies of this shape (the copy constructor's label names the class).
struct CCreatureActionBase {
    virtual void v00();
};

struct CCreatureAction_SheatheItemToInventory : CCreatureActionBase {
    unsigned char _body[0xb0 - 4];    // sizeof(CCreatureAction_SheatheItemToInventory) = 0xb0
    CCreatureAction_SheatheItemToInventory(const CCreatureAction_SheatheItemToInventory& other);             // 0x006A0C50
    CCreatureActionBase* Clone();
};

CCreatureActionBase* CCreatureAction_SheatheItemToInventory::Clone()
{
    return new CCreatureAction_SheatheItemToInventory(*this);
}