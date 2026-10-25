// CEventScript::~CEventScript() -- EH-framed destructor of the libpk event script: own vtable (0x012B46FC), every event the container at +8 still yields (virtual slot 11) is handed to the event handler (@ 0x00C2C8F0), the owner's script registry (+0x10 of the owner at +4) told to drop `this` (virtual slot 8); then the flag override at +0x38 (its inlined base destructor stores 0x012B2828), the event container (vtable 0x012B46B8; its red-black tree at +8 @ 0x00C19250 at state 3; root vtable 0x012B1AB0) and the script root (vtable 0x012B463C)
// Leaf: ??1CEventScript@@QAE@XZ
// Flags: /EHsc
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C2CC60, oracle length 154 bytes. Built under /EHsc.
// Retail bytes:  push -1 ; push offset __ehhandler ; mov eax,fs:[0] ; push eax ; mov fs:[0],esp ; sub esp,8 ; push esi ; mov esi,ecx ; push edi ; mov [esp+8],esi ; mov dword ptr [esi],offset vtable ; mov eax,[esi+8] ; lea edi,[esi+8] ; mov ecx,edi ; mov dword ptr [esp+18h],2 ; call [eax+2Ch] ; test eax,eax ; je done ; loop: push eax ; mov ecx,esi ; call HandleEvent ; mov edx,[edi] ; mov ecx,edi ; call [edx+2Ch] ; test eax,eax ; jne loop ; done: mov eax,[esi+4] ; lea ecx,[eax+10h] ; mov eax,[ecx] ; push esi ; call [eax+20h] ; mov dword ptr [esi+38h],offset flagbasevtable ; mov [esp+0Ch],edi ; mov dword ptr [edi],offset containervtable ; lea ecx,[edi+8] ; mov byte ptr [esp+18h],3 ; call ~PKCTRRedblackTree ; mov ecx,[esp+10h] ; mov dword ptr [edi],offset root ; mov dword ptr [esi],offset scriptroot ; pop edi ; pop esi ; mov fs:[0],ecx ; add esp,14h ; ret
// Naming (sweep1, 2026-10-25): was landed as `CCombatScript_00c2cc60::~CCombatScript_00c2cc60` (the manifest's
//   CQ_GuildTrainingPreMeleeScript label is propagated); FINDINGS chunk 436: it is the destructor of the libpk
//   CEventScript whose constructor 0x00C2CBD0 (parked by agent436, scheduling only) builds exactly these members: the
//   script root with the owner (+4), the event container over a PKCTRRedblackTree (+8, ctor 0x00C194F0), the
//   CFlagOverride at +0x38 (ctor 0x00C2C6B0, vtable 0x012B4630, base 0x012B2828) and the registration with the
//   owner's script registry (slot 9; slot 8 removes here). Descriptive (no PDB covers libpk); the class model is
//   agent436's.
// Callees: CEventScript::HandleEvent @ 0x00C2C8F0 (the catalog's "Combat_HandleBlockParryEvent" is propagated),
//   ~PKCTRRedblackTree @ 0x00C19250.
class CEventScript;
struct CEvent;

struct CScriptRegistry {
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04();
    virtual void v05(); virtual void v06(); virtual void v07();
    virtual void Remove(CEventScript* script);   // slot 8 (+0x20)
    virtual void Add(CEventScript* script);      // slot 9 (+0x24)
};

struct CScriptOwner {
    unsigned char   _pad_0x00[0x10];
    CScriptRegistry m_registry;             // +0x10
};

class PKCTRRedblackTree {                   // 0x28 bytes (vtable 0x012B2F0C)
public:
    ~PKCTRRedblackTree();                   // 0x00C19250
private:
    void* m_words[10];
};

class CScriptRoot {                         // vtable 0x012B463C
public:
    virtual void v00();
    ~CScriptRoot() { }
    CScriptOwner* m_pOwner;                 // +0x04
};

class CContainerRoot {                      // vtable 0x012B1AB0
public:
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04();
    virtual void v05(); virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09();
    virtual void v10();
    virtual CEvent* Next();                 // slot 11 (+0x2c)
    ~CContainerRoot() { }
    PKCTRRedblackTree* m_pTree;             // +0x04
};

class CEventContainer : public CContainerRoot {   // vtable 0x012B46B8, 0x30 bytes
public:
    virtual void v00();
    ~CEventContainer() { }
    PKCTRRedblackTree m_tree;               // +0x08
};

class CFlagOverrideBase {                   // vtable 0x012B2828
public:
    virtual void v00();
    ~CFlagOverrideBase() { }
};

class CFlagOverride : public CFlagOverrideBase {  // vtable 0x012B4630, 12 bytes
public:
    virtual void v00();
    ~CFlagOverride() { }
private:
    unsigned short m_words[4];              // +0x04 .. +0x0a
};

class CEventScript : public CScriptRoot {   // vtable 0x012B46FC
public:
    virtual void v00();
    ~CEventScript();
    void HandleEvent(CEvent* event);        // 0x00C2C8F0
    CEventContainer m_events;               // +0x08
    CFlagOverride   m_flags;                // +0x38
};

CEventScript::~CEventScript()
{
    CEvent* event = m_events.Next();
    while (event) {
        HandleEvent(event);
        event = m_events.Next();
    }
    m_pOwner->m_registry.Remove(this);
}
