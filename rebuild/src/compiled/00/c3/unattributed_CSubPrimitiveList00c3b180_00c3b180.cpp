// CSubPrimitiveList_00c3b180::CSubPrimitiveList_00c3b180() -- EH-framed constructor: the root keeps a pointer to the node member at +8, the container stores its vtable (0x012B5650) and builds the node out of line with 0 (@ 0x00C194F0)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C3B180, oracle length 74 bytes. Built under /EHsc.
// Retail bytes:  push -1 ; push offset __ehhandler ; mov eax,fs:[0] ; push eax ; mov fs:[0],esp ; push ecx ; push esi ; mov esi,ecx ; lea ecx,[esi+8] ; mov [esp+4],esi ; mov [esi+4],ecx ; push 0 ; mov dword ptr [esp+14h],0 ; mov dword ptr [esi],offset vtable ; call CNode::CNode ; mov ecx,[esp+8] ; mov eax,esi ; pop esi ; mov fs:[0],ecx ; add esp,10h ; ret
// The same two-level layout as the typed containers of `2026-10-14_linux_i1130`: the polymorphic
// root takes the address of the node member (+4, stored before EH state 0 because it is the root's
// own member; the root's vtable store is dead and dropped), then the container stores its vtable
// and builds the node out of line. The manifest label is propagated; the classes are named by address.
struct CNode_00c3b180 {
    unsigned char _body[0x10];
    CNode_00c3b180(long kind);                  // 0x00C194F0
    ~CNode_00c3b180();
};

struct CRoot_00c3b180 {                          // the root owns the vptr and the node pointer
    virtual void v00();
    CNode_00c3b180* m_pNode;                    // +4
    CRoot_00c3b180(CNode_00c3b180* node) : m_pNode(node) { }
    ~CRoot_00c3b180() { }
};

struct CSubPrimitiveList_00c3b180 : CRoot_00c3b180 {                 // vtable 0x012B5650
    CNode_00c3b180 m_node;                      // +8
    CSubPrimitiveList_00c3b180();
};

CSubPrimitiveList_00c3b180::CSubPrimitiveList_00c3b180()
    : CRoot_00c3b180(&m_node), m_node(0)
{
}