// CCreatureAction_PickCrop::Clone() -- new CCreatureAction_PickCrop(*this)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009406B0, oracle length 34 bytes.
// Grade: landed -- byte-exact (RELOCATION_MATCH) and behaviour PASS under the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); generated for a repeated body shape, see the batch README.
//
// Retail bytes:  push esi ; push 0xb0 ; mov esi,ecx ; call operator new ; add esp,4 ; test eax,eax ; je N ; push esi ; mov ecx,eax ; call CCreatureAction_PickCrop::CCreatureAction_PickCrop(const CCreatureAction_PickCrop&) ; pop esi ; ret ; N: xor eax,eax ; pop esi ; ret
// Virtual clone: allocates sizeof(CCreatureAction_PickCrop) = 0xb0, null-checks and copy-constructs from
// `*this` through the out-of-line copy constructor @ 0x009406E0; the constructor's `this` return
// is the result. One of 104 bodies of this shape (the copy constructor's label names the class).
class CCreatureActionBase { public:
    virtual void v00();
};

class CCreatureAction_PickCrop : public CCreatureActionBase { public:
    unsigned char _body[0xb0 - 4];    // sizeof(CCreatureAction_PickCrop) = 0xb0
    CCreatureAction_PickCrop(const CCreatureAction_PickCrop& other);             // 0x009406E0
    CCreatureActionBase* Clone();
};

CCreatureActionBase* CCreatureAction_PickCrop::Clone()
{
    return new CCreatureAction_PickCrop(*this);
}