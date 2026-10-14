// CTCDPhysicalObstruction_00c2a8c0::CTCDPhysicalObstruction_00c2a8c0(void*, void*) -- EH-framed constructor (the class of the destructor @ 0x00C2A9B0): the first argument stored at +0 and the flag at +4 set; four inline container members built in order (+8 vtable 0x012B4460, +0x38 0x012B4268, +0x68 0x012B42B0, +0x98 0x012B42F8: root vtable and node pointer stored, the node at +8 built out of line with 0 @ 0x00C194F0, states 0/2/4/6 while a node builds with the container pointer parked in the dead first-argument slot); then +0xc8 zeroed, the bytes at +0xcc/+0xcd cleared, +0xce set and the second argument stored at +0xd0
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C2A8C0, oracle length 228 bytes. Built under /EHsc.
// Retail bytes:  ... mov eax,[esp+14h] ; push ebx ; push esi ; mov esi,ecx ; mov [esi],eax ; lea eax,[esi+8] ; lea ecx,[eax+8] ; mov byte ptr [esi+4],1 ; mov [esp+8],esi ; mov [esp+1Ch],eax ; mov [eax+4],ecx ; xor ebx,ebx ; push ebx ; mov [esp+18h],ebx ; mov dword ptr [eax],012B4460h ; call CNode::CNode ; lea eax,[esi+38h] ; lea ecx,[eax+8] ; mov dword ptr [esp+14h],1 ; mov [esp+1Ch],eax ; mov [eax+4],ecx ; push ebx ; mov byte ptr [esp+18h],2 ; mov dword ptr [eax],012B4268h ; call CNode::CNode ; ... (0x68 / 012B42B0h, state 4 ; 0x98 / 012B42F8h, state 6) ; mov ecx,[esp+20h] ; mov [esi+0C8h],ebx ; mov [esi+0CCh],bl ; mov [esi+0CDh],bl ; mov [esi+0D0h],ecx ; mov ecx,[esp+0Ch] ; mov byte ptr [esi+0CEh],1 ; mov eax,esi ; ... ret 8
// The root/node container pattern of the 0x012B1AB0 family: each root keeps the vptr and the
// node pointer, the derived container embeds the node. Named from the destructor's PDB label.
struct CNode_00c2a8c0 {                    // 0x28 bytes
    unsigned char _body[0x28];
    CNode_00c2a8c0(long mode);                 // 0x00C194F0
    ~CNode_00c2a8c0();                         // 0x00C19250
};

struct CRoot_00c2a8c0 {                    // vtable 0x012B1AB0
    virtual void v00();
    CNode_00c2a8c0* m_pNode;                   // +4
    CRoot_00c2a8c0(CNode_00c2a8c0* node) : m_pNode(node) { }
    ~CRoot_00c2a8c0() { }
};

struct CContainerA_00c2a8c0 : CRoot_00c2a8c0 { CNode_00c2a8c0 m_node; CContainerA_00c2a8c0() : CRoot_00c2a8c0(&m_node), m_node(0) { } ~CContainerA_00c2a8c0() { } };   // vtable 0x012B4460, +8
struct CContainerB_00c2a8c0 : CRoot_00c2a8c0 { CNode_00c2a8c0 m_node; CContainerB_00c2a8c0() : CRoot_00c2a8c0(&m_node), m_node(0) { } ~CContainerB_00c2a8c0() { } };   // vtable 0x012B4268, +0x38
struct CContainerC_00c2a8c0 : CRoot_00c2a8c0 { CNode_00c2a8c0 m_node; CContainerC_00c2a8c0() : CRoot_00c2a8c0(&m_node), m_node(0) { } ~CContainerC_00c2a8c0() { } };   // vtable 0x012B42B0, +0x68
struct CContainerD_00c2a8c0 : CRoot_00c2a8c0 { CNode_00c2a8c0 m_node; CContainerD_00c2a8c0() : CRoot_00c2a8c0(&m_node), m_node(0) { } ~CContainerD_00c2a8c0() { } };   // vtable 0x012B42F8, +0x98

struct CTCDPhysicalObstruction_00c2a8c0 {
    void* m_pOwner;                        // +0
    bool m_4;
    CContainerA_00c2a8c0 m_8;                  // +8
    CContainerB_00c2a8c0 m_38;                 // +0x38
    CContainerC_00c2a8c0 m_68;                 // +0x68
    CContainerD_00c2a8c0 m_98;                 // +0x98
    long m_c8;
    bool m_cc;
    bool m_cd;
    bool m_ce;
    void* m_pContext;                      // +0xd0
    CTCDPhysicalObstruction_00c2a8c0(void* owner, void* context);
};

CTCDPhysicalObstruction_00c2a8c0::CTCDPhysicalObstruction_00c2a8c0(void* owner, void* context)
    : m_pOwner(owner), m_4(true), m_8(), m_38(), m_68(), m_98(), m_c8(0), m_cc(false), m_cd(false), m_ce(true), m_pContext(context)
{
}
