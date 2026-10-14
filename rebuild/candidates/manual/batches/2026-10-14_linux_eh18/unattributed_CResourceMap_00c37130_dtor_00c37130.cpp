// CResourceMap_00c37130::~CResourceMap_00c37130() -- EH-framed destructor: the owned resource at +0xc deleted when present (out-of-line destructor @ 0x00C463A0, then the global operator delete @ 0x00BFE9BC) and nulled, the registry at +8 of the owner at +0 told to drop `this` (virtual slot 8), then the container member at +0x10 destroyed inline (its vtable 0x012B51F0, node at +8 @ 0x00C19250 at state 1, root vtable 0x012B1AB0)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C37130, oracle length 131 bytes. Built under /EHsc.
// Retail bytes:  push -1 ; push offset __ehhandler ; mov eax,fs:[0] ; push eax ; mov fs:[0],esp ; push ecx ; push esi ; mov esi,ecx ; push edi ; mov [esp+8],esi ; mov edi,[esi+0Ch] ; test edi,edi ; mov dword ptr [esp+14h],0 ; je skip ; mov ecx,edi ; call ~CResource ; push edi ; call operator delete ; add esp,4 ; mov dword ptr [esi+0Ch],0 ; skip: mov eax,[esi] ; lea ecx,[eax+8] ; mov eax,[ecx] ; push esi ; call [eax+20h] ; add esi,10h ; mov [esp+8],esi ; mov dword ptr [esi],offset vtable ; lea ecx,[esi+8] ; mov dword ptr [esp+14h],1 ; call ~CNode ; mov ecx,[esp+0Ch] ; mov dword ptr [esi],offset root ; pop edi ; pop esi ; mov fs:[0],ecx ; add esp,10h ; ret
// `delete p` on a class with a non-virtual out-of-line destructor is the destructor call followed
// by operator delete. No vtable store of its own (not polymorphic). The manifest label is
// propagated; the classes are named by address.
struct CResourceMap_00c37130;
struct CRegistry_00c37130 {
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04();
    virtual void v05(); virtual void v06(); virtual void v07();
    virtual void Remove(CResourceMap_00c37130* map);   // slot 8
};

struct COwner_00c37130 {
    unsigned char _pad[8];
    CRegistry_00c37130 m_registry;         // +8
};

struct CResource_00c37130 {
    ~CResource_00c37130();                 // 0x00C463A0
};

struct CNode_00c37130 {
    unsigned char _body[0x28];
    ~CNode_00c37130();                     // 0x00C19250
};

struct CRoot_00c37130 {                    // vtable 0x012B1AB0: the root owns the vptr and the node pointer
    virtual void v00();
    CNode_00c37130* m_pNode;               // +4
    ~CRoot_00c37130() { }
};

struct CContainer_00c37130 : CRoot_00c37130 {   // vtable 0x012B51F0
    CNode_00c37130 m_node;                 // +8
    ~CContainer_00c37130() { }
};

struct CResourceMap_00c37130 {
    COwner_00c37130* m_pOwner;             // +0
    long m_4;
    long m_8;
    CResource_00c37130* m_pResource;       // +0xc
    CContainer_00c37130 m_entries;         // +0x10
    ~CResourceMap_00c37130();
};

CResourceMap_00c37130::~CResourceMap_00c37130()
{
    if (m_pResource) {
        delete m_pResource;
        m_pResource = 0;
    }
    m_pOwner->m_registry.Remove(this);
}
