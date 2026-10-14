// CSubPrimitiveList::CSubPrimitiveList(void*, void*, void*) -- EH-framed constructor: the three arguments stored at +0, +4 and +8, two 0x30-byte node members built out of line (+0xc @ 0x00C3ABF0, then +0x3c @ 0x00C3B180 at state 0), then two dwords at +0x6c/+0x70 and seven flag bytes at +0x74..+0x7a zeroed through ebx
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C2E0C0, oracle length 120 bytes. Built under /EHsc.
// Retail bytes:  push -1 ; push offset __ehhandler ; mov eax,fs:[0] ; push eax ; mov fs:[0],esp ; push ecx ; mov eax,[a] ; mov edx,[c] ; push ebx ; push esi ; mov esi,ecx ; mov ecx,[b] ; mov [esi+4],ecx ; lea ecx,[esi+0Ch] ; mov [esp+8],esi ; mov [esi],eax ; mov [esi+8],edx ; call CNodeA::CNodeA ; xor ebx,ebx ; lea ecx,[esi+3Ch] ; mov [esp+14h],ebx ; call CNodeB::CNodeB ; mov ecx,[esp+0Ch] ; mov [esi+6Ch],ebx ; mov [esi+70h],ebx ; mov [esi+74h],bl ; ... ; mov [esi+7Ah],bl ; mov eax,esi ; pop esi ; pop ebx ; mov fs:[0],ecx ; add esp,10h ; ret 0Ch
// No vtable store: a plain record. The zero register is shared by the EH state store and the
// trailing initialisers, which is why they follow the second constructor.
struct CNodeA_00c2e0c0 {                   // 0x30 bytes
    unsigned char _body[0x30];
    CNodeA_00c2e0c0();                     // 0x00C3ABF0
    ~CNodeA_00c2e0c0();
};

struct CNodeB_00c2e0c0 {                   // 0x30 bytes
    unsigned char _body[0x30];
    CNodeB_00c2e0c0();                     // 0x00C3B180
    ~CNodeB_00c2e0c0();
};

struct CSubPrimitiveList {
    void* m_pOwner;                        // +0
    void* m_pMesh;                         // +4
    void* m_pMaterial;                     // +8
    CNodeA_00c2e0c0 m_nodeA;               // +0xc
    CNodeB_00c2e0c0 m_nodeB;               // +0x3c
    long m_count;                          // +0x6c
    long m_capacity;                       // +0x70
    bool m_flag74, m_flag75, m_flag76, m_flag77, m_flag78, m_flag79, m_flag7a;
    CSubPrimitiveList(void* owner, void* mesh, void* material);
};

CSubPrimitiveList::CSubPrimitiveList(void* owner, void* mesh, void* material)
    : m_pOwner(owner), m_pMesh(mesh), m_pMaterial(material)
{
    m_count = 0;
    m_capacity = 0;
    m_flag74 = false;
    m_flag75 = false;
    m_flag76 = false;
    m_flag77 = false;
    m_flag78 = false;
    m_flag79 = false;
    m_flag7a = false;
}