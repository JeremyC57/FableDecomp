// CCreatureAction_ActivateQuest::Clone() -- new CCreatureAction_ActivateQuest(*this)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0084C8D0, oracle length 34 bytes.
// Grade: landed -- byte-exact (RELOCATION_MATCH) and behaviour PASS under the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); generated for a repeated body shape, see the batch README.
//
// Retail bytes:  push esi ; push 0xb8 ; mov esi,ecx ; call operator new ; add esp,4 ; test eax,eax ; je N ; push esi ; mov ecx,eax ; call CCreatureAction_ActivateQuest::CCreatureAction_ActivateQuest(const CCreatureAction_ActivateQuest&) ; pop esi ; ret ; N: xor eax,eax ; pop esi ; ret
// Virtual clone: allocates sizeof(CCreatureAction_ActivateQuest) = 0xb8, null-checks and copy-constructs from
// `*this` through the out-of-line copy constructor @ 0x0084C900; the constructor's `this` return
// is the result. One of 104 bodies of this shape (the copy constructor's label names the class).
struct CCreatureActionBase {
    virtual void v00();
};

struct CCreatureAction_ActivateQuest : CCreatureActionBase {
    unsigned char _body[0xb8 - 4];    // sizeof(CCreatureAction_ActivateQuest) = 0xb8
    CCreatureAction_ActivateQuest(const CCreatureAction_ActivateQuest& other);             // 0x0084C900
    CCreatureActionBase* Clone();
};

CCreatureActionBase* CCreatureAction_ActivateQuest::Clone()
{
    return new CCreatureAction_ActivateQuest(*this);
}