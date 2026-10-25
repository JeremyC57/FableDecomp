// CRequestQueue::~CRequestQueue() -- EH-framed destructor: the PKCMailbox at +0x14 destroyed inline (its semaphore at +0x30 @ 0x00C29FB0 at state 1, its critical section at +0x14 @ 0x00C1C820 at state 0), then the PKCStaticQueue at +0 (@ 0x00C1AEA0)
// Leaf: ??1CRequestQueue@@QAE@XZ
// Flags: /EHsc
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C1B2A0, 104 bytes, thiscall; built under /EHsc.
// Retail bytes:  push -1 ; push offset __ehhandler ; mov eax,fs:[0] ; push eax ; mov fs:[0],esp ; sub esp,8 ; push esi ; mov esi,ecx ; push edi ; mov [esp+8],esi ; lea edi,[esi+14h] ; mov dword ptr [esp+18h],0 ; mov [esp+0Ch],edi ; lea ecx,[edi+1Ch] ; mov byte ptr [esp+18h],1 ; call ~PKCSemaphore ; mov ecx,edi ; mov byte ptr [esp+18h],0 ; call ~PKCCriticalSection ; mov ecx,esi ; mov dword ptr [esp+18h],-1 ; call ~PKCStaticQueue ; mov ecx,[esp+0Ch] ; pop edi ; pop esi ; mov fs:[0],ecx ; add esp,14h ; ret
// Identity: ~PKCDefaultFileManager 0x00C1A230 calls it on its +0x44 member (state 1 -> 0); the mailbox's implicit
//   destructor is inlined (no vtable store: neither CRequestQueue nor PKCMailbox is polymorphic; the queue member is).
// Naming (sweep1, 2026-10-25): the PKCDefaultFileManager request queue family -- CRequestQueue (descriptive, 0x34
//   bytes: a PKCStaticQueue and the PKCMailbox over it) and PKCMailbox<T> (0x20 bytes: critical section, queue pointer
//   +0x18, semaphore +0x1c; its Receive 0x00C1B030 warns from "d:\dev\audio\ver_fable_1.5_b\libpk\PKCMailbox.h" line 63,
//   hence the name; a template because 0x00C49B90 is the same Send over another queue). PKCStaticQueue (Pop trace names
//   PKCStaticQueue.h), PKCSemaphore (".\PKCSemaphore.cpp"), PKCProtectionInstance (".\PKCProtectionInstance.cpp") and
//   the descriptive PKCCriticalSection are the sweep's lock family. The PKCDefaultFileManager constructor 0x00C1AC40
//   inlines CRequestQueue(0x28) for its +0x44 member; ~PKCDefaultFileManager 0x00C1A230 sends its quit request through
//   the mailbox at +0x58. No PDB covers libpk.

// Label correction: the landed row was CScriptNode_00c1b2a0::~CScriptNode_00c1b2a0() over a "CCharString" pair member (catalog "_Dest_val<...CSymbolInfo...>", propagated).
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

CRequestQueue::~CRequestQueue()
{
}
