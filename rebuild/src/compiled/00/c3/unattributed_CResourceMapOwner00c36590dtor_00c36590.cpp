// CResourceMapOwner_00c36590::~CResourceMapOwner_00c36590() -- EH-framed destructor: every entry the container member at +0x14 still yields (virtual slot 11) has its resource at +4 released (@ 0x00C36290), the owner's cleared byte at +0x4c is set, `this` is dropped from the registries at +0x68 and +0x98 of the owner's world and from the owner's registry at +0x10 (virtual slot 8 each), then the container is destroyed inline (vtable 0x012B5180, node at +8 @ 0x00C19250 at state 1, root vtable 0x012B1AB0)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C36590, oracle length 177 bytes. Built under /EHsc.
// Retail bytes:  ... mov [esp+8],esi ; mov eax,[esi+14h] ; lea edi,[esi+14h] ; mov ecx,edi ; mov dword ptr [esp+14h],0 ; call [eax+2Ch] ; test eax,eax ; je done ; loop: mov ecx,[eax+4] ; push ecx ; mov ecx,esi ; call Release ; mov edx,[edi] ; mov ecx,edi ; call [edx+2Ch] ; test eax,eax ; jne loop ; done: mov eax,[esi] ; mov byte ptr [eax+4Ch],1 ; mov ecx,[esi] ; mov eax,[ecx] ; mov edx,[eax+68h] ; lea ecx,[eax+68h] ; push esi ; call [edx+20h] ; mov eax,[esi] ; mov eax,[eax] ; mov edx,[eax+98h] ; lea ecx,[eax+98h] ; push esi ; call [edx+20h] ; mov eax,[esi] ; lea ecx,[eax+10h] ; mov eax,[ecx] ; push esi ; call [eax+20h] ; mov esi,edi ; mov [esp+8],esi ; mov dword ptr [esi],offset vtable ; lea ecx,[esi+8] ; mov dword ptr [esp+14h],1 ; call ~CNode ; mov ecx,[esp+0Ch] ; mov dword ptr [esi],offset root ; ...
// Not polymorphic itself (no own vtable store). The manifest label is propagated; the classes
// are named by address.
struct CResourceMapOwner_00c36590;
struct CRegistry_00c36590 {
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04();
    virtual void v05(); virtual void v06(); virtual void v07();
    virtual void Remove(CResourceMapOwner_00c36590* owner);   // slot 8
};

struct CWorld_00c36590 {
    unsigned char _pad[0x68];
    CRegistry_00c36590 m_first;            // +0x68
    unsigned char _pad2[0x2c];
    CRegistry_00c36590 m_second;           // +0x98
};

struct COwner_00c36590 {
    CWorld_00c36590* m_pWorld;             // +0
    unsigned char _pad[0xc];
    CRegistry_00c36590 m_registry;         // +0x10
    unsigned char _pad2[0x38];
    bool m_cleared;                        // +0x4c
};

struct CEntry_00c36590 {
    long m_0;
    void* m_pResource;                     // +4
};

struct CNode_00c36590 {
    unsigned char _body[0x28];
    ~CNode_00c36590();                     // 0x00C19250
};

struct CContainerRoot_00c36590 {           // vtable 0x012B1AB0
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04();
    virtual void v05(); virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09();
    virtual void v10();
    virtual CEntry_00c36590* Next();       // slot 11
    CNode_00c36590* m_pNode;               // +4
    ~CContainerRoot_00c36590() { }
};

struct CContainer_00c36590 : CContainerRoot_00c36590 {   // vtable 0x012B5180
    CNode_00c36590 m_node;                 // +8
    ~CContainer_00c36590() { }
};

struct CResourceMapOwner_00c36590 {
    COwner_00c36590* m_pOwner;             // +0
    long m_4;
    long m_8;
    long m_c;
    long m_10;
    CContainer_00c36590 m_entries;         // +0x14
    void Release(void* resource);          // 0x00C36290
    ~CResourceMapOwner_00c36590();
};

CResourceMapOwner_00c36590::~CResourceMapOwner_00c36590()
{
    CEntry_00c36590* entry = m_entries.Next();
    while (entry) {
        Release(entry->m_pResource);
        entry = m_entries.Next();
    }
    m_pOwner->m_cleared = true;
    m_pOwner->m_pWorld->m_first.Remove(this);
    m_pOwner->m_pWorld->m_second.Remove(this);
    m_pOwner->m_registry.Remove(this);
}