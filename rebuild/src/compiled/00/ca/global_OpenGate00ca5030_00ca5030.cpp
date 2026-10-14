// OpenGate_00ca5030(CPrimitiveOwner*) -- EH-framed `__stdcall`: holds the two critical sections at +0x7c and +0x94 of the owner's engine object at +0xcc; under both the owner is opened out of line (@ 0x00CA8E10) and the engine's virtual slot 33 runs, the inner section is left, then the engine's virtual slot 25 yields the result before the outer section is left
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00CA5030, oracle length 157 bytes. Built under /EHsc.
// Retail bytes:  ... mov ebx,[__imp_EnterCriticalSection] ; ... mov esi,[owner] ; ... mov edi,[esi+0CCh] ; add edi,7Ch ; push edi ; mov [esp+28h],edi ; call ebx ; mov ebp,[esi+0CCh] ; add ebp,94h ; push ebp ; mov dword ptr [esp+20h],0 ; mov [esp+14h],ebp ; call ebx ; push esi ; mov byte ptr [esp+20h],1 ; call Open ; mov ecx,[esi+0CCh] ; mov eax,[ecx] ; call [eax+84h] ; mov ebx,[__imp_LeaveCriticalSection] ; push ebp ; mov byte ptr [esp+20h],0 ; call ebx ; mov ecx,[esi+0CCh] ; mov edx,[ecx] ; call [edx+64h] ; push edi ; mov esi,eax ; call ebx ; ...
// The inner lock lives in a nested block; the import pointers are cached in ebx across the
// paired calls. `this` is not used (a `__stdcall` free function). The manifest label is
// propagated; named by the shape.
extern "C" __declspec(dllimport) void __stdcall EnterCriticalSection(void* section);
extern "C" __declspec(dllimport) void __stdcall LeaveCriticalSection(void* section);

struct CSectionLock_00ca5030 {             // scoped critical-section holder
    void* m_pSection;
    CSectionLock_00ca5030(void* section) : m_pSection(section) { EnterCriticalSection(m_pSection); }
    ~CSectionLock_00ca5030() { LeaveCriticalSection(m_pSection); }
};

struct CEngine_00ca5030 {
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04();
    virtual void v05(); virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09();
    virtual void v10(); virtual void v11(); virtual void v12(); virtual void v13(); virtual void v14();
    virtual void v15(); virtual void v16(); virtual void v17(); virtual void v18(); virtual void v19();
    virtual void v20(); virtual void v21(); virtual void v22(); virtual void v23(); virtual void v24();
    virtual long Finish();                 // slot 25
    virtual void v26(); virtual void v27(); virtual void v28(); virtual void v29(); virtual void v30();
    virtual void v31(); virtual void v32();
    virtual void Apply();                  // slot 33
    unsigned char _pad[0x78];
    unsigned char m_firstSection[0x18];    // +0x7c
    unsigned char m_secondSection[0x18];   // +0x94
};

struct CPrimitiveOwner_00ca5030 {
    unsigned char _pad[0xcc];
    CEngine_00ca5030* m_pEngine;           // +0xcc
};

void __stdcall Open_00ca8e10(CPrimitiveOwner_00ca5030* owner);   // 0x00CA8E10

long __stdcall OpenGate_00ca5030(CPrimitiveOwner_00ca5030* owner)
{
    CSectionLock_00ca5030 first(owner->m_pEngine->m_firstSection);
    {
        CSectionLock_00ca5030 second(owner->m_pEngine->m_secondSection);
        Open_00ca8e10(owner);
        owner->m_pEngine->Apply();
    }
    return owner->m_pEngine->Finish();
}