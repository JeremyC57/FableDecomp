// CGameFilters_00c44f80::~CGameFilters_00c44f80() -- EH-framed destructor: two out-of-line cleanups on `this` (@ 0x00C44B80, @ 0x00C44D30, body at state 3), the registry at +0x14 of the owner at +0 told to drop `this` (virtual slot 8), the member at +0x80 destroyed out of line (@ 0x00C61BD0, state 2), the two container members at +0x50 (vtable 0x012B7CE8) and +0x20 (vtable 0x012B7CA0) destroyed inline (node at +8 @ 0x00C19250 at states 4 and 5, root vtable 0x012B1AB0 after each), then the member at +8 destroyed out of line (@ 0x00C22A00)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C44F80, oracle length 168 bytes. Built under /EHsc.
// Retail bytes:  push -1 ; push offset __ehhandler ; mov eax,fs:[0] ; push eax ; mov fs:[0],esp ; sub esp,8 ; push esi ; mov esi,ecx ; push edi ; mov [esp+8],esi ; mov dword ptr [esp+18h],3 ; call CleanupA ; mov ecx,esi ; call CleanupB ; mov eax,[esi] ; lea ecx,[eax+14h] ; mov eax,[ecx] ; push esi ; call [eax+20h] ; lea ecx,[esi+80h] ; mov byte ptr [esp+18h],2 ; call ~CLast ; lea edi,[esi+50h] ; mov [esp+0Ch],edi ; mov dword ptr [edi],offset vtableB ; lea ecx,[edi+8] ; mov byte ptr [esp+18h],4 ; call ~CNode ; mov dword ptr [edi],offset root ; lea edi,[esi+20h] ; mov [esp+0Ch],edi ; mov dword ptr [edi],offset vtableA ; lea ecx,[edi+8] ; mov byte ptr [esp+18h],5 ; call ~CNode ; lea ecx,[esi+8] ; mov dword ptr [edi],offset root ; call ~CFirst ; ...
// Not polymorphic itself (no own vtable store); the inlined container destructors take the
// states after the class's own. The manifest label is propagated; the classes are named by
// address.
struct CGameFilters_00c44f80;
struct CRegistry_00c44f80 {
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04();
    virtual void v05(); virtual void v06(); virtual void v07();
    virtual void Remove(CGameFilters_00c44f80* filters);   // slot 8
};

struct COwner_00c44f80 {
    unsigned char _pad[0x14];
    CRegistry_00c44f80 m_registry;         // +0x14
};

struct CFirst_00c44f80 {                   // +8
    unsigned char _body[0x18];
    ~CFirst_00c44f80() throw();            // 0x00C22A00 (cannot throw: no state change precedes the call)
};

struct CNode_00c44f80 {
    unsigned char _body[0x28];
    ~CNode_00c44f80();                     // 0x00C19250
};

struct CRoot_00c44f80 {                    // vtable 0x012B1AB0: the root owns the vptr and the node pointer
    virtual void v00();
    CNode_00c44f80* m_pNode;               // +4
    ~CRoot_00c44f80() { }
};

struct CContainerA_00c44f80 : CRoot_00c44f80 {   // vtable 0x012B7CA0
    CNode_00c44f80 m_node;                 // +8
    ~CContainerA_00c44f80() { }
};

struct CContainerB_00c44f80 : CRoot_00c44f80 {   // vtable 0x012B7CE8
    CNode_00c44f80 m_node;                 // +8
    ~CContainerB_00c44f80() { }
};

struct CLast_00c44f80 {                    // +0x80
    unsigned char _body[0x10];
    ~CLast_00c44f80();                     // 0x00C61BD0
};

struct CGameFilters_00c44f80 {
    COwner_00c44f80* m_pOwner;             // +0
    long m_4;
    CFirst_00c44f80 m_first;               // +8
    CContainerA_00c44f80 m_streams;        // +0x20
    CContainerB_00c44f80 m_filters;        // +0x50
    CLast_00c44f80 m_last;                 // +0x80
    void CleanupA();                       // 0x00C44B80
    void CleanupB();                       // 0x00C44D30
    ~CGameFilters_00c44f80();
};

CGameFilters_00c44f80::~CGameFilters_00c44f80()
{
    CleanupA();
    CleanupB();
    m_pOwner->m_registry.Remove(this);
}
