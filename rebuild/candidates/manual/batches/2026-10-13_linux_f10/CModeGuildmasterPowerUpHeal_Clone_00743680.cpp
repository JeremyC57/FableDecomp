// CModeGuildmasterPowerUpHeal::Clone() -- new CModeGuildmasterPowerUpHeal(*this)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00743680, oracle length 34 bytes.
// Grade: landed -- byte-exact (RELOCATION_MATCH) and behaviour PASS under the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); generated for a repeated body shape, see the batch README.
//
// Retail bytes:  push esi ; push 0x90 ; mov esi,ecx ; call operator new ; add esp,4 ; test eax,eax ; je N ; push esi ; mov ecx,eax ; call CModeGuildmasterPowerUpHeal::CModeGuildmasterPowerUpHeal(const CModeGuildmasterPowerUpHeal&) ; pop esi ; ret ; N: xor eax,eax ; pop esi ; ret
// Virtual clone: allocates sizeof(CModeGuildmasterPowerUpHeal) = 0x90, null-checks and copy-constructs from
// `*this` through the out-of-line copy constructor @ 0x00742E60; the constructor's `this` return
// is the result. One of 104 bodies of this shape (the copy constructor's label names the class).
struct undefined {
    virtual void v00();
};

struct CModeGuildmasterPowerUpHeal : undefined {
    unsigned char _body[0x90 - 4];    // sizeof(CModeGuildmasterPowerUpHeal) = 0x90
    CModeGuildmasterPowerUpHeal(const CModeGuildmasterPowerUpHeal& other);             // 0x00742E60
    undefined* Clone();
};

undefined* CModeGuildmasterPowerUpHeal::Clone()
{
    return new CModeGuildmasterPowerUpHeal(*this);
}
