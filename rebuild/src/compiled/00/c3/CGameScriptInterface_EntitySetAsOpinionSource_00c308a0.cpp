// CGameScriptInterface::EntitySetAsOpinionSource(long) -- EH-framed: when virtual slot 15 reports an entity, a 4-byte polymorphic visitor (vtable 0x012B4C28) is built on the stack and handed to virtual slot 10; the argument is not read
// Leaf: EntitySetAsOpinionSource
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C308A0, oracle length 80 bytes. Built under /EHsc.
// Retail bytes:  push -1 ; push offset __ehhandler ; mov eax,fs:[0] ; push eax ; mov fs:[0],esp ; push ecx ; push esi ; mov esi,ecx ; mov eax,[esi] ; call [eax+3Ch] ; test eax,eax ; je done ; mov dword ptr [esp+4],offset visitor vtable ; mov edx,[esi] ; lea eax,[esp+4] ; push eax ; mov ecx,esi ; mov dword ptr [esp+14h],0 ; call [edx+28h] ; done: mov ecx,[esp+8] ; pop esi ; mov fs:[0],ecx ; add esp,10h ; ret 4
// The stack visitor has an inline constructor (vtable only) and an inline empty destructor, which
// is what registers EH state 0 around the virtual call (nothing is called on the normal path).
// The class comes from the manifest label; the visitor and the slots are descriptive.
struct CScriptVisitor_00c308a0 {           // vtable 0x012B4C28
    virtual void v00();
    CScriptVisitor_00c308a0() { }
    ~CScriptVisitor_00c308a0() { }
};

struct CGameScriptInterface {
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04();
    virtual void v05(); virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09();
    virtual void Visit(CScriptVisitor_00c308a0* visitor);   // slot 10
    virtual void v11(); virtual void v12(); virtual void v13(); virtual void v14();
    virtual void* GetEntity();             // slot 15
    void EntitySetAsOpinionSource(long entity);
};

void CGameScriptInterface::EntitySetAsOpinionSource(long)
{
    if (GetEntity()) {
        CScriptVisitor_00c308a0 visitor;
        Visit(&visitor);
    }
}