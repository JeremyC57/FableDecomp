// CCombatAbilityStrafeDef_00c0a6d0::CCombatAbilityStrafeDef_00c0a6d0() -- EH-framed constructor: two container members constructed inline, at +0 (vtable 0x012B1C40) and +0x10 (vtable 0x012B1B78); each root keeps a pointer to its 8-byte node at +8, built out of line with 1 (@ 0x00C185B0); states 0..2 as the parts come alive
// Leaf: ??0CCombatAbilityStrafeDef@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C0A6D0, oracle length 115 bytes. Built under /EHsc.
// Retail bytes:  push -1 ; push offset __ehhandler ; mov eax,fs:[0] ; push eax ; mov fs:[0],esp ; sub esp,8 ; push esi ; mov esi,ecx ; lea ecx,[esi+8] ; mov [esp+4],esi ; mov [esi+4],ecx ; push 1 ; mov dword ptr [esp+18h],0 ; mov dword ptr [esi],offset vtable ; call CNode::CNode ; lea eax,[esi+10h] ; lea ecx,[eax+8] ; mov dword ptr [esp+14h],1 ; mov [esp+8],eax ; mov [eax+4],ecx ; push 1 ; mov byte ptr [esp+18h],2 ; mov dword ptr [eax],offset vtable2 ; call CNode::CNode ; mov ecx,[esp+0Ch] ; mov eax,esi ; pop esi ; mov fs:[0],ecx ; add esp,14h ; ret
// The same root/container shape as the landed CTypedContainer constructors (0x00C306D0 ...): the
// root's node pointer is stored before EH state 0 because it is the root's own member, the
// container stores its vtable and builds the node; here the outer container also owns a second
// container as a member, whose inlined constructor parks its `this` in the second frame slot for
// the unwind funclet (state 2 while its node is under construction). The manifest label is
// propagated; the classes are named by address.
struct CNode_00c0a6d0 {                    // 8 bytes
    unsigned char _body[8];
    CNode_00c0a6d0(long kind);             // 0x00C185B0
    ~CNode_00c0a6d0();
};

struct CRoot_00c0a6d0 {                    // the root owns the vptr and the node pointer
    virtual void v00();
    CNode_00c0a6d0* m_pNode;               // +4
    CRoot_00c0a6d0(CNode_00c0a6d0* node) : m_pNode(node) { }
    ~CRoot_00c0a6d0() { }
};

struct CInnerContainer_00c0a6d0 : CRoot_00c0a6d0 {   // vtable 0x012B1B78
    CNode_00c0a6d0 m_node;                 // +8
    CInnerContainer_00c0a6d0() : CRoot_00c0a6d0(&m_node), m_node(1) { }
    ~CInnerContainer_00c0a6d0() { }
};

struct COuterContainer_00c0a6d0 : CRoot_00c0a6d0 {   // vtable 0x012B1C40
    CNode_00c0a6d0 m_node;                 // +8
    COuterContainer_00c0a6d0() : CRoot_00c0a6d0(&m_node), m_node(1) { }
    ~COuterContainer_00c0a6d0() { }
};

struct CCombatAbilityStrafeDef_00c0a6d0 {  // two container members, not polymorphic itself
    COuterContainer_00c0a6d0 m_first;      // +0
    CInnerContainer_00c0a6d0 m_second;     // +0x10
    CCombatAbilityStrafeDef_00c0a6d0();
};

CCombatAbilityStrafeDef_00c0a6d0::CCombatAbilityStrafeDef_00c0a6d0()
{
}