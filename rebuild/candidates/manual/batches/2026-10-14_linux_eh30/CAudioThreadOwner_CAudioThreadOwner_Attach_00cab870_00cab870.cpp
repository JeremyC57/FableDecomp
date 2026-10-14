// CAudioThreadOwner_00cab870::Attach(thread) -- EH-framed under the owner's critical section at +0xa4 (one-word scoped holder, inline Enter/Leave, state 0 for the out-of-line unwind @ 0x00CA48C0; the manifest's "NHitLocations::CPhysicalPrimitiveCylinder" is propagated): an owner already attached (+0xc4) fails with 0x80040293 when the caller is the attached thread (+0xc8 against GetCurrentThreadId) and 0x80040294 otherwise; the caller's thread handle is duplicated into +0xc0 with THREAD_SUSPEND_RESUME access (GetCurrentProcess twice, DuplicateHandle), a failure returning the last-error HRESULT (@ 0x00CB0BC0); +0xc4 is set and +0xc8 takes the current thread id, slot 25 is asked whether the owner is ready and if not the fallback @ 0x00CA9820 runs; returns 0
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00CAB870, oracle length 281 bytes. Built under /EHsc.
// Retail bytes:  ... push ecx ; push esi ; push edi ; mov esi,ecx ; lea edi,[esi+0A4h] ; push edi ; mov [esp+0Ch],edi ; call [__imp_EnterCriticalSection] ; mov eax,[esi+0C4h] ; test eax,eax ; mov dword ptr [esp+14h],0 ; je attach ; call [__imp_GetCurrentThreadId] ; cmp [esi+0C8h],eax ; push edi ; jne other ; call [__imp_LeaveCriticalSection] ; ... mov eax,80040293h ; ... ret 4 ; other: call [__imp_LeaveCriticalSection] ; ... mov eax,80040294h ; ... ret 4 ; attach: push ebx ; mov ebx,[__imp_GetCurrentProcess] ; push 0 ; push 0 ; push 2 ; lea eax,[esi+0C0h] ; push eax ; call ebx ; mov ecx,[esp+30h] ; push eax ; push ecx ; call ebx ; push eax ; call [__imp_DuplicateHandle] ; test eax,eax ; pop ebx ; jne ok ; call LastErrorToHResult ; push edi ; mov esi,eax ; call [__imp_LeaveCriticalSection] ; ... mov eax,esi ; ... ret 4 ; ok: mov dword ptr [esi+0C4h],1 ; call [__imp_GetCurrentThreadId] ; mov edx,[esi] ; mov ecx,esi ; mov [esi+0C8h],eax ; call [edx+64h] ; test al,al ; jne done ; mov ecx,esi ; call CAudioThreadOwner::Fallback ; done: push edi ; call [__imp_LeaveCriticalSection] ; ... xor eax,eax ; ... ret 4
// The classes are named by address.
extern "C" __declspec(dllimport) void __stdcall EnterCriticalSection(void* section);
extern "C" __declspec(dllimport) void __stdcall LeaveCriticalSection(void* section);
extern "C" __declspec(dllimport) unsigned long __stdcall GetCurrentThreadId();
extern "C" __declspec(dllimport) void* __stdcall GetCurrentProcess();
extern "C" __declspec(dllimport) int __stdcall DuplicateHandle(void* sourceProcess, void* source, void* targetProcess, void** target, unsigned long access, int inherit, unsigned long options);
long LastErrorToHResult_00cab870();        // 0x00CB0BC0

struct CSectionLock_00cab870 {             // scoped critical-section holder
    void* m_pSection;
    CSectionLock_00cab870(void* section) : m_pSection(section) { EnterCriticalSection(m_pSection); }
    ~CSectionLock_00cab870() { LeaveCriticalSection(m_pSection); }
};

struct CAudioThreadOwner_00cab870 {
    virtual void Slot00(); virtual void Slot04(); virtual void Slot08(); virtual void Slot0C();
    virtual void Slot10(); virtual void Slot14(); virtual void Slot18(); virtual void Slot1C();
    virtual void Slot20(); virtual void Slot24(); virtual void Slot28(); virtual void Slot2C();
    virtual void Slot30(); virtual void Slot34(); virtual void Slot38(); virtual void Slot3C();
    virtual void Slot40(); virtual void Slot44(); virtual void Slot48(); virtual void Slot4C();
    virtual void Slot50(); virtual void Slot54(); virtual void Slot58(); virtual void Slot5C();
    virtual void Slot60();
    virtual bool IsReady();                // slot 0x64
    unsigned char _pad[0xa0];
    unsigned char m_section[0x1c];         // +0xa4
    void* m_hThread;                       // +0xc0
    long m_bAttached;                      // +0xc4
    unsigned long m_threadId;              // +0xc8
    void Fallback();                       // 0x00CA9820
    long Attach(void* thread);
};

long CAudioThreadOwner_00cab870::Attach(void* thread)
{
    CSectionLock_00cab870 lock(m_section);
    if (m_bAttached) {
        if (m_threadId == GetCurrentThreadId())
            return 0x80040293;
        return 0x80040294;
    }
    if (!DuplicateHandle(GetCurrentProcess(), thread, GetCurrentProcess(), &m_hThread, 2, 0, 0))
        return LastErrorToHResult_00cab870();
    m_bAttached = 1;
    m_threadId = GetCurrentThreadId();
    if (!IsReady())
        Fallback();
    return 0;
}
