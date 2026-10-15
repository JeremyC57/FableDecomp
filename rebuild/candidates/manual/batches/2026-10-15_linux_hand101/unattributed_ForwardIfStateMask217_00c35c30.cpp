// _unattributed::ForwardIfStateMask217(CStateObject* object, long first, long second) -- forward two values to the object's handler when any of its state bits 0x217 is set
// Leaf: ForwardIfStateMask217
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C35C30, length 31 bytes (`ret 0Ch`; callers 0x00C27A3C, 0x00C496A2).
// Callees: the object's handler @ 0x00C43980 (manifest "CCombatRenderState_OnActionComplete", unverified: it forwards
// both values to a virtual slot 11 of the member at +0x50 and then switches on the low 7 bits of the word at +0x10).
// Label correction: the chunk calls this CTCSpecialAbilities::OnKill; it is a free `__stdcall` helper of the 0x00C3xxxx
// module (no `this`, three stack arguments, the object loaded into ecx), the catalogue's 0x00C3 null-object-call
// family shape (GOTCHAS wave 24c), so it is attributed descriptively.
// Shape: `ret 0Ch` with no `this` = `__stdcall`; the two values are re-pushed in right-to-left order.
struct CStateObject {
    unsigned char _pad[0x10];
    unsigned long m_state;                                  // +0x10
    void Handle(long first, long second);                   // 0x00C43980
};

void __stdcall ForwardIfStateMask217(CStateObject* object, long first, long second)
{
    if (object->m_state & 0x217)
        object->Handle(first, second);
}
