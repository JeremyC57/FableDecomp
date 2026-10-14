// CPredicateOwner_00c473f0::~CPredicateOwner_00c473f0() -- EH-framed destructor: two out-of-line cleanups on `this` (@ 0x00C471C0, @ 0x00C47170), the registry at +0x28 of the owner at +0 told to drop `this` (virtual slot 8), then the two container members at +0x38 (vtable 0x012B7FD0) and +8 (vtable 0x012B7F88) destroyed inline (node at +8 @ 0x00C19250 at states 2 and 3, root vtable 0x012B1AB0 after each)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C473F0, oracle length 147 bytes. Built under /EHsc.
// Retail bytes:  push -1 ; push offset __ehhandler ; mov eax,fs:[0] ; push eax ; mov fs:[0],esp ; sub esp,8 ; push esi ; mov esi,ecx ; push edi ; mov [esp+8],esi ; mov dword ptr [esp+18h],1 ; call Cleanup ; mov ecx,esi ; call Drain ; mov eax,[esi] ; lea ecx,[eax+28h] ; mov eax,[ecx] ; push esi ; call [eax+20h] ; lea edi,[esi+38h] ; mov [esp+0Ch],edi ; mov dword ptr [edi],offset vtableB ; lea ecx,[edi+8] ; mov byte ptr [esp+18h],2 ; call ~CNode ; add esi,8 ; mov dword ptr [edi],offset root ; mov [esp+0Ch],esi ; mov dword ptr [esi],offset vtableA ; lea ecx,[esi+8] ; mov dword ptr [esp+18h],3 ; call ~CNode ; mov ecx,[esp+10h] ; mov dword ptr [esi],offset root ; pop edi ; pop esi ; mov fs:[0],ecx ; add esp,14h ; ret
// Not polymorphic itself (no own vtable store); the body runs at state 1 (both members alive).
// The manifest label (`~CPredicate_And`) is propagated; the classes are named by address.
struct CPredicateOwner_00c473f0;
struct CRegistry_00c473f0 {
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04();
    virtual void v05(); virtual void v06(); virtual void v07();
    virtual void Remove(CPredicateOwner_00c473f0* owner);   // slot 8
};

struct COwner_00c473f0 {
    unsigned char _pad[0x28];
    CRegistry_00c473f0 m_registry;         // +0x28
};

struct CNode_00c473f0 {
    unsigned char _body[0x28];
    ~CNode_00c473f0();                     // 0x00C19250
};

struct CRoot_00c473f0 {                    // vtable 0x012B1AB0: the root owns the vptr and the node pointer
    virtual void v00();
    CNode_00c473f0* m_pNode;               // +4
    ~CRoot_00c473f0() { }
};

struct CContainerA_00c473f0 : CRoot_00c473f0 {   // vtable 0x012B7F88
    CNode_00c473f0 m_node;                 // +8
    ~CContainerA_00c473f0() { }
};

struct CContainerB_00c473f0 : CRoot_00c473f0 {   // vtable 0x012B7FD0
    CNode_00c473f0 m_node;                 // +8
    ~CContainerB_00c473f0() { }
};

struct CPredicateOwner_00c473f0 {
    COwner_00c473f0* m_pOwner;             // +0
    long m_4;
    CContainerA_00c473f0 m_first;          // +8
    CContainerB_00c473f0 m_second;         // +0x38
    void Cleanup();                        // 0x00C471C0
    void Drain();                          // 0x00C47170
    ~CPredicateOwner_00c473f0();
};

CPredicateOwner_00c473f0::~CPredicateOwner_00c473f0()
{
    Cleanup();
    Drain();
    m_pOwner->m_registry.Remove(this);
}
