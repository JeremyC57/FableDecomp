// CScriptContext_00c36cf0::CheckThingAlive(void*) -- EH-framed: a 0x3c-byte helper built on the stack from (the argument, the member at +4) (@ 0x00C467D0), queried (@ 0x00C465D0) and destroyed (@ 0x00C46860); the query result returned
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C36CF0, oracle length 99 bytes. Built under /EHsc.
// Retail bytes:  push -1 ; push offset __ehhandler ; mov eax,fs:[0] ; push eax ; mov fs:[0],esp ; sub esp,3Ch ; mov eax,[ecx+4] ; mov ecx,[thing] ; push esi ; push eax ; push ecx ; lea ecx,[esp+0Ch] ; call CHelper::CHelper ; lea ecx,[esp+4] ; mov dword ptr [esp+48h],0 ; call CHelper::Check ; lea ecx,[esp+4] ; mov esi,eax ; mov dword ptr [esp+48h],-1 ; call ~CHelper ; mov ecx,[esp+40h] ; mov eax,esi ; pop esi ; mov fs:[0],ecx ; add esp,48h ; ret 4
// The helper's out-of-line constructor, query and destructor; the result survives the destructor
// in esi. The manifest label is propagated; the class is named by address.
struct CAliveHelper_00c36cf0 {             // 0x3c-byte stack helper
    unsigned char _body[0x3c];
    CAliveHelper_00c36cf0(void* thing, void* context);   // 0x00C467D0
    long Check();                          // 0x00C465D0
    ~CAliveHelper_00c36cf0();              // 0x00C46860
};

struct CScriptContext_00c36cf0 {
    unsigned char _pad[4];
    void* m_pContext;                      // +4
    long CheckThingAlive(void* thing);
};

long CScriptContext_00c36cf0::CheckThingAlive(void* thing)
{
    CAliveHelper_00c36cf0 helper(thing, m_pContext);
    return helper.Check();
}