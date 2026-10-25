// PKCMailbox::Send(const T* item) -- EH-framed, PKCMailbox<SFileReply>: under the mailbox lock (PKCProtectionInstance over the +0 section), 1 when the queue at +0x18 is full (virtual IsFull, slot 3); else the item semaphore at +0x1c is released (@ 0x00C2A150), the item pushed into the queue (virtual Push, slot 1) and 0 returned
// Leaf: ?Send@?$PKCMailbox@USFileReply@@@@QAEJPBUSFileReply@@@Z
// Flags: /EHsc
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C1DC40, 155 bytes, thiscall `ret 4`; built under /EHsc.
// Retail bytes:  push -1 ; push offset __ehhandler ; mov eax,fs:[0] ; push eax ; mov fs:[0],esp ; sub esp,8 ; push esi ; mov esi,ecx ; push esi ; lea ecx,[esp+8] ; call PKCProtectionInstance::PKCProtectionInstance ; mov ecx,[esi+18h] ; mov eax,[ecx] ; mov dword ptr [esp+14h],0 ; call [eax+0Ch] ; test eax,eax ; je push ; lea ecx,[esp+4] ; mov dword ptr [esp+14h],-1 ; call ~PKCProtectionInstance ; mov eax,1 ; pop esi ; ... ret 4 ; push: lea ecx,[esi+1Ch] ; call PKCSemaphore::Release ; mov ecx,[esi+18h] ; mov eax,[item] ; mov edx,[ecx] ; push eax ; call [edx+4] ; ...
// Naming: was landed as ProcessMessage_CMailboxQueue_00c1dc40 (propagated autoname "MailboxQueue_ProcessMessage");
//   byte-identical (relocations aside) to PKCMailbox<SFileRequest>::Send 0x00C1B130. Its caller is the reply slot
//   0x00C1DB60 of the PKCASyncFileReader request (vtable 0x012B32CC), which warns "Cannot reply to the user mailbox"
//   (PKCASyncFileReader.h line 41) when this returns nonzero.
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

class PKCProtectionInstance {                   // scoped guard: section + entry tick
public:
    PKCCriticalSection* m_pSection;
    unsigned long m_tick;
    PKCProtectionInstance(PKCCriticalSection* section);   // 0x00C0FFC0
    ~PKCProtectionInstance();                   // 0x00C0FFE0
};

class PKCSemaphore {                            // a Win32 handle
public:
    void* m_hSemaphore;
    PKCSemaphore(long initialCount, long maximumCount);   // 0x00C29FC0
    ~PKCSemaphore();                            // 0x00C29FB0
    void Wait();                                // 0x00C2A090
    void Release();                             // 0x00C2A150
};

template<class T> class PKCQueue {              // the queue interface PKCMailbox drives (4 pure slots, no destructor slot)
public:
    virtual void Pop(T* item) = 0;              // slot 0
    virtual long Push(const T* item) = 0;       // slot 1
    virtual unsigned long GetCount() = 0;       // slot 2
    virtual long IsFull() = 0;                  // slot 3
};

template<class T> class PKCMailbox {            // 0x20 bytes: a thread-safe front of a queue (libpk PKCMailbox.h)
public:
    PKCCriticalSection m_lock;                  // +0x00
    PKCQueue<T>*       m_pQueue;                // +0x18
    PKCSemaphore       m_available;             // +0x1c: counts the queued items
    PKCMailbox(PKCQueue<T>* queue, long capacity);          // 0x00C1AFD0 / 0x00C1D9D0
    T* Receive(T* item);                        // 0x00C1B030 / 0x00C1DA30: waits for an item, pops it into *item
    long Send(const T* item);                   // 0x00C1B130 / 0x00C1DC40: 1 when the queue is full (the caller retries)
};

template<class T> long PKCMailbox<T>::Send(const T* item)
{
    PKCProtectionInstance lock(&m_lock);
    if (m_pQueue->IsFull())
        return 1;
    m_available.Release();
    m_pQueue->Push(item);
    return 0;
}

template long PKCMailbox<SFileReply>::Send(const SFileReply*);
