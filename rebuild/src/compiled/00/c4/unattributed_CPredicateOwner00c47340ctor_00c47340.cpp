// CPredicateOwner_00c47340::CPredicateOwner_00c47340(COwner*, void*) -- EH-framed constructor: the owner and data stored at +0/+4, the two container members at +8 (vtable 0x012B7F88) and +0x38 (vtable 0x012B7FD0) constructed inline (root node pointer, node built with 0 @ 0x00C194F0; states 0..2), `this` added to the owner's registry at +0x28 (virtual slot 9, state 3), then the input streams opened (@ 0x00C47050) and the solid-to-thing filters created (@ 0x00C470E0)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C47340, oracle length 167 bytes. Built under /EHsc.
// Retail bytes:  push -1 ; push offset __ehhandler ; mov eax,fs:[0] ; push eax ; mov fs:[0],esp ; push ecx ; mov eax,[owner] ; push esi ; mov esi,ecx ; mov ecx,[data] ; mov [esi],eax ; lea eax,[esi+8] ; mov [esi+4],ecx ; lea ecx,[eax+8] ; mov [esp+4],esi ; mov [esp+18h],eax ; mov [eax+4],ecx ; push 0 ; mov dword ptr [esp+14h],0 ; mov dword ptr [eax],offset vtableA ; call CNode::CNode ; lea eax,[esi+38h] ; lea ecx,[eax+8] ; mov dword ptr [esp+10h],1 ; mov [esp+18h],eax ; mov [eax+4],ecx ; push 0 ; mov byte ptr [esp+14h],2 ; mov dword ptr [eax],offset vtableB ; call CNode::CNode ; mov eax,[esi] ; mov edx,[eax+28h] ; lea ecx,[eax+28h] ; push esi ; mov byte ptr [esp+14h],3 ; call [edx+24h] ; mov ecx,esi ; call OpenInputStreams ; mov ecx,esi ; call CreateSolidToThingFilters ; mov ecx,[esp+8] ; mov eax,esi ; pop esi ; mov fs:[0],ecx ; add esp,10h ; ret 8
// The constructor counterpart of the destructor landed at 0x00C473F0. The manifest label is
// propagated; the classes are named by address.
struct CPredicateOwner_00c47340;
struct CRegistry_00c47340 {
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04();
    virtual void v05(); virtual void v06(); virtual void v07(); virtual void v08();
    virtual void Add(CPredicateOwner_00c47340* owner);   // slot 9
};

struct COwner_00c47340 {
    unsigned char _pad[0x28];
    CRegistry_00c47340 m_registry;         // +0x28
};

struct CNode_00c47340 {
    unsigned char _body[0x28];
    CNode_00c47340(long kind);             // 0x00C194F0
    ~CNode_00c47340();
};

struct CRoot_00c47340 {                    // the root owns the vptr and the node pointer
    virtual void v00();
    CNode_00c47340* m_pNode;               // +4
    CRoot_00c47340(CNode_00c47340* node) : m_pNode(node) { }
    ~CRoot_00c47340() { }
};

struct CContainerA_00c47340 : CRoot_00c47340 {   // vtable 0x012B7F88
    CNode_00c47340 m_node;                 // +8
    CContainerA_00c47340() : CRoot_00c47340(&m_node), m_node(0) { }
    ~CContainerA_00c47340() { }
};

struct CContainerB_00c47340 : CRoot_00c47340 {   // vtable 0x012B7FD0
    CNode_00c47340 m_node;                 // +8
    CContainerB_00c47340() : CRoot_00c47340(&m_node), m_node(0) { }
    ~CContainerB_00c47340() { }
};

struct CPredicateOwner_00c47340 {
    COwner_00c47340* m_pOwner;             // +0
    void* m_pData;                         // +4
    CContainerA_00c47340 m_first;          // +8
    CContainerB_00c47340 m_second;         // +0x38
    void OpenInputStreams();               // 0x00C47050
    void CreateSolidToThingFilters();      // 0x00C470E0
    CPredicateOwner_00c47340(COwner_00c47340* owner, void* data);
};

CPredicateOwner_00c47340::CPredicateOwner_00c47340(COwner_00c47340* owner, void* data)
    : m_pOwner(owner), m_pData(data)
{
    m_pOwner->m_registry.Add(this);
    OpenInputStreams();
    CreateSolidToThingFilters();
}