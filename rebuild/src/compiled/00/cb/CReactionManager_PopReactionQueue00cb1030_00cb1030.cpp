// CReactionManager::PopReactionQueue(SReaction*) -- EH-framed: a one-word scoped lock over the critical section at +0x24 (state 0) guards each pop from the queue at +0xc (@ 0x00CB1900); while nothing is queued the waiter count at +0x40 is bumped, the lock dropped and the event at +0x3c waited for (INFINITE) before relocking; the popped 16-byte record is copied to the out pointer after the unlock and freed (`delete`, @ 0x00BFE9BC)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00CB1030, oracle length 197 bytes. Built under /EHsc.
// Retail bytes:  ... lea edi,[esi+24h] ; push edi ; mov [esp+14h],edi ; call [__imp_EnterCriticalSection] ; lea ebp,[esi+0Ch] ; mov ecx,ebp ; mov dword ptr [esp+1Ch],0 ; call CQueue::Pop ; mov ebx,eax ; test ebx,ebx ; jne have ; wait: mov ecx,[esi+40h] ; inc ecx ; push edi ; mov [esi+40h],ecx ; call [__imp_LeaveCriticalSection] ; mov eax,[esi+3Ch] ; push -1 ; push eax ; call [__imp_WaitForSingleObject] ; push edi ; mov [esp+14h],edi ; call [__imp_EnterCriticalSection] ; mov ecx,ebp ; mov dword ptr [esp+1Ch],0 ; call CQueue::Pop ; mov ebx,eax ; test ebx,ebx ; je wait ; have: push edi ; mov dword ptr [esp+20h],-1 ; call [__imp_LeaveCriticalSection] ; mov edx,[esp+24h] ; mov ecx,ebx ; (four dword copies) ; push ebx ; call operator delete ; ...
// The lock is scoped to the inner block of the wait loop, so the state returns to 0 on each
// relock without a -1 store (the imports cannot throw under /EHsc). Named from the manifest.
extern "C" __declspec(dllimport) void __stdcall EnterCriticalSection(void* section);
extern "C" __declspec(dllimport) void __stdcall LeaveCriticalSection(void* section);
extern "C" __declspec(dllimport) unsigned long __stdcall WaitForSingleObject(void* handle, unsigned long milliseconds);   // IAT 0x0143FEAC

struct CSectionLock_00cb1030 {             // scoped critical-section holder
    void* m_pSection;
    CSectionLock_00cb1030(void* section) : m_pSection(section) { EnterCriticalSection(m_pSection); }
    ~CSectionLock_00cb1030() { LeaveCriticalSection(m_pSection); }
};

struct SReaction_00cb1030 {                // 16-byte POD record
    long m_a, m_b, m_c, m_d;
};

struct CReactionQueue_00cb1030 {           // +0xc
    unsigned char _body[0x18];
    SReaction_00cb1030* Pop();             // 0x00CB1900
};

struct CReactionManager_00cb1030 {
    unsigned char _pad[0xc];
    CReactionQueue_00cb1030 m_queue;       // +0xc
    unsigned char m_section[0x18];         // +0x24
    void* m_hEvent;                        // +0x3c
    long m_waiters;                        // +0x40
    void PopReactionQueue(SReaction_00cb1030* out);
};

void CReactionManager_00cb1030::PopReactionQueue(SReaction_00cb1030* out)
{
    SReaction_00cb1030* reaction;
    for (;;) {
        {
            CSectionLock_00cb1030 lock(m_section);
            reaction = m_queue.Pop();
            if (reaction)
                break;
            ++m_waiters;
        }
        WaitForSingleObject(m_hEvent, 0xFFFFFFFF);
    }
    *out = *reaction;
    delete reaction;
}