// CSubPrimitiveList_00c3abf0::CSubPrimitiveList_00c3abf0() -- EH-framed constructor: the root keeps a pointer to the node member at +8, the container stores its vtable (0x012B5578) and builds the node out of line with 0 (@ 0x00C194F0)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C3ABF0, oracle length 74 bytes. Built under /EHsc.
// Retail bytes:  push -1 ; push offset __ehhandler ; mov eax,fs:[0] ; push eax ; mov fs:[0],esp ; push ecx ; push esi ; mov esi,ecx ; lea ecx,[esi+8] ; mov [esp+4],esi ; mov [esi+4],ecx ; push 0 ; mov dword ptr [esp+14h],0 ; mov dword ptr [esi],offset vtable ; call CNode::CNode ; mov ecx,[esp+8] ; mov eax,esi ; pop esi ; mov fs:[0],ecx ; add esp,10h ; ret
// The same two-level layout as the typed containers of `2026-10-14_linux_i1130`: the polymorphic
// root takes the address of the node member (+4, stored before EH state 0 because it is the root's
// own member; the root's vtable store is dead and dropped), then the container stores its vtable
// and builds the node out of line. The manifest label is propagated; the classes are named by address.
struct CNode_00c3abf0 {
    unsigned char _body[0x10];
    CNode_00c3abf0(long kind);                  // 0x00C194F0
    ~CNode_00c3abf0();
};

struct CRoot_00c3abf0 {                          // the root owns the vptr and the node pointer
    virtual void v00();
    CNode_00c3abf0* m_pNode;                    // +4
    CRoot_00c3abf0(CNode_00c3abf0* node) : m_pNode(node) { }
    ~CRoot_00c3abf0() { }
};

struct CSubPrimitiveList_00c3abf0 : CRoot_00c3abf0 {                 // vtable 0x012B5578
    CNode_00c3abf0 m_node;                      // +8
    CSubPrimitiveList_00c3abf0();
};

CSubPrimitiveList_00c3abf0::CSubPrimitiveList_00c3abf0()
    : CRoot_00c3abf0(&m_node), m_node(0)
{
}