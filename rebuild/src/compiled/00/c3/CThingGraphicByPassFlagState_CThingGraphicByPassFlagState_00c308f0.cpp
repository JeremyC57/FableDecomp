// CThingGraphicByPassFlagState::CThingGraphicByPassFlagState() -- EH-framed constructor: the root keeps a pointer to the node member at +8, own vtable (0x012B4CC0), the node built out of line with 0 (@ 0x00C194F0), then the flag byte at +0x30 set
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C308F0, oracle length 78 bytes. Built under /EHsc.
// Retail bytes:  push -1 ; push offset __ehhandler ; mov eax,fs:[0] ; push eax ; mov fs:[0],esp ; push ecx ; push esi ; mov esi,ecx ; lea ecx,[esi+8] ; mov [esp+4],esi ; mov [esi+4],ecx ; push 0 ; mov dword ptr [esp+14h],0 ; mov dword ptr [esi],offset vtable ; call CNode::CNode ; mov ecx,[esp+8] ; mov byte ptr [esi+30h],1 ; mov eax,esi ; pop esi ; mov fs:[0],ecx ; add esp,10h ; ret
// The two-level container layout of `2026-10-14_linux_i1130` (the root owns the vptr and the node
// pointer, the node is built out of line) followed by one more initialiser: the byte flag at
// +0x30, stored after the node constructor returns. The class comes from the manifest label.
struct CNode_00c308f0 {
    unsigned char _body[0x10];
    CNode_00c308f0(long kind);             // 0x00C194F0
    ~CNode_00c308f0();
};

struct CRoot_00c308f0 {                      // the root owns the vptr and the node pointer
    virtual void v00();
    CNode_00c308f0* m_pNode;               // +4
    CRoot_00c308f0(CNode_00c308f0* node) : m_pNode(node) { }
    ~CRoot_00c308f0() { }
};

struct CThingGraphicByPassFlagState : CRoot_00c308f0 {   // vtable 0x012B4CC0
    CNode_00c308f0 m_node;                 // +8
    unsigned char _pad[0x18];
    bool m_bEnabled;                       // +0x30
    CThingGraphicByPassFlagState();
};

CThingGraphicByPassFlagState::CThingGraphicByPassFlagState()
    : CRoot_00c308f0(&m_node), m_node(0), m_bEnabled(true)
{
}