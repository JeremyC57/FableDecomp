// CAIStateGroup_BirdFlyToFood_00c48300::Issue(SActionRequest_00c48300* request) -- forward an action request to the mailbox queue member at +0x18
// Leaf: ?Issue@CAIStateGroup_BirdFlyToFood_00c48300@@QAEXPAUSActionRequest_00c48300@@@Z
// Flags: none
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C47F00, 8 bytes, thiscall: `add ecx,18h ; jmp 0x00C48960` (the target returns `ret 4`).
// Identity (descriptive; libpk has no PDB or RTTI): the catalog's `_Cons_val<...CParticleMorphs::CEntry...>` label is
//   propagated. Five landed callers (0x00C48300, 0x00C483A0, 0x00C48430, 0x00C484B0, 0x00C48560) build a 0x40-byte request
//   and call this as `Issue(request)`; this keeps the first landed spelling (reloc_bindings binds
//   `?Issue@CAIStateGroup_BirdFlyToFood_00c48300@@...` to 0x00C47F00). The member at +0x18 is the landed
//   CMailboxQueue_00c48960, whose 0x00C48960 posts the message under its lock.
// Callees: CMailboxQueue_00c48960::ProcessMessage_CMailboxQueue_00c48960(void*) 0x00C48960 (landed spelling).
struct SActionRequest_00c48300;

struct CMailboxQueue_00c48960 {
    long ProcessMessage_CMailboxQueue_00c48960(void* message);   // 0x00C48960
};

struct CAIStateGroup_BirdFlyToFood_00c48300 {
    unsigned char _pad[0x18];
    CMailboxQueue_00c48960 m_queue;             // +0x18
    void Issue(SActionRequest_00c48300* request);
};

void CAIStateGroup_BirdFlyToFood_00c48300::Issue(SActionRequest_00c48300* request)
{
    m_queue.ProcessMessage_CMailboxQueue_00c48960(request);
}