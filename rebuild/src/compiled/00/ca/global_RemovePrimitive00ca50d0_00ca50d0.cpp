// RemovePrimitive_00ca50d0(CPrimitiveOwner*) -- EH-framed `__stdcall`: holds the two critical sections at +0x7c and +0x94 of the owner's engine object at +0xcc through scoped locks (states 0 and 1), queries the engine through virtual slot 34, and when the result is not negative replaces it with the out-of-line removal (@ 0x00CA8E40); the sections are left in reverse order through the cached import
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00CA50D0, oracle length 147 bytes. Built under /EHsc.
// Retail bytes:  push -1 ; push offset __ehhandler ; mov eax,fs:[0] ; push eax ; mov fs:[0],esp ; push ecx ; push ebx ; mov ebx,[owner] ; push ebp ; mov ebp,[__imp_EnterCriticalSection] ; push esi ; mov esi,[ebx+0CCh] ; push edi ; add esi,7Ch ; push esi ; mov [esp+28h],esi ; call ebp ; mov edi,[ebx+0CCh] ; add edi,94h ; push edi ; mov dword ptr [esp+20h],0 ; mov [esp+14h],edi ; call ebp ; mov ecx,[ebx+0CCh] ; mov eax,[ecx] ; mov byte ptr [esp+1Ch],1 ; call [eax+88h] ; mov ebp,eax ; test ebp,ebp ; jl skip ; push ebx ; call Remove ; mov ebp,eax ; skip: push edi ; mov edi,[__imp_LeaveCriticalSection] ; call edi ; push esi ; call edi ; mov ecx,[esp+14h] ; pop edi ; pop esi ; mov eax,ebp ; pop ebp ; pop ebx ; mov fs:[0],ecx ; add esp,10h ; ret 4
// Two one-word scoped lock locals (each parked for the unwind funclet); the import pointers are
// cached in registers across the paired calls. `this` is not used, so the function is modelled
// as a `__stdcall` free function. The manifest label is a description; named by the shape.
extern "C" __declspec(dllimport) void __stdcall EnterCriticalSection(void* section);
extern "C" __declspec(dllimport) void __stdcall LeaveCriticalSection(void* section);

struct CSectionLock_00ca50d0 {             // scoped critical-section holder
    void* m_pSection;
    CSectionLock_00ca50d0(void* section) : m_pSection(section) { EnterCriticalSection(m_pSection); }
    ~CSectionLock_00ca50d0() { LeaveCriticalSection(m_pSection); }
};

struct CEngine_00ca50d0 {
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04();
    virtual void v05(); virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09();
    virtual void v10(); virtual void v11(); virtual void v12(); virtual void v13(); virtual void v14();
    virtual void v15(); virtual void v16(); virtual void v17(); virtual void v18(); virtual void v19();
    virtual void v20(); virtual void v21(); virtual void v22(); virtual void v23(); virtual void v24();
    virtual void v25(); virtual void v26(); virtual void v27(); virtual void v28(); virtual void v29();
    virtual void v30(); virtual void v31(); virtual void v32(); virtual void v33();
    virtual long Query();                  // slot 34
    unsigned char _pad[0x78];
    unsigned char m_firstSection[0x18];    // +0x7c
    unsigned char m_secondSection[0x18];   // +0x94
};

struct CPrimitiveOwner_00ca50d0 {
    unsigned char _pad[0xcc];
    CEngine_00ca50d0* m_pEngine;           // +0xcc
};

long __stdcall Remove_00ca8e40(CPrimitiveOwner_00ca50d0* owner);   // 0x00CA8E40

long __stdcall RemovePrimitive_00ca50d0(CPrimitiveOwner_00ca50d0* owner)
{
    CSectionLock_00ca50d0 first(owner->m_pEngine->m_firstSection);
    CSectionLock_00ca50d0 second(owner->m_pEngine->m_secondSection);
    long result = owner->m_pEngine->Query();
    if (result >= 0)
        result = Remove_00ca8e40(owner);
    return result;
}