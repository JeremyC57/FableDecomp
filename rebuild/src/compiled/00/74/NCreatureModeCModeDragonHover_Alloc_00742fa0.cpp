// NCreatureMode::CModeDragonHover::Alloc(CThing&) -- factory: operator new(0x80), then the out-of-line constructor @ 0x00741B60 with the incoming CThing& (its last vtable store is CModeDragonHover's); the stand-in passes the incoming register as `*this`
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00742FA0, oracle length 34 bytes.
// Grade: landed -- byte-exact (RELOCATION_MATCH) and behaviour PASS under the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); generated for a repeated body shape, see the batch README.
//
// Retail bytes:  push esi ; push 0x80 ; mov esi,ecx ; call operator new ; add esp,4 ; test eax,eax ; je N ; push esi ; mov ecx,eax ; call CModeDragonHover::CModeDragonHover(const CModeDragonHover&) ; pop esi ; ret ; N: xor eax,eax ; pop esi ; ret
// Virtual clone: allocates sizeof(CModeDragonHover) = 0x80, null-checks and copy-constructs from
// `*this` through the out-of-line copy constructor @ 0x00741B60; the constructor's `this` return
// is the result. One of 104 bodies of this shape (the copy constructor's label names the class).
struct undefined {
    virtual void v00();
};

struct CModeDragonHover : undefined {
    unsigned char _body[0x80 - 4];    // sizeof(CModeDragonHover) = 0x80
    CModeDragonHover(const CModeDragonHover& other);             // 0x00741B60
    undefined* Alloc();
};

undefined* CModeDragonHover::Alloc()
{
    return new CModeDragonHover(*this);
}