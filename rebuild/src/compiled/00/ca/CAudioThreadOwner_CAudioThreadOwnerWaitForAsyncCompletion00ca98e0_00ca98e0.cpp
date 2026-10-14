// CAudioThreadOwner_00ca98e0::WaitForAsyncCompletion() -- EH-framed under the owner's critical section at +0xa4 (one-word scoped holder, inline Enter/Leave, state 0 for the out-of-line unwind @ 0x00CA48C0; the manifest's "WaitForAsyncCompletion_WithRetry"): while the state at +0xc4 reads 2 (busy) the section is released, the pair of handles at +0xbc / +0xd0 waited on (WaitForMultipleObjects, either, infinite) and the section retaken; the first handle signalling retries, the second fails with 0x80040223, a wait error with the last-error HRESULT (@ 0x00CB0BC0) and anything else with E_UNEXPECTED; the +0xcc counter is bumped and 0 returned
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00CA98E0, oracle length 282 bytes. Built under /EHsc.
// Retail bytes:  ... sub esp,0Ch ; push ebx ; push ebp ; push esi ; push edi ; mov esi,ecx ; lea edi,[esi+0A4h] ; mov ebx,edi ; push ebx ; mov [esp+14h],ebx ; call [__imp_EnterCriticalSection] ; cmp dword ptr [esi+0C4h],2 ; mov ebp,[__imp_LeaveCriticalSection] ; mov dword ptr [esp+24h],0 ; jne done ; loop: push edi ; call ebp ; mov eax,[esi+0BCh] ; mov ecx,[esi+0D0h] ; push -1 ; push 0 ; lea edx,[esp+1Ch] ; push edx ; push 2 ; mov [esp+24h],eax ; mov [esp+28h],ecx ; call [__imp_WaitForMultipleObjects] ; push edi ; mov ebx,eax ; call [__imp_EnterCriticalSection] ; test ebx,ebx ; jne failed ; cmp dword ptr [esi+0C4h],2 ; je loop ; mov ebx,[esp+10h] ; done: mov ecx,[esi+0CCh] ; inc ecx ; push ebx ; mov [esi+0CCh],ecx ; call ebp ; ... xor eax,eax ; ... ret ; failed: cmp ebx,1 ; je second ; cmp ebx,-1 ; je error ; mov eax,[esp+10h] ; push eax ; call ebp ; ... mov eax,8000FFFFh ; ... ret ; error: call LastErrorToHResult ; mov ecx,[esp+10h] ; push ecx ; mov esi,eax ; call ebp ; ... mov eax,esi ; ... ret ; second: mov edx,[esp+10h] ; push edx ; call ebp ; ... mov eax,80040223h ; ... ret
// The classes are named by address.
extern "C" __declspec(dllimport) void __stdcall EnterCriticalSection(void* section);
extern "C" __declspec(dllimport) void __stdcall LeaveCriticalSection(void* section);
extern "C" __declspec(dllimport) unsigned long __stdcall WaitForMultipleObjects(unsigned long count, void* const* handles, int waitAll, unsigned long timeout);
long LastErrorToHResult_00ca98e0();        // 0x00CB0BC0

struct CSectionLock_00ca98e0 {             // scoped critical-section holder
    void* m_pSection;
    CSectionLock_00ca98e0(void* section) : m_pSection(section) { EnterCriticalSection(m_pSection); }
    ~CSectionLock_00ca98e0() { LeaveCriticalSection(m_pSection); }
};

struct CAudioThreadOwner_00ca98e0 {
    unsigned char _pad[0xa4];
    unsigned char m_section[0x18];         // +0xa4
    void* m_hDone;                         // +0xbc
    void* m_hThread;                       // +0xc0
    long m_state;                          // +0xc4: 2 while busy
    unsigned long m_threadId;              // +0xc8
    long m_waits;                          // +0xcc
    void* m_hAbort;                        // +0xd0
    long WaitForAsyncCompletion();
};

long CAudioThreadOwner_00ca98e0::WaitForAsyncCompletion()
{
    CSectionLock_00ca98e0 lock(m_section);
    while (m_state == 2) {
        LeaveCriticalSection(m_section);
        void* handles[2];
        handles[0] = m_hDone;
        handles[1] = m_hAbort;
        unsigned long result = WaitForMultipleObjects(2, handles, 0, 0xffffffff);
        EnterCriticalSection(m_section);
        switch (result) {
        case 0:
            break;
        case 1:
            return 0x80040223;
        case 0xffffffff:
            return LastErrorToHResult_00ca98e0();
        default:
            return 0x8000ffff;
        }
    }
    ++m_waits;
    return 0;
}