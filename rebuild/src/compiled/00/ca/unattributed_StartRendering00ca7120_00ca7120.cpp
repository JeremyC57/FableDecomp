// CAudioStream_00ca7120::StartRendering() -- EH-framed: holds the critical section at +0x94 through a scoped lock (state 0); 0 when the source at +0x78 has no buffer at +0x18, else the source's command interface at +0xc is referenced (`__stdcall` slot 1 with `this` pushed), the start command (0x16) issued with it (@ 0x00CA7930, the pointer converted with a null check), the queued word at +0x60 set, virtual slot 28 run and the interface released (slot 2); returns 1
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00CA7120, oracle length 178 bytes. Built under /EHsc.
// Retail bytes:  ... mov eax,[esi+78h] ; mov ecx,[eax+18h] ; test ecx,ecx ; mov dword ptr [esp+14h],0 ; jne go ; push edi ; call [__imp_LeaveCriticalSection] ; pop edi ; xor eax,eax ; ... ret ; go: test eax,eax ; push ebx ; je null ; lea ebx,[eax+0Ch] ; jmp issue ; null: xor ebx,ebx ; issue: mov ecx,[eax+0Ch] ; add eax,0Ch ; push eax ; call [ecx+4] ; push 0 ; push ebx ; push 16h ; mov ecx,esi ; call Issue ; mov edx,[esi] ; mov ecx,esi ; mov dword ptr [esi+60h],1 ; call [edx+70h] ; mov esi,[esi+78h] ; mov ecx,[esi+0Ch] ; lea eax,[esi+0Ch] ; push eax ; call [ecx+8] ; push edi ; call [__imp_LeaveCriticalSection] ; ... mov eax,1 ; ...
// The member call through the derived pointer adjusts without a null check; the converted
// pointer passed to the issue carries one. The sibling of the auto-stop and restart bodies
// landed at 0x00CA7080 / 0x00CA4CC0. The manifest label is propagated; the class is named by
// address.
extern "C" __declspec(dllimport) void __stdcall EnterCriticalSection(void* section);
extern "C" __declspec(dllimport) void __stdcall LeaveCriticalSection(void* section);

struct CSectionLock_00ca7120 {             // scoped critical-section holder
    void* m_pSection;
    CSectionLock_00ca7120(void* section) : m_pSection(section) { EnterCriticalSection(m_pSection); }
    ~CSectionLock_00ca7120() { LeaveCriticalSection(m_pSection); }
};

struct CSourceHead_00ca7120 {              // the 12 bytes in front of the command interface (own vtable, so the interface stays at +0xc)
    virtual void v00();
    long m_a, m_b;
};

struct ICommandTarget_00ca7120 {           // +0xc of the source: COM-style, `this` pushed
    virtual long __stdcall QueryInterface();
    virtual unsigned long __stdcall AddRef();        // slot 1
    virtual unsigned long __stdcall Release();       // slot 2
    unsigned char _pad[8];
    void* m_pBuffer;                       // +0xc (= +0x18 of the source)
};

struct CSource_00ca7120 : CSourceHead_00ca7120, ICommandTarget_00ca7120 { };

struct CAudioStream_00ca7120 {
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04();
    virtual void v05(); virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09();
    virtual void v10(); virtual void v11(); virtual void v12(); virtual void v13(); virtual void v14();
    virtual void v15(); virtual void v16(); virtual void v17(); virtual void v18(); virtual void v19();
    virtual void v20(); virtual void v21(); virtual void v22(); virtual void v23(); virtual void v24();
    virtual void v25(); virtual void v26(); virtual void v27();
    virtual void OnStart();                // slot 28
    unsigned char _pad[0x5c];
    long m_queued;                         // +0x60
    unsigned char _pad2[0x14];
    CSource_00ca7120* m_pSource;           // +0x78
    unsigned char _pad3[0x18];
    unsigned char m_section[0x18];         // +0x94
    long Issue(long command, ICommandTarget_00ca7120* target, long flags);   // 0x00CA7930
    long StartRendering();
};

long CAudioStream_00ca7120::StartRendering()
{
    CSectionLock_00ca7120 lock(m_section);
    if (!m_pSource->m_pBuffer)
        return 0;
    ICommandTarget_00ca7120* target = m_pSource;
    m_pSource->AddRef();
    Issue(0x16, target, 0);
    m_queued = 1;
    OnStart();
    m_pSource->Release();
    return 1;
}