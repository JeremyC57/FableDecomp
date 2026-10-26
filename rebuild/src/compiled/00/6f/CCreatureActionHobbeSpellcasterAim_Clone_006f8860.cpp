// CCreatureAction_HobbeSpellcasterAim::Clone() -- new CCreatureAction_HobbeSpellcasterAim(*this)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006F8860, oracle length 34 bytes.
// Grade: landed -- byte-exact (RELOCATION_MATCH) and behaviour PASS under the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); generated for a repeated body shape, see the batch README.
//
// Retail bytes:  push esi ; push 0x11c ; mov esi,ecx ; call operator new ; add esp,4 ; test eax,eax ; je N ; push esi ; mov ecx,eax ; call CCreatureAction_HobbeSpellcasterAim::CCreatureAction_HobbeSpellcasterAim(const CCreatureAction_HobbeSpellcasterAim&) ; pop esi ; ret ; N: xor eax,eax ; pop esi ; ret
// Virtual clone: allocates sizeof(CCreatureAction_HobbeSpellcasterAim) = 0x11c, null-checks and copy-constructs from
// `*this` through the out-of-line copy constructor @ 0x006F8890; the constructor's `this` return
// is the result. One of 104 bodies of this shape (the copy constructor's label names the class).
class CCreatureActionBase { public:
    virtual void v00();
};

class CCreatureAction_HobbeSpellcasterAim : public CCreatureActionBase { public:
    unsigned char _body[0x11c - 4];    // sizeof(CCreatureAction_HobbeSpellcasterAim) = 0x11c
    CCreatureAction_HobbeSpellcasterAim(const CCreatureAction_HobbeSpellcasterAim& other);             // 0x006F8890
    CCreatureActionBase* Clone();
};

CCreatureActionBase* CCreatureAction_HobbeSpellcasterAim::Clone()
{
    return new CCreatureAction_HobbeSpellcasterAim(*this);
}