// CMailboxQueue_00c27130::PostMessage(CMailboxMessage_00c27130* message) -- under the scoped queue lock append the message to the pending vector at +0x18, then release the queue's semaphore at +0x28
// Grade: DIFFER(115v115) -- register/scheduling only: the conversion temporary's address is formed in ecx (`lea ecx,[esp+1Ch] ; push ecx`) where retail uses edx, and the vector's `lea ecx,[esi+18h]` follows both the count and position pushes where retail puts it between them (`push 1 ; lea ecx,[esi+18h] ; push eax`). Tried: push_back member wrapper (_Last read through ecx), insert(end(), 1, x) at the call site (this one).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C27130, length 115 bytes. Flags: /EHsc (the lock lives in EH state 0).
// Callees: CQueueLock::CQueueLock(queue) @ 0x00C0FFC0 / ~CQueueLock @ 0x00C0FFE0 (the 8-byte scoped lock of the landed mailbox-queue bodies,
// e.g. 0x00C48BE0), the VC6 vector's insert(position, count, const T&) @ 0x00C14080 (out of line), CSemaphore::Release @ 0x00C2A150.
// Label correction: the manifest's "CEditControlCentre::CreateAndSaveNewLevel" (4 parameters) is a propagated name -- the body takes one
// argument (`ret 4`) and is the post side of a mailbox queue in the 0x00C0..0x00C4 script/thread module; the class is named by address
// (twin @ 0x00C26CD0: same body, a different element type, so a different insert instantiation).
// Shape: `insert(end(), 1, x)` on the VC6 Dinkumware vector (the empty allocator word at +0x18, _Last at +0x20) written at the call site:
// _Last is read through `this` (esi) before the vector address goes to ecx (a push_back member wrapper reads it through ecx); the argument is a derived-message pointer converted to the vector's base-pointer
// element, a conversion temporary VC homes in the argument's own dead slot (`mov eax,[esp+1Ch] ; lea edx,[esp+1Ch] ; mov [esp+20h],eax`);
// the EH state-0 store is scheduled among the pushes; the semaphore member's Release, then the lock's destructor at state -1.
// Flags: /EHsc
struct CMailboxQueue_00c27130;

struct CQueueLock {                         // 8-byte scoped lock over the queue
    CMailboxQueue_00c27130* m_pQueue;
    long m_state;
    CQueueLock(CMailboxQueue_00c27130* queue);   // 0x00C0FFC0
    ~CQueueLock();                           // 0x00C0FFE0
};

struct CSemaphore {
    long m_value;
    void Release();                          // 0x00C2A150
};

struct CMailboxMessageBase_00c27130 {
    long m_id;
};

struct CMailboxMessage_00c27130 : CMailboxMessageBase_00c27130 {
};

template <class T>
struct CRetailVectorVC6 {                   // VC6 Dinkumware vector: allocator word, _First, _Last, _End
    unsigned char m_allocator[4];
    T* _First;
    T* _Last;
    T* _End;
    void insert(T* position, unsigned int count, const T& x);   // 0x00C14080
    T* end() { return _Last; }
};

struct CMailboxQueue_00c27130 {
    unsigned char _pad[0x18];
    CRetailVectorVC6<CMailboxMessageBase_00c27130*> m_pending;   // +0x18
    CSemaphore m_semaphore;                  // +0x28
    void PostMessage(CMailboxMessage_00c27130* message);
};

void CMailboxQueue_00c27130::PostMessage(CMailboxMessage_00c27130* message)
{
    CQueueLock lock(this);
    m_pending.insert(m_pending.end(), 1, message);
    m_semaphore.Release();
}
