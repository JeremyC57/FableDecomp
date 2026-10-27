// CActivationReceptor_00c38b20::Final() -- pass a scratch message word through the mailbox queue at +0xc (DispatchMessage 0x00C392F0)
// Leaf: ?Final@CActivationReceptor_00c38b20@@QAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C38B10, 15 bytes, thiscall, plain `ret`.
// Identity: called by the landed receptor destructor 0x00C38B20 (state 5, bound ?Final@CActivationReceptor_00c38b20@@QAEXXZ)
//   and by 0x00C390EC; the sibling 0x00C38AF0 (bound ?Stop@...) does the same on the queue at +0xd8. The catalog label
//   "CComponent::GetCurrentState" is propagated. Descriptive (libpk, no PDB/RTTI).
// Callee: CMailboxQueue_00c392f0::DispatchMessage_CMailboxQueue_00c392f0(void*) 0x00C392F0 (the landed symbol).
// Shape: the message is the `push ecx` frame word; its address is the only argument.
struct CMailboxQueue_00c392f0 {
    unsigned char _body[0x20];
    void* DispatchMessage_CMailboxQueue_00c392f0(void* message);   // 0x00C392F0
};

struct CActivationReceptor_00c38b20 {
    unsigned char _0[0xc];
    CMailboxQueue_00c392f0 m_queue;          // +0x0c
    void Final();
};

void CActivationReceptor_00c38b20::Final()
{
    long message;
    m_queue.DispatchMessage_CMailboxQueue_00c392f0(&message);
}