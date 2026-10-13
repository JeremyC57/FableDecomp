// CCreatureAction_DragonTakeOffIntoFlying::Clone() -- new CCreatureAction_DragonTakeOffIntoFlying(*this)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00953B90, oracle length 34 bytes.
// Grade: landed -- byte-exact (RELOCATION_MATCH) and behaviour PASS under the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); generated for a repeated body shape, see the batch README.
//
// Retail bytes:  push esi ; push 0x150 ; mov esi,ecx ; call operator new ; add esp,4 ; test eax,eax ; je N ; push esi ; mov ecx,eax ; call CCreatureAction_DragonTakeOffIntoFlying::CCreatureAction_DragonTakeOffIntoFlying(const CCreatureAction_DragonTakeOffIntoFlying&) ; pop esi ; ret ; N: xor eax,eax ; pop esi ; ret
// Virtual clone: allocates sizeof(CCreatureAction_DragonTakeOffIntoFlying) = 0x150, null-checks and copy-constructs from
// `*this` through the out-of-line copy constructor @ 0x00953BC0; the constructor's `this` return
// is the result. One of 104 bodies of this shape (the copy constructor's label names the class).
struct CCreatureActionBase {
    virtual void v00();
};

struct CCreatureAction_DragonTakeOffIntoFlying : CCreatureActionBase {
    unsigned char _body[0x150 - 4];    // sizeof(CCreatureAction_DragonTakeOffIntoFlying) = 0x150
    CCreatureAction_DragonTakeOffIntoFlying(const CCreatureAction_DragonTakeOffIntoFlying& other);             // 0x00953BC0
    CCreatureActionBase* Clone();
};

CCreatureActionBase* CCreatureAction_DragonTakeOffIntoFlying::Clone()
{
    return new CCreatureAction_DragonTakeOffIntoFlying(*this);
}