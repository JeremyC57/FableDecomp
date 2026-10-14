// CCombatScript_00c2cc60::~CCombatScript_00c2cc60() -- EH-framed destructor: own vtable (0x012B46FC), every event the container member at +8 still yields (virtual slot 11) is handed to the block/parry dispatch (@ 0x00C2C8F0), the registry at +0x10 of the owner at +4 told to drop `this` (virtual slot 8), the polymorphic member at +0x38 stores its vtable (0x012B2828) from its inlined empty destructor, the container destroyed inline (vtable 0x012B46B8, node at +8 @ 0x00C19250 at state 3, root vtable 0x012B1AB0), then the script root stores its vtable (0x012B463C)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C2CC60, oracle length 154 bytes. Built under /EHsc.
// Retail bytes:  push -1 ; push offset __ehhandler ; mov eax,fs:[0] ; push eax ; mov fs:[0],esp ; sub esp,8 ; push esi ; mov esi,ecx ; push edi ; mov [esp+8],esi ; mov dword ptr [esi],offset vtable ; mov eax,[esi+8] ; lea edi,[esi+8] ; mov ecx,edi ; mov dword ptr [esp+18h],2 ; call [eax+2Ch] ; test eax,eax ; je done ; loop: push eax ; mov ecx,esi ; call Dispatch ; mov edx,[edi] ; mov ecx,edi ; call [edx+2Ch] ; test eax,eax ; jne loop ; done: mov eax,[esi+4] ; lea ecx,[eax+10h] ; mov eax,[ecx] ; push esi ; call [eax+20h] ; mov dword ptr [esi+38h],offset listenervtable ; mov [esp+0Ch],edi ; mov dword ptr [edi],offset containervtable ; lea ecx,[edi+8] ; mov byte ptr [esp+18h],3 ; call ~CNode ; mov ecx,[esp+10h] ; mov dword ptr [edi],offset root ; mov dword ptr [esi],offset scriptroot ; pop edi ; pop esi ; mov fs:[0],ecx ; add esp,14h ; ret
// The destructor counterpart of the (parked) constructor at 0x00C2CBD0: the owner pointer
// belongs to the polymorphic script root whose inlined destructor stores its own vtable last.
// The manifest label is propagated; the classes are named by address.
struct CCombatScript_00c2cc60;
struct CRegistry_00c2cc60 {
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04();
    virtual void v05(); virtual void v06(); virtual void v07();
    virtual void Remove(CCombatScript_00c2cc60* script);   // slot 8
};

struct COwner_00c2cc60 {
    unsigned char _pad[0x10];
    CRegistry_00c2cc60 m_registry;         // +0x10
};

struct CScriptRoot_00c2cc60 {              // vtable 0x012B463C: the polymorphic root that owns the owner pointer
    virtual void v00();
    COwner_00c2cc60* m_pOwner;             // +4
    ~CScriptRoot_00c2cc60() { }
};

struct CEvent_00c2cc60;
struct CNode_00c2cc60 {
    unsigned char _body[0x28];
    ~CNode_00c2cc60();                     // 0x00C19250
};

struct CContainerRoot_00c2cc60 {           // vtable 0x012B1AB0
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04();
    virtual void v05(); virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09();
    virtual void v10();
    virtual CEvent_00c2cc60* Next();       // slot 11
    CNode_00c2cc60* m_pNode;               // +4
    ~CContainerRoot_00c2cc60() { }
};

struct CContainer_00c2cc60 : CContainerRoot_00c2cc60 {   // vtable 0x012B46B8, 0x30 bytes
    CNode_00c2cc60 m_node;                 // +8
    ~CContainer_00c2cc60() { }
};

struct CListener_00c2cc60 {                // +0x38, vtable 0x012B2828
    virtual void v00();
    unsigned char _body[0xc];
    ~CListener_00c2cc60() { }
};

struct CCombatScript_00c2cc60 : CScriptRoot_00c2cc60 {   // vtable 0x012B46FC
    CContainer_00c2cc60 m_events;          // +8
    CListener_00c2cc60 m_listener;         // +0x38
    void HandleBlockParryEvent(CEvent_00c2cc60* event);   // 0x00C2C8F0
    ~CCombatScript_00c2cc60();
};

CCombatScript_00c2cc60::~CCombatScript_00c2cc60()
{
    CEvent_00c2cc60* event = m_events.Next();
    while (event) {
        HandleBlockParryEvent(event);
        event = m_events.Next();
    }
    m_pOwner->m_registry.Remove(this);
}
