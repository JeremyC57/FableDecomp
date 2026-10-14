// COpinionState_00c37c90::COpinionState(owner, a, b, c, d, bounds, flagA, flagB) -- EH-framed constructor (vtable 0x012B5370): the owner at +4, the +8 part built from (a, c) (@ 0x00C38330; destructor @ 0x00C38350, state 0), d at +0x10, the 16-byte bounds record copied to +0x14, the +0x24 byte cleared, the 0x12B5328 entry container at +0x28 (root inline, the container pointer homed in the dead owner slot, node @ 0x00C194F0 mode 0; root/container destructors @ 0x00C38060 / 0x00C382B0, states 1-2), b at +0x58 and the two flags packed into +0x5c (bit 0 from flagA, bit 1 from flagB, branchless); then the state registers with the owner's +4 registry, the world registry reached through owner->+0 ->+4 ->+0x38 ->+0x104 and the owner's +0x34 registry (slot 9 each) and runs its two initialisers (@ 0x00C37AD0 / 0x00C37C40)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C37C90, oracle length 263 bytes. Built under /EHsc.
// Retail bytes:  ... mov edx,[esp+18h] ; mov eax,[esp+14h] ; push esi ; mov esi,ecx ; mov ecx,[esp+24h] ; push ecx ; push edx ; lea ecx,[esi+8] ; mov [esp+0Ch],esi ; mov dword ptr [esi],12B5370h ; mov [esi+4],eax ; call CPart::CPart ; mov eax,[esp+28h] ; mov edx,[esp+2Ch] ; mov [esi+10h],eax ; mov eax,[edx] ; lea ecx,[esi+14h] ; mov [ecx],eax ; ... (four dword copies) ... lea eax,[esi+28h] ; mov byte ptr [esi+24h],0 ; mov dword ptr [esp+10h],0 ; mov [esp+18h],eax ; mov [eax+4],ecx ; push 0 ; mov byte ptr [esp+14h],1 ; mov dword ptr [eax],12B5328h ; call CNode::CNode ; mov dl,[esp+30h] ; mov eax,[esp+20h] ; xor ecx,ecx ; test dl,dl ; mov dl,[esp+34h] ; setne cl ; neg dl ; mov [esi+58h],eax ; mov eax,[esi+4] ; push esi ; mov byte ptr [esp+14h],2 ; sbb edx,edx ; and edx,2 ; or ecx,edx ; mov [esi+5Ch],ecx ; lea ecx,[eax+4] ; mov eax,[ecx] ; call [eax+24h] ; ... (two more registrations) ... mov ecx,esi ; call Init1 ; mov ecx,esi ; call Init2 ; ... mov eax,esi ; ... ret 20h
// The manifest label names the PDB class; the model classes are named by address.
struct COpinionState_00c37c90;
struct CRegistry_00c37c90 {
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04();
    virtual void v05(); virtual void v06(); virtual void v07(); virtual void v08();
    virtual void Add(COpinionState_00c37c90* state);   // slot 9
};

struct CWorldPart_00c37c90 {
    unsigned char _pad[0x104];
    CRegistry_00c37c90 m_registry;         // +0x104
};
struct CWorldLink_00c37c90 {
    unsigned char _pad[0x38];
    CWorldPart_00c37c90* m_pPart;          // +0x38
};
struct CWorld_00c37c90 {
    unsigned char _pad[4];
    CWorldLink_00c37c90* m_pLink;          // +4
};
struct COwner_00c37c90 {
    CWorld_00c37c90* m_pWorld;             // +0
    CRegistry_00c37c90 m_registry;         // +4
    unsigned char _pad[0x2c];
    CRegistry_00c37c90 m_registry2;        // +0x34
};

struct SBounds_00c37c90 { long m_0; long m_4; long m_8; long m_c; };   // 16-byte POD copied as a block

struct CPart_00c37c90 {                    // +8
    unsigned char _body[8];
    CPart_00c37c90(void* a, void* c);      // 0x00C38330
    ~CPart_00c37c90();                     // 0x00C38350
};

struct CNode_00c37c90 {                    // 0x28 bytes
    unsigned char _body[0x28];
    CNode_00c37c90(long mode);             // 0x00C194F0
    ~CNode_00c37c90();                     // 0x00C19250
};

struct CContainerRoot_00c37c90 {           // vtable 0x012B1AB0
    virtual void v00();
    CNode_00c37c90* m_pNode;               // +4
    CContainerRoot_00c37c90(CNode_00c37c90* node) : m_pNode(node) { }
    ~CContainerRoot_00c37c90();            // 0x00C38060
};

struct CContainer_00c37c90 : CContainerRoot_00c37c90 {   // vtable 0x012B5328
    CNode_00c37c90 m_node;                 // +8
    CContainer_00c37c90() : CContainerRoot_00c37c90(&m_node), m_node(0) { }
    ~CContainer_00c37c90();                // 0x00C382B0
};

struct COpinionState_00c37c90 {
    virtual void v00();
    COwner_00c37c90* m_pOwner;             // +4
    CPart_00c37c90 m_part;                 // +8
    void* m_10;
    SBounds_00c37c90 m_bounds;             // +0x14
    bool m_24;
    CContainer_00c37c90 m_entries;         // +0x28
    void* m_58;
    long m_flags;                          // +0x5c
    void Init1();                          // 0x00C37AD0
    void Init2();                          // 0x00C37C40
    COpinionState_00c37c90(COwner_00c37c90* owner, void* a, void* b, void* c, void* d, const SBounds_00c37c90* bounds, bool flagA, bool flagB);
};

COpinionState_00c37c90::COpinionState_00c37c90(COwner_00c37c90* owner, void* a, void* b, void* c, void* d, const SBounds_00c37c90* bounds, bool flagA, bool flagB)
    : m_pOwner(owner), m_part(a, c), m_10(d), m_bounds(*bounds), m_24(false), m_entries(), m_58(b),
      m_flags((flagA ? 1 : 0) | (flagB ? 2 : 0))
{
    m_pOwner->m_registry.Add(this);
    m_pOwner->m_pWorld->m_pLink->m_pPart->m_registry.Add(this);
    m_pOwner->m_registry2.Add(this);
    Init1();
    Init2();
}
