// CCreatureAction_BriarRoseForcePush::Clone() -- new CCreatureAction_BriarRoseForcePush(*this)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00957450, oracle length 34 bytes.
// Grade: landed -- byte-exact (RELOCATION_MATCH) and behaviour PASS under the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); generated for a repeated body shape, see the batch README.
//
// Retail bytes:  push esi ; push 0x11c ; mov esi,ecx ; call operator new ; add esp,4 ; test eax,eax ; je N ; push esi ; mov ecx,eax ; call CCreatureAction_BriarRoseForcePush::CCreatureAction_BriarRoseForcePush(const CCreatureAction_BriarRoseForcePush&) ; pop esi ; ret ; N: xor eax,eax ; pop esi ; ret
// Virtual clone: allocates sizeof(CCreatureAction_BriarRoseForcePush) = 0x11c, null-checks and copy-constructs from
// `*this` through the out-of-line copy constructor @ 0x00957480; the constructor's `this` return
// is the result. One of 104 bodies of this shape (the copy constructor's label names the class).
class CCreatureActionBase { public:
    virtual void v00();
};

class CCreatureAction_BriarRoseForcePush : public CCreatureActionBase { public:
    unsigned char _body[0x11c - 4];    // sizeof(CCreatureAction_BriarRoseForcePush) = 0x11c
    CCreatureAction_BriarRoseForcePush(const CCreatureAction_BriarRoseForcePush& other);             // 0x00957480
    CCreatureActionBase* Clone();
};

CCreatureActionBase* CCreatureAction_BriarRoseForcePush::Clone()
{
    return new CCreatureAction_BriarRoseForcePush(*this);
}