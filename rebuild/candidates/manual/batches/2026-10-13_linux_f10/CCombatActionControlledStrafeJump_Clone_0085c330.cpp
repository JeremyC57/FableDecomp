// CCombatAction_ControlledStrafeJump::Clone() -- new CCombatAction_ControlledStrafeJump(*this)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0085C330, oracle length 34 bytes.
// Grade: landed -- byte-exact (RELOCATION_MATCH) and behaviour PASS under the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); generated for a repeated body shape, see the batch README.
//
// Retail bytes:  push esi ; push 0x130 ; mov esi,ecx ; call operator new ; add esp,4 ; test eax,eax ; je N ; push esi ; mov ecx,eax ; call CCombatAction_ControlledStrafeJump::CCombatAction_ControlledStrafeJump(const CCombatAction_ControlledStrafeJump&) ; pop esi ; ret ; N: xor eax,eax ; pop esi ; ret
// Virtual clone: allocates sizeof(CCombatAction_ControlledStrafeJump) = 0x130, null-checks and copy-constructs from
// `*this` through the out-of-line copy constructor @ 0x0085C360; the constructor's `this` return
// is the result. One of 104 bodies of this shape (the copy constructor's label names the class).
struct CCreatureActionBase {
    virtual void v00();
};

struct CCombatAction_ControlledStrafeJump : CCreatureActionBase {
    unsigned char _body[0x130 - 4];    // sizeof(CCombatAction_ControlledStrafeJump) = 0x130
    CCombatAction_ControlledStrafeJump(const CCombatAction_ControlledStrafeJump& other);             // 0x0085C360
    CCreatureActionBase* Clone();
};

CCreatureActionBase* CCombatAction_ControlledStrafeJump::Clone()
{
    return new CCombatAction_ControlledStrafeJump(*this);
}
