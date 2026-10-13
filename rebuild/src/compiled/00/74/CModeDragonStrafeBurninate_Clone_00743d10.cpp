// CModeDragonStrafeBurninate::Clone() -- new CModeDragonStrafeBurninate(*this)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00743D10, oracle length 34 bytes.
// Grade: landed -- byte-exact (RELOCATION_MATCH) and behaviour PASS under the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); generated for a repeated body shape, see the batch README.
//
// Retail bytes:  push esi ; push 0xc8 ; mov esi,ecx ; call operator new ; add esp,4 ; test eax,eax ; je N ; push esi ; mov ecx,eax ; call CModeDragonStrafeBurninate::CModeDragonStrafeBurninate(const CModeDragonStrafeBurninate&) ; pop esi ; ret ; N: xor eax,eax ; pop esi ; ret
// Virtual clone: allocates sizeof(CModeDragonStrafeBurninate) = 0xc8, null-checks and copy-constructs from
// `*this` through the out-of-line copy constructor @ 0x00743990; the constructor's `this` return
// is the result. One of 104 bodies of this shape (the copy constructor's label names the class).
struct undefined {
    virtual void v00();
};

struct CModeDragonStrafeBurninate : undefined {
    unsigned char _body[0xc8 - 4];    // sizeof(CModeDragonStrafeBurninate) = 0xc8
    CModeDragonStrafeBurninate(const CModeDragonStrafeBurninate& other);             // 0x00743990
    undefined* Clone();
};

undefined* CModeDragonStrafeBurninate::Clone()
{
    return new CModeDragonStrafeBurninate(*this);
}