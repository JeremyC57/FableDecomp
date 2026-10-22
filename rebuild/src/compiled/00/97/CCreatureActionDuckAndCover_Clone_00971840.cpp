// CCreatureAction_DuckAndCover::Clone() -- new CCreatureAction_DuckAndCover(*this)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00971840, oracle length 34 bytes.
// Grade: landed -- byte-exact (RELOCATION_MATCH) and behaviour PASS under the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); generated for a repeated body shape, see the batch README.
//
// Retail bytes:  push esi ; push 0x120 ; mov esi,ecx ; call operator new ; add esp,4 ; test eax,eax ; je N ; push esi ; mov ecx,eax ; call CCreatureAction_DuckAndCover::CCreatureAction_DuckAndCover(const CCreatureAction_DuckAndCover&) ; pop esi ; ret ; N: xor eax,eax ; pop esi ; ret
// Virtual clone: allocates sizeof(CCreatureAction_DuckAndCover) = 0x120, null-checks and copy-constructs from
// `*this` through the out-of-line copy constructor @ 0x00971870; the constructor's `this` return
// is the result. One of 104 bodies of this shape (the copy constructor's label names the class).
struct CCreatureActionBase {
    virtual void v00();
};

struct CCreatureAction_DuckAndCover : CCreatureActionBase {
    unsigned char _body[0x120 - 4];    // sizeof(CCreatureAction_DuckAndCover) = 0x120
    CCreatureAction_DuckAndCover(const CCreatureAction_DuckAndCover& other);             // 0x00971870
    CCreatureActionBase* Clone();
};

CCreatureActionBase* CCreatureAction_DuckAndCover::Clone()
{
    return new CCreatureAction_DuckAndCover(*this);
}