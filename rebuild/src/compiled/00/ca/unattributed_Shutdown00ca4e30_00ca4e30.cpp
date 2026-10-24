// CAudioManager_00ca4e30::Shutdown() -- EH-framed: holds the critical section at +0x94, clears the member at +0x74, and when the state at +0x64 is 1 clears it, runs virtual slot 17 and a one-argument import with 1; returns 0
// Leaf: Shutdown_00ca4e30
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00CA4E30, oracle length 104 bytes. Built under /EHsc.
// Retail bytes:  push -1 ; push offset __ehhandler ; mov eax,fs:[0] ; push eax ; mov fs:[0],esp ; push ecx ; push esi ; push edi ; mov esi,ecx ; lea edi,[esi+94h] ; push edi ; mov [esp+0Ch],edi ; call [__imp_EnterCriticalSection] ; mov ecx,[esi+64h] ; xor eax,eax ; cmp ecx,1 ; mov [esp+14h],eax ; mov [esi+74h],eax ; jne skip ; mov [esi+64h],eax ; mov eax,[esi] ; mov ecx,esi ; call [eax+44h] ; push 1 ; call [__imp_Notify] ; skip: push edi ; call [__imp_LeaveCriticalSection] ; mov ecx,[esp+0Ch] ; pop edi ; xor eax,eax ; pop esi ; mov fs:[0],ecx ; add esp,10h ; ret
// The scoped critical-section lock of 0x00CAF890 (the manager of 0x00CA4C60). The manifest label
// is propagated; the class is named by address.
struct CRITICAL_SECTION_00ca4e30;
extern "C" __declspec(dllimport) void __stdcall EnterCriticalSection(CRITICAL_SECTION_00ca4e30*);   // IAT 0x0143FE0C
extern "C" __declspec(dllimport) void __stdcall LeaveCriticalSection(CRITICAL_SECTION_00ca4e30*);   // IAT 0x0143FE10

struct CCriticalSectionLock_00ca4e30 {
    CRITICAL_SECTION_00ca4e30* m_pSection;
    CCriticalSectionLock_00ca4e30(CRITICAL_SECTION_00ca4e30* section) : m_pSection(section) { EnterCriticalSection(section); }
    ~CCriticalSectionLock_00ca4e30() { LeaveCriticalSection(m_pSection); }
};
extern "C" __declspec(dllimport) void __stdcall NotifyShutdown_00ca4e30(long state);   // IAT 0x014404A8

struct CAudioManager_00ca4e30 {
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05();
    virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11();
    virtual void v12(); virtual void v13(); virtual void v14(); virtual void v15(); virtual void v16();
    virtual void OnShutdown();             // slot 17
    unsigned char _pad[0x60];
    long m_state;                          // +0x64
    unsigned char _pad2[0xc];
    long m_pending;                        // +0x74
    unsigned char _pad3[0x1c];
    unsigned char m_section[4];            // +0x94
    long Shutdown();
};

long CAudioManager_00ca4e30::Shutdown()
{
    CCriticalSectionLock_00ca4e30 lock((CRITICAL_SECTION_00ca4e30*)m_section);
    m_pending = 0;
    if (m_state == 1) {
        m_state = 0;
        OnShutdown();
        NotifyShutdown_00ca4e30(1);
    }
    return 0;
}