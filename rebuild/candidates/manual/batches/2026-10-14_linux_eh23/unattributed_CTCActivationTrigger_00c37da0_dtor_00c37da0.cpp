// CTCActivationTrigger_00c37da0::~CTCActivationTrigger_00c37da0() -- EH-framed destructor (vtable 0x012B5370): every entry the predicate container at +0x28 yields (virtual slot 11, re-queried) has its predicate at +4 released (@ 0x00C37B90); the owner's registry at +0x34, the registry at +0x104 of the world's +0x38 object reached through the owner's +0 and +4, and the owner's registry at +4 are told to drop `this` (virtual slot 8 each); then the container is destroyed inline (vtable 0x012B5328, node at +8 @ 0x00C19250 at state 2, root vtable 0x012B1AB0) and the member at +8 out of line (@ 0x00C38350)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C37DA0, oracle length 199 bytes. Built under /EHsc.
// Retail bytes:  ... mov dword ptr [esi],012B5370h ; mov eax,[esi+28h] ; lea edi,[esi+28h] ; mov ecx,edi ; mov dword ptr [esp+18h],1 ; call [eax+2Ch] ; test eax,eax ; je reg ; loop: mov ecx,[eax+4] ; push ecx ; mov ecx,esi ; call ReleasePredicate ; mov edx,[edi] ; mov ecx,edi ; call [edx+2Ch] ; test eax,eax ; jne loop ; reg: mov eax,[esi+4] ; lea ecx,[eax+34h] ; mov eax,[ecx] ; push esi ; call [eax+20h] ; mov ecx,[esi+4] ; mov edx,[ecx] ; mov eax,[edx+4] ; mov eax,[eax+38h] ; mov edx,[eax+104h] ; lea ecx,[eax+104h] ; push esi ; call [edx+20h] ; mov eax,[esi+4] ; lea ecx,[eax+4] ; mov eax,[ecx] ; push esi ; call [eax+20h] ; mov [esp+0Ch],edi ; mov dword ptr [edi],012B5328h ; lea ecx,[edi+8] ; mov byte ptr [esp+18h],2 ; call ~CNode ; lea ecx,[esi+8] ; mov dword ptr [edi],012B1AB0h ; mov dword ptr [esp+18h],-1 ; call ~CMember ; ...
// The same drain-and-unregister shape as 0x00C36590 with one more registry level. The manifest
// label is a description; the class is named by address.
struct CTCActivationTrigger_00c37da0;
struct CRegistry_00c37da0 {
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04();
    virtual void v05(); virtual void v06(); virtual void v07();
    virtual void Remove(CTCActivationTrigger_00c37da0* trigger);   // slot 8
};

struct CWorldPart_00c37da0 {
    unsigned char _pad[0x104];
    CRegistry_00c37da0 m_registry;         // +0x104
};

struct CWorldInner_00c37da0 {
    unsigned char _pad[0x38];
    CWorldPart_00c37da0* m_pPart;          // +0x38
};

struct CWorld_00c37da0 {
    long m_0;
    CWorldInner_00c37da0* m_pInner;        // +4
};

struct COwner_00c37da0 {
    CWorld_00c37da0* m_pWorld;             // +0
    CRegistry_00c37da0 m_registry4;        // +4
    unsigned char _pad[0x2c];
    CRegistry_00c37da0 m_registry34;       // +0x34
};

struct CEntry_00c37da0 {
    long m_0;
    void* m_pPredicate;                    // +4
};

struct CNode_00c37da0 {
    unsigned char _body[0x28];
    ~CNode_00c37da0();                     // 0x00C19250
};

struct CContainerRoot_00c37da0 {           // vtable 0x012B1AB0
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04();
    virtual void v05(); virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09();
    virtual void v10();
    virtual CEntry_00c37da0* Next();       // slot 11
    CNode_00c37da0* m_pNode;               // +4
    ~CContainerRoot_00c37da0() { }
};

struct CContainer_00c37da0 : CContainerRoot_00c37da0 {   // vtable 0x012B5328
    CNode_00c37da0 m_node;                 // +8
    ~CContainer_00c37da0() { }
};

struct CMember_00c37da0 {                  // +8
    unsigned char _body[0x20];
    ~CMember_00c37da0();                   // 0x00C38350
};

struct CTCActivationTrigger_00c37da0 {     // vtable 0x012B5370
    virtual void v00();
    COwner_00c37da0* m_pOwner;             // +4
    CMember_00c37da0 m_8;                  // +8
    CContainer_00c37da0 m_predicates;      // +0x28
    void ReleasePredicate(void* predicate);   // 0x00C37B90
    ~CTCActivationTrigger_00c37da0();
};

CTCActivationTrigger_00c37da0::~CTCActivationTrigger_00c37da0()
{
    CEntry_00c37da0* entry = m_predicates.Next();
    while (entry) {
        ReleasePredicate(entry->m_pPredicate);
        entry = m_predicates.Next();
    }
    m_pOwner->m_registry34.Remove(this);
    m_pOwner->m_pWorld->m_pInner->m_pPart->m_registry.Remove(this);
    m_pOwner->m_registry4.Remove(this);
}
