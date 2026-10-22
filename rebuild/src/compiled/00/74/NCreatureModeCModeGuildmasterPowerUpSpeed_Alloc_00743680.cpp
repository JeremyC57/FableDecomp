// NCreatureMode::CModeGuildmasterPowerUpSpeed::Alloc(CThing&) -- factory: operator new(0x90), then the out-of-line constructor @ 0x00742E60 with the incoming CThing& (its last vtable store is CModeGuildmasterPowerUpSpeed's); the stand-in passes the incoming register as `*this`
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00743680, oracle length 34 bytes.
// Grade: landed -- byte-exact (RELOCATION_MATCH) and behaviour PASS under the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); generated for a repeated body shape, see the batch README.
//
// Retail bytes:  push esi ; push 0x90 ; mov esi,ecx ; call operator new ; add esp,4 ; test eax,eax ; je N ; push esi ; mov ecx,eax ; call CModeGuildmasterPowerUpSpeed::CModeGuildmasterPowerUpSpeed(const CModeGuildmasterPowerUpSpeed&) ; pop esi ; ret ; N: xor eax,eax ; pop esi ; ret
// Virtual clone: allocates sizeof(CModeGuildmasterPowerUpSpeed) = 0x90, null-checks and copy-constructs from
// `*this` through the out-of-line copy constructor @ 0x00742E60; the constructor's `this` return
// is the result. One of 104 bodies of this shape (the copy constructor's label names the class).
struct undefined {
    virtual void v00();
};

struct CModeGuildmasterPowerUpSpeed : undefined {
    unsigned char _body[0x90 - 4];    // sizeof(CModeGuildmasterPowerUpSpeed) = 0x90
    CModeGuildmasterPowerUpSpeed(const CModeGuildmasterPowerUpSpeed& other);             // 0x00742E60
    undefined* Alloc();
};

undefined* CModeGuildmasterPowerUpSpeed::Alloc()
{
    return new CModeGuildmasterPowerUpSpeed(*this);
}