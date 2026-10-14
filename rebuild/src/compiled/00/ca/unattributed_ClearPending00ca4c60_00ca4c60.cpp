// CAudioManager_00ca4c60::ClearPending() -- EH-framed: holds the critical section at +0x94; when the pending pointer at +0xc0 is set it is cleared and virtual slot 24 runs on `this`
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00CA4C60, oracle length 96 bytes. Built under /EHsc.
// Retail bytes:  push -1 ; push offset __ehhandler ; mov eax,fs:[0] ; push eax ; mov fs:[0],esp ; push ecx ; push esi ; push edi ; mov esi,ecx ; lea edi,[esi+94h] ; push edi ; mov [esp+0Ch],edi ; call [__imp_EnterCriticalSection] ; mov ecx,[esi+0C0h] ; xor eax,eax ; cmp ecx,eax ; mov [esp+14h],eax ; je skip ; mov [esi+0C0h],eax ; mov eax,[esi] ; mov ecx,esi ; call [eax+60h] ; skip: push edi ; call [__imp_LeaveCriticalSection] ; mov ecx,[esp+0Ch] ; pop edi ; pop esi ; mov fs:[0],ecx ; add esp,10h ; ret
// The scoped critical-section lock of 0x00CAF890 (the zero is shared between the EH state, the
// compare and the clearing store). The manifest label is propagated; the class is named by address.
struct CRITICAL_SECTION_00ca4c60;
extern "C" __declspec(dllimport) void __stdcall EnterCriticalSection(CRITICAL_SECTION_00ca4c60*);   // IAT 0x0143FE0C
extern "C" __declspec(dllimport) void __stdcall LeaveCriticalSection(CRITICAL_SECTION_00ca4c60*);   // IAT 0x0143FE10

struct CCriticalSectionLock_00ca4c60 {
    CRITICAL_SECTION_00ca4c60* m_pSection;
    CCriticalSectionLock_00ca4c60(CRITICAL_SECTION_00ca4c60* section) : m_pSection(section) { EnterCriticalSection(section); }
    ~CCriticalSectionLock_00ca4c60() { LeaveCriticalSection(m_pSection); }
};

struct CAudioManager_00ca4c60 {
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05();
    virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11();
    virtual void v12(); virtual void v13(); virtual void v14(); virtual void v15(); virtual void v16(); virtual void v17();
    virtual void v18(); virtual void v19(); virtual void v20(); virtual void v21(); virtual void v22(); virtual void v23();
    virtual void OnPendingCleared();       // slot 24
    unsigned char _pad[0x90];
    unsigned char m_section[0x2c];         // +0x94: the critical section
    void* m_pPending;                      // +0xc0
    void ClearPending();
};

void CAudioManager_00ca4c60::ClearPending()
{
    CCriticalSectionLock_00ca4c60 lock((CRITICAL_SECTION_00ca4c60*)m_section);
    if (m_pPending) {
        m_pPending = 0;
        OnPendingCleared();
    }
}