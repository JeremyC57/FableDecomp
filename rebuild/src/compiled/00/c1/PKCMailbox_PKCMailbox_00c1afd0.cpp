// PKCMailbox::PKCMailbox(PKCStaticQueue* queue, long capacity) -- EH-framed constructor of PKCMailbox<SFileRequest>: the critical section at +0 (@ 0x00C1C810; EH state 0 once built), the queue pointer at +0x18, the item-count semaphore at +0x1c created with (0, capacity) (@ 0x00C29FC0)
// Leaf: ??0?$PKCMailbox@USFileRequest@@@@QAE@PAV?$PKCQueue@USFileRequest@@@@J@Z
// Flags: /EHsc
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C1AFD0, 84 bytes, thiscall `ret 8`, returns `this`; built under /EHsc.
// Retail bytes:  (EH prologue) ; push ecx ; push esi ; mov esi,ecx ; mov [esp+4],esi ; call PKCCriticalSection::PKCCriticalSection ; mov ecx,[esp+1Ch] ; mov eax,[esp+18h] ; push ecx ; push 0 ; lea ecx,[esi+0x1c] ; mov dword ptr [esp+18h],0 ; mov [esi+0x18],eax ; call PKCSemaphore::PKCSemaphore ; mov ecx,[esp+8] ; mov eax,esi ; pop esi ; mov fs:[0],ecx ; add esp,10h ; ret 8
// Identity: called by the CRequestQueue constructor 0x00C1B6D0 and inlined CRequestQueue(0x28) of the
//   PKCDefaultFileManager constructor 0x00C1AC40 (both pass the queue at +0 and the capacity).
// Naming (sweep1, 2026-10-25): the PKCDefaultFileManager request queue family -- CRequestQueue (descriptive, 0x34
//   bytes: a PKCStaticQueue and the PKCMailbox over it) and PKCMailbox<T> (0x20 bytes: critical section, queue pointer
//   +0x18, semaphore +0x1c; its Receive 0x00C1B030 warns from "d:\dev\audio\ver_fable_1.5_b\libpk\PKCMailbox.h" line 63,
//   hence the name; a template because 0x00C49B90 is the same Send over another queue). PKCStaticQueue (Pop trace names
//   PKCStaticQueue.h), PKCSemaphore (".\PKCSemaphore.cpp"), PKCProtectionInstance (".\PKCProtectionInstance.cpp") and
//   the descriptive PKCCriticalSection are the sweep's lock family. The PKCDefaultFileManager constructor 0x00C1AC40
//   inlines CRequestQueue(0x28) for its +0x44 member; ~PKCDefaultFileManager 0x00C1A230 sends its quit request through
//   the mailbox at +0x58. No PDB covers libpk.

// Label correction: the landed row was CScriptEntry_00c1afd0::CScriptEntry_00c1afd0(void*, void*) over an address-named base (catalog "_Dest_val<...CScriptInfo...>", propagated).
struct SFileRequest {                           // seven words: handle, key and five qualifiers
    void* m_hFile;
    void* m_pKey;
    long m_a;
    long m_b;
    long m_c;
    long m_d;
    long m_e;
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

template<class T> class PKCStaticQueue : public PKCQueue<T> {   // 0x14 bytes (libpk PKCStaticQueue.h); vtable 0x012B2FC8 for SFileRequest
public:
    PKCStaticQueue(long capacity);              // 0x00C1B340
    virtual void Pop(T* item);                  // slot 0 (0x00C1B370)
    virtual long Push(const T* item);           // slot 1 (0x00C1AEC0)
    virtual unsigned long GetCount();           // slot 2 (0x00C1AFB0)
    virtual long IsFull();                      // slot 3 (0x00C1AFC0)
    long m_capacity;                            // +0x04
    T*   m_pRecords;                            // +0x08
    long m_count;                               // +0x0c
    long m_head;                                // +0x10
    ~PKCStaticQueue();                          // 0x00C1AEA0 (non-virtual: no destructor slot)
};

template<class T> class PKCMailbox {            // 0x20 bytes: a thread-safe front of a queue (libpk PKCMailbox.h)
public:
    PKCCriticalSection m_lock;                  // +0x00
    PKCQueue<T>*       m_pQueue;                // +0x18
    PKCSemaphore       m_available;             // +0x1c: counts the queued items
    PKCMailbox(PKCQueue<T>* queue, long capacity);          // 0x00C1AFD0
    T* Receive(T* item);                        // 0x00C1B030
    long Send(const T* item);                   // 0x00C1B130: 1 when the queue is full (the caller retries)
};

class CRequestQueue {                           // 0x34 bytes: the static queue and its mailbox
public:
    PKCStaticQueue<SFileRequest> m_queue;           // +0x00
    PKCMailbox<SFileRequest> m_mailbox;         // +0x14
    CRequestQueue(long capacity);               // 0x00C1B6D0
    ~CRequestQueue();                           // 0x00C1B2A0
};

template<class T> PKCMailbox<T>::PKCMailbox(PKCQueue<T>* queue, long capacity)
    : m_lock(), m_pQueue(queue), m_available(0, capacity)
{
}

template PKCMailbox<SFileRequest>::PKCMailbox(PKCQueue<SFileRequest>*, long);
