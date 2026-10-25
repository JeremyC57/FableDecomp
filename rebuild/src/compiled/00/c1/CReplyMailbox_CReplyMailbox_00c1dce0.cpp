// CReplyMailbox::~CReplyMailbox() -- EH-framed destructor of the reply channel: the mailbox's implicit destructor is inlined (its semaphore at +0x10+0x1c @ 0x00C29FB0 under EH state 0, then its critical section at +0x10 @ 0x00C1C820); the one-shot queue at +0 is trivial
// Leaf: ??1CReplyMailbox@@QAE@XZ
// Flags: /EHsc
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C1DCE0, 77 bytes, __thiscall `ret`; built under /EHsc.
// Retail bytes:  (EH prologue) ; push ecx ; push esi ; lea esi,[ecx+0x10] ; mov [esp+4],esi ; lea ecx,[esi+0x1c] ; mov dword ptr [esp+10h],0 ; call ~PKCSemaphore ; mov ecx,esi ; mov dword ptr [esp+10h],-1 ; call ~PKCCriticalSection ; mov ecx,[esp+8] ; pop esi ; mov fs:[0],ecx ; add esp,10h ; ret
// Naming: was landed as CPair_00c1dce0::~CPair_00c1dce0 over address-named classes (catalog "_Dest_val<...CScriptInfo...>",
//   propagated); FINDINGS chunk 437 ("0xC1DCE0 = sync-wait object dtor"). Callers: 0x00C1DFF3 / 0x00C1E02A (both exits
//   of 0x00C1DF70), 0x00C320DE / 0x00C32115, and two unwind funclets.
// Family (sweep1, 2026-10-25): libpk's synchronous reply channel in the PKCASyncFileReader TU. CReplyMailbox (descriptive,
//   0x30 bytes, non-polymorphic) = a PKCOneShotQueue<SFileReply> at +0 (vtable 0x012B332C = Pop 0x00C1DD70 / Push
//   0x00C1DE50, whose traces name "d:\dev\audio\ver_fable_1.5_b\libpk\PKCOneShotQueue.h" with "Nothing to return!" /
//   "Already filled this queue!", GetCount 0x00C1DF30, IsFull 0x00C1DF40) and the PKCMailbox<SFileReply> over it at
//   +0x10 (capacity 1). The caller 0x00C1DF70 builds one on the stack, hands &m_mailbox to a request (vtable 0x012B32CC,
//   whose Reply 0x00C1DB60 sends an 8-byte SFileReply {bool, long} and warns "Cannot reply to the user mailbox" from
//   PKCASyncFileReader.h) and blocks in the mailbox Receive 0x00C1DA30. PKCOneShotQueue.h / PKCMailbox.h /
//   PKCStaticQueue.h traces recur in 6 / 7+ / 4 bodies, so the three are templates over the item type; PKCQueue<T> is
//   the descriptive name of their 4-pure-slot interface (an all-_purecall vtable 0x012B32BC sits in this TU). No PDB
//   covers libpk. PKCCriticalSection 0x00C1C810/0x00C1C820 and PKCSemaphore 0x00C29FC0/0x00C29FB0 are the sweep's
//   lock family (as in PKCMailbox<SFileRequest> 0x00C1AFD0).

struct SFileReply {                             // 8 bytes: the answer a PKCASyncFileReader request posts back
    bool m_bSuccess;                            // +0x00
    long m_value;                               // +0x04
    SFileReply() : m_bSuccess(true), m_value(0) {}
};

class PKCCriticalSection {                      // a Win32 CRITICAL_SECTION (0x18 bytes)
public:
    unsigned char m_section[0x18];
    PKCCriticalSection();                       // 0x00C1C810
    ~PKCCriticalSection();                      // 0x00C1C820
};

class PKCSemaphore {                            // a Win32 handle
public:
    void* m_hSemaphore;
    PKCSemaphore(long initialCount, long maximumCount);   // 0x00C29FC0
    ~PKCSemaphore();                            // 0x00C29FB0
};

template<class T> class PKCQueue {              // the queue interface PKCMailbox drives (4 pure slots, no destructor slot)
public:
    virtual void Pop(T* item) = 0;              // slot 0
    virtual long Push(const T* item) = 0;       // slot 1
    virtual unsigned long GetCount() = 0;       // slot 2
    virtual long IsFull() = 0;                  // slot 3
};

template<class T> class PKCOneShotQueue : public PKCQueue<T> {   // 0x10 bytes (libpk PKCOneShotQueue.h)
public:
    T    m_item;                                // +0x04
    long m_bFilled;                             // +0x0c
    PKCOneShotQueue();                          // 0x00C1DD50 (vtable 0x012B332C)
    virtual void Pop(T* item);                  // slot 0 (0x00C1DD70)
    virtual long Push(const T* item);           // slot 1 (0x00C1DE50)
    virtual unsigned long GetCount();           // slot 2 (0x00C1DF30)
    virtual long IsFull();                      // slot 3 (0x00C1DF40)
};

template<class T> class PKCMailbox {            // 0x20 bytes: a thread-safe front of a queue (libpk PKCMailbox.h)
public:
    PKCCriticalSection m_lock;                  // +0x00
    PKCQueue<T>*       m_pQueue;                // +0x18
    PKCSemaphore       m_available;             // +0x1c: counts the queued items
    PKCMailbox(PKCQueue<T>* queue, long capacity);          // 0x00C1D9D0 for SFileReply
    T* Receive(T* item);                        // 0x00C1DA30 for SFileReply
    long Send(const T* item);                   // 0x00C1DC40 for SFileReply
};

class CReplyMailbox {                           // 0x30 bytes: a one-item reply queue and the mailbox over it
public:
    PKCOneShotQueue<SFileReply> m_queue;        // +0x00
    PKCMailbox<SFileReply>      m_mailbox;      // +0x10
    CReplyMailbox();                            // 0x00C1DF50
    ~CReplyMailbox();                           // 0x00C1DCE0
};

CReplyMailbox::~CReplyMailbox()
{
}
