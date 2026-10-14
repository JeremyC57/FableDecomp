// CResourceMapOwner_00c364a0::CResourceMapOwner(owner, a, b, c, d, e, f) -- EH-framed constructor (the manifest's "PropertyContainerObject_Construct" is propagated; the matching destructor landed at 0x00C36590): five words stored from the arguments (+0, +4, +8, +0xc, +0x10), the 0x12B5180 entry container at +0x14 (root inline with the container pointer homed in the dead first argument slot, node @ 0x00C194F0; root and container destructors out of line @ 0x00C368B0 / 0x00C36B00, states 0-1), a (1.0f, d) float pair at +0x44 (@ 0x00C1BD20; d is a float, so retail re-reads its slot for +0x164 instead of keeping it in a register) and the +0x4c part from the owner's +4 (@ 0x00C45F30); the body stores c and d at +0x160 / +0x164, flags the owner (+0x4c), registers with the owner's registry (+0x10, slot 9) and the world's two registries (+0x98, +0x68) and initialises (@ 0x00C362F0)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C364A0, oracle length 236 bytes. Built under /EHsc.
// Retail bytes:  ... mov eax,[esp+14h] ; mov edx,[esp+1Ch] ; push esi ; mov esi,ecx ; mov ecx,[esp+1Ch] ; mov [esi],eax ; mov eax,[esp+2Ch] ; mov [esi+4],ecx ; mov ecx,[esp+30h] ; mov [esi+0Ch],eax ; lea eax,[esi+14h] ; mov [esi+10h],ecx ; lea ecx,[eax+8] ; mov [esi+8],edx ; mov [esp+4],esi ; mov [esp+18h],eax ; mov [eax+4],ecx ; push 0 ; mov dword ptr [esp+14h],0 ; mov dword ptr [eax],12B5180h ; call CNode::CNode ; mov edx,[esp+28h] ; push edx ; push 3F800000h ; lea ecx,[esi+44h] ; mov dword ptr [esp+18h],1 ; call CPair::CPair ; mov eax,[esi] ; mov ecx,[eax+4] ; push ecx ; lea ecx,[esi+4Ch] ; call CPart::CPart ; mov edx,[esp+24h] ; mov eax,[esp+28h] ; mov ecx,[esi] ; mov [esi+164h],eax ; mov [esi+160h],edx ; mov byte ptr [ecx+4Ch],1 ; mov eax,[esi] ; mov edx,[eax+10h] ; lea ecx,[eax+10h] ; push esi ; call [edx+24h] ; ... (+98h, +68h) ; mov ecx,esi ; call Init ; ... mov eax,esi ; ... ret 1Ch
// The classes are named by address, matching the landed destructor's model.
struct CResourceMapOwner_00c364a0;
struct CRegistry_00c364a0 {
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04();
    virtual void v05(); virtual void v06(); virtual void v07(); virtual void v08();
    virtual void Add(CResourceMapOwner_00c364a0* owner);   // slot 9
};

struct CWorld_00c364a0 {
    unsigned char _pad[0x68];
    CRegistry_00c364a0 m_first;            // +0x68
    unsigned char _pad2[0x2c];
    CRegistry_00c364a0 m_second;           // +0x98
};

struct COwner_00c364a0 {
    CWorld_00c364a0* m_pWorld;             // +0
    void* m_pContext;                      // +4
    unsigned char _pad[8];
    CRegistry_00c364a0 m_registry;         // +0x10
    unsigned char _pad2[0x38];
    bool m_dirty;                          // +0x4c
};

struct CNode_00c364a0 {                    // 0x28 bytes
    unsigned char _body[0x28];
    CNode_00c364a0(long mode);             // 0x00C194F0
    ~CNode_00c364a0();                     // 0x00C19250
};

struct CContainerRoot_00c364a0 {           // vtable 0x012B1AB0
    virtual void v00();
    CNode_00c364a0* m_pNode;               // +4
    CContainerRoot_00c364a0(CNode_00c364a0* node) : m_pNode(node) { }
    ~CContainerRoot_00c364a0();            // 0x00C368B0
};

struct CContainer_00c364a0 : CContainerRoot_00c364a0 {   // vtable 0x012B5180
    CNode_00c364a0 m_node;                 // +8
    CContainer_00c364a0() : CContainerRoot_00c364a0(&m_node), m_node(0) { }
    ~CContainer_00c364a0();                // 0x00C36B00
};

struct CPair_00c364a0 {                    // +0x44: a float and a word
    float m_scale;
    long m_value;
    CPair_00c364a0(float scale, float value);   // 0x00C1BD20
};

struct CPart_00c364a0 {                    // +0x4c
    unsigned char _body[0x114];
    CPart_00c364a0(void* context);         // 0x00C45F30
};

struct CResourceMapOwner_00c364a0 {
    COwner_00c364a0* m_pOwner;             // +0
    long m_4;
    long m_8;
    long m_c;
    long m_10;
    CContainer_00c364a0 m_entries;         // +0x14
    CPair_00c364a0 m_44;
    CPart_00c364a0 m_4c;
    long m_160;
    float m_164;
    void Init();                           // 0x00C362F0
    CResourceMapOwner_00c364a0(COwner_00c364a0* owner, long a, long b, long c, float d, long e, long f);
};

CResourceMapOwner_00c364a0::CResourceMapOwner_00c364a0(COwner_00c364a0* owner, long a, long b, long c, float d, long e, long f)
    : m_pOwner(owner), m_4(a), m_8(b), m_c(e), m_10(f), m_entries(), m_44(1.0f, d), m_4c(m_pOwner->m_pContext)
{
    m_160 = c;
    m_164 = d;
    m_pOwner->m_dirty = true;
    m_pOwner->m_registry.Add(this);
    m_pOwner->m_pWorld->m_second.Add(this);
    m_pOwner->m_pWorld->m_first.Add(this);
    Init();
}