// CAudioDevice_00ca4d80::Start() -- EH-framed: holds the critical section at +0x94 through a scoped lock (state 0); when the state at +0x64 is not 1 it becomes 1, the import @ IAT 0x014404AC is called with 1, virtual slot 16 runs, and either (no worker at +0x6c) virtual slot 24's result is returned, or the worker is queried through virtual slot 21 and on 0 the handle at +0x54 is signalled through the import @ IAT 0x0143FDE0; otherwise 0
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00CA4D80, oracle length 168 bytes. Built under /EHsc.
// Retail bytes:  push -1 ; push offset __ehhandler ; mov eax,fs:[0] ; push eax ; mov fs:[0],esp ; push ecx ; push esi ; push edi ; mov esi,ecx ; lea edi,[esi+94h] ; push edi ; mov [esp+0Ch],edi ; call [__imp_EnterCriticalSection] ; mov ecx,[esi+64h] ; mov eax,1 ; cmp ecx,eax ; mov dword ptr [esp+14h],0 ; je done ; push eax ; mov [esi+64h],eax ; call [__imp_timeBeginPeriod] ; mov eax,[esi] ; mov ecx,esi ; call [eax+40h] ; mov eax,[esi+6Ch] ; test eax,eax ; mov edx,[esi] ; mov ecx,esi ; jne worker ; call [edx+60h] ; push edi ; mov esi,eax ; call [__imp_LeaveCriticalSection] ; pop edi ; mov eax,esi ; ... ret ; worker: push eax ; call [edx+54h] ; test eax,eax ; jne done ; mov eax,[esi+54h] ; push eax ; call [__imp_SetEvent] ; done: push edi ; call [__imp_LeaveCriticalSection] ; ... xor eax,eax ; ... ret
// The two imports are modelled by their call shapes (one dword argument, `__stdcall`). The
// manifest label is propagated; the class is named by address.
extern "C" __declspec(dllimport) void __stdcall EnterCriticalSection(void* section);
extern "C" __declspec(dllimport) void __stdcall LeaveCriticalSection(void* section);
extern "C" __declspec(dllimport) unsigned int __stdcall timeBeginPeriod(unsigned int period);   // IAT 0x014404AC = WINMM timeBeginPeriod (Fable.exe import table), not Sleep
extern "C" __declspec(dllimport) long __stdcall SetEvent(void* handle);                   // IAT 0x0143FDE0

struct CSectionLock_00ca4d80 {             // scoped critical-section holder
    void* m_pSection;
    CSectionLock_00ca4d80(void* section) : m_pSection(section) { EnterCriticalSection(m_pSection); }
    ~CSectionLock_00ca4d80() { LeaveCriticalSection(m_pSection); }
};

struct CAudioDevice_00ca4d80 {
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04();
    virtual void v05(); virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09();
    virtual void v10(); virtual void v11(); virtual void v12(); virtual void v13(); virtual void v14();
    virtual void v15();
    virtual void Prime();                  // slot 16
    virtual void v17(); virtual void v18(); virtual void v19(); virtual void v20();
    virtual long Dispatch(void* worker);   // slot 21
    virtual void v22(); virtual void v23();
    virtual long Run();                    // slot 24
    unsigned char _pad[0x50];
    void* m_hEvent;                        // +0x54
    unsigned char _pad2[0xc];
    long m_state;                          // +0x64
    unsigned char _pad3[4];
    void* m_pWorker;                       // +0x6c
    unsigned char _pad4[0x24];
    unsigned char m_section[0x18];         // +0x94
    long Start();
};

long CAudioDevice_00ca4d80::Start()
{
    CSectionLock_00ca4d80 lock(m_section);
    if (m_state != 1) {
        m_state = 1;
        timeBeginPeriod(1);
        Prime();
        if (!m_pWorker)
            return Run();
        if (Dispatch(m_pWorker) == 0)
            SetEvent(m_hEvent);
    }
    return 0;
}