// CCreatureAction_DragonSwoop::Clone() -- new CCreatureAction_DragonSwoop(*this)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008F57C0, oracle length 34 bytes.
// Grade: landed -- byte-exact (RELOCATION_MATCH) and behaviour PASS under the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); generated for a repeated body shape, see the batch README.
//
// Retail bytes:  push esi ; push 0x138 ; mov esi,ecx ; call operator new ; add esp,4 ; test eax,eax ; je N ; push esi ; mov ecx,eax ; call CCreatureAction_DragonSwoop::CCreatureAction_DragonSwoop(const CCreatureAction_DragonSwoop&) ; pop esi ; ret ; N: xor eax,eax ; pop esi ; ret
// Virtual clone: allocates sizeof(CCreatureAction_DragonSwoop) = 0x138, null-checks and copy-constructs from
// `*this` through the out-of-line copy constructor @ 0x008F57F0; the constructor's `this` return
// is the result. One of 104 bodies of this shape (the copy constructor's label names the class).
struct CCreatureActionBase {
    virtual void v00();
};

struct CCreatureAction_DragonSwoop : CCreatureActionBase {
    unsigned char _body[0x138 - 4];    // sizeof(CCreatureAction_DragonSwoop) = 0x138
    CCreatureAction_DragonSwoop(const CCreatureAction_DragonSwoop& other);             // 0x008F57F0
    CCreatureActionBase* Clone();
};

CCreatureActionBase* CCreatureAction_DragonSwoop::Clone()
{
    return new CCreatureAction_DragonSwoop(*this);
}
