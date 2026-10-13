// CModeThunderLightningStorm::Clone() -- new CModeThunderLightningStorm(*this)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x007428E0, oracle length 34 bytes.
// Grade: landed -- byte-exact (RELOCATION_MATCH) and behaviour PASS under the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); generated for a repeated body shape, see the batch README.
//
// Retail bytes:  push esi ; push 0xc8 ; mov esi,ecx ; call operator new ; add esp,4 ; test eax,eax ; je N ; push esi ; mov ecx,eax ; call CModeThunderLightningStorm::CModeThunderLightningStorm(const CModeThunderLightningStorm&) ; pop esi ; ret ; N: xor eax,eax ; pop esi ; ret
// Virtual clone: allocates sizeof(CModeThunderLightningStorm) = 0xc8, null-checks and copy-constructs from
// `*this` through the out-of-line copy constructor @ 0x00740640; the constructor's `this` return
// is the result. One of 104 bodies of this shape (the copy constructor's label names the class).
struct undefined {
    virtual void v00();
};

struct CModeThunderLightningStorm : undefined {
    unsigned char _body[0xc8 - 4];    // sizeof(CModeThunderLightningStorm) = 0xc8
    CModeThunderLightningStorm(const CModeThunderLightningStorm& other);             // 0x00740640
    undefined* Clone();
};

undefined* CModeThunderLightningStorm::Clone()
{
    return new CModeThunderLightningStorm(*this);
}