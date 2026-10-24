// CTCDiceGame_00c2bd90::CTCDiceGame_00c2bd90(void*) -- EH-framed constructor: the argument stored at +0, then the two container members at +4 (vtable 0x012B45E8) and +0x34 (vtable 0x012B4510) constructed inline: each root keeps a pointer to its node at +8, the node built out of line with 0 (@ 0x00C194F0); states 0..2 as the parts come alive
// Leaf: ??0CTCDiceGame@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C2BD90, oracle length 128 bytes. Built under /EHsc.
// Retail bytes:  push -1 ; push offset __ehhandler ; mov eax,fs:[0] ; push eax ; mov fs:[0],esp ; push ecx ; mov eax,[arg] ; push esi ; mov esi,ecx ; mov [esi],eax ; lea eax,[esi+4] ; lea ecx,[eax+8] ; mov [esp+4],esi ; mov [esp+18h],eax ; mov [eax+4],ecx ; push 0 ; mov dword ptr [esp+14h],0 ; mov dword ptr [eax],offset vtableA ; call CNode::CNode ; lea eax,[esi+34h] ; lea ecx,[eax+8] ; mov dword ptr [esp+10h],1 ; mov [esp+18h],eax ; mov [eax+4],ecx ; push 0 ; mov byte ptr [esp+14h],2 ; mov dword ptr [eax],offset vtableB ; call CNode::CNode ; mov ecx,[esp+8] ; mov eax,esi ; pop esi ; mov fs:[0],ecx ; add esp,10h ; ret 4
// The constructor counterpart of the destructor landed at 0x00C2BE10 (same two containers).
// Each inlined member constructor parks its `this` in the argument-side frame slot for the
// unwind funclet. The manifest label is propagated; the class is named by address.
struct CNode_00c2bd90 {                    // +8 of each container, 0x28 bytes
    unsigned char _body[0x28];
    CNode_00c2bd90(long kind);             // 0x00C194F0
    ~CNode_00c2bd90();
};

struct CRoot_00c2bd90 {                    // the root owns the vptr and the node pointer
    virtual void v00();
    CNode_00c2bd90* m_pNode;               // +4
    CRoot_00c2bd90(CNode_00c2bd90* node) : m_pNode(node) { }
    ~CRoot_00c2bd90() { }
};

struct CContainerA_00c2bd90 : CRoot_00c2bd90 {   // vtable 0x012B45E8
    CNode_00c2bd90 m_node;                 // +8
    CContainerA_00c2bd90() : CRoot_00c2bd90(&m_node), m_node(0) { }
    ~CContainerA_00c2bd90() { }
};

struct CContainerB_00c2bd90 : CRoot_00c2bd90 {   // vtable 0x012B4510
    CNode_00c2bd90 m_node;                 // +8
    CContainerB_00c2bd90() : CRoot_00c2bd90(&m_node), m_node(0) { }
    ~CContainerB_00c2bd90() { }
};

struct CTCDiceGame_00c2bd90 {
    void* m_pOwner;                        // +0
    CContainerA_00c2bd90 m_first;          // +4
    CContainerB_00c2bd90 m_second;         // +0x34
    CTCDiceGame_00c2bd90(void* owner);
};

CTCDiceGame_00c2bd90::CTCDiceGame_00c2bd90(void* owner)
    : m_pOwner(owner)
{
}