// CMailboxQueue_00c26cd0::PostMessage(CMailboxMessage_00c26cd0* message) -- under the scoped queue lock append the message to the pending vector at +0x18, then release the queue's semaphore at +0x28
// Grade: RELOCATION_MATCH (wave-2 sec6): the VC6 Dinkumware two-argument insert(end(), x) (inline: off = P - begin();
//   insert(P, 1, x); return begin() + off) at the call site gives retail's edx temp address and the lea ecx between the
//   count and position pushes
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C26CD0, length 115 bytes. Flags: /EHsc (the lock lives in EH state 0).
// Callees: PKCProtectionInstance::PKCProtectionInstance(queue) @ 0x00C0FFC0 / ~PKCProtectionInstance @ 0x00C0FFE0 (the 8-byte scoped lock of the landed mailbox-queue bodies,
// e.g. 0x00C48BE0), the VC6 vector's insert(position, count, const T&) @ 0x00C14270 (out of line), CSemaphore::Release @ 0x00C2A150.
// Label correction: the manifest's "CEditControlCentre::CreateAndSaveNewLevel" (4 parameters) is a propagated name -- the body takes one
// argument (`ret 4`) and is the post side of a mailbox queue in the 0x00C0..0x00C4 script/thread module; the class is named by address
// (twin @ 0x00C27130: same body, a different element type, so a different insert instantiation).
// Shape: `insert(end(), 1, x)` on the VC6 Dinkumware vector (the empty allocator word at +0x18, _Last at +0x20) written at the call site:
// _Last is read through `this` (esi) before the vector address goes to ecx (a push_back member wrapper reads it through ecx); the argument is a derived-message pointer converted to the vector's base-pointer
// element, a conversion temporary VC homes in the argument's own dead slot (`mov eax,[esp+1Ch] ; lea edx,[esp+1Ch] ; mov [esp+20h],eax`);
// the EH state-0 store is scheduled among the pushes; the semaphore member's Release, then the lock's destructor at state -1.
// Flags: /EHsc
struct CMailboxQueue_00c26cd0;

class PKCCriticalSection;
class PKCProtectionInstance { public:     // libpk scoped lock (trace string '.\\PKCProtectionInstance.cpp'; landed mailbox rows)                         // 8-byte scoped lock over the queue
    CMailboxQueue_00c26cd0* m_pQueue;
    long m_state;
    PKCProtectionInstance(PKCCriticalSection* section);   // 0x00C0FFC0
    ~PKCProtectionInstance();                           // 0x00C0FFE0
};

struct CSemaphore {
    long m_value;
    void Release();                          // 0x00C2A150
};

struct CMailboxMessageBase_00c26cd0 {
    long m_id;
};

struct CMailboxMessage_00c26cd0 : CMailboxMessageBase_00c26cd0 {
};

template <class T>
struct CRetailVectorVC6 {                   // VC6 Dinkumware vector: allocator word, _First, _Last, _End
    unsigned char m_allocator[4];
    T* _First;
    T* _Last;
    T* _End;
    void insert(T* position, unsigned int count, const T& x);   // 0x00C14270
    T* begin() { return _First; }
    T* end() { return _Last; }
    T* insert(T* position, const T& x) { unsigned int off = position - begin(); insert(position, 1, x); return begin() + off; }
    void push_back(const T& x) { insert(end(), x); }
};

struct CMailboxQueue_00c26cd0 {
    unsigned char _pad[0x18];
    CRetailVectorVC6<CMailboxMessageBase_00c26cd0*> m_pending;   // +0x18
    CSemaphore m_semaphore;                  // +0x28
    void PostMessage(CMailboxMessage_00c26cd0* message);
};

void CMailboxQueue_00c26cd0::PostMessage(CMailboxMessage_00c26cd0* message)
{
    PKCProtectionInstance lock((PKCCriticalSection*)this);
    m_pending.insert(m_pending.end(), message);
    m_semaphore.Release();
}