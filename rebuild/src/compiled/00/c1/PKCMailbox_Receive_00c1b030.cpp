// PKCMailbox::Receive(T* item) -- EH-framed, PKCMailbox<SFileRequest>: waits on the item semaphore at +0x1c (@ 0x00C2A090), then under the mailbox lock (PKCProtectionInstance over the +0 section) warns and breaks unless the queue at +0x18 holds an item (virtual GetCount, slot 2), pops it into *item (virtual Pop, slot 0) and returns item
// Leaf: ?Receive@?$PKCMailbox@USFileRequest@@@@QAEPAUSFileRequest@@PAU2@@Z
// Flags: /EHsc
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C1B030, 241 bytes, thiscall `ret 4`, returns its argument; built under /EHsc.
// Retail bytes:  (EH prologue) ; sub esp,10h ; push esi ; mov esi,ecx ; lea ecx,[esi+1Ch] ; call PKCSemaphore::Wait ; push esi ; lea ecx,[esp+10h] ; call PKCProtectionInstance::PKCProtectionInstance ; mov ecx,[esi+18h] ; mov eax,[ecx] ; mov dword ptr [esp+1Ch],0 ; call [eax+8] ; test eax,eax ; ja pop ; (CString warning ".PKCMailbox.h(63) : the queue is out of step with the mailbox handler" through operator<< 0x00C0DEC0/0x00C0DF00, the hook 0x0143BB2C, ~CString 0x00C0BA40, CDebugBreak 0x00C16750) ; pop: mov ecx,[esi+18h] ; mov esi,[item] ; mov edx,[ecx] ; push esi ; call [edx] ; lea ecx,[esp+0Ch] ; mov dword ptr [esp+1Ch],-1 ; call ~PKCProtectionInstance ; ... mov eax,esi ; ... ret 4
// Naming: was landed as CMailboxQueue_00c1b030::DispatchMessage_CMailboxQueue_00c1b030 (a propagated autoname over address-named classes CMailbox / CQueueLock /
//   CSemaphore / CDebugStream); the body is the one that names libpk's PKCMailbox.h (line 63). The `ja` is an unsigned
//   `GetCount() > 0`. Its byte-identical twin is PKCMailbox<SFileReply>::Receive 0x00C1DA30 (relocations aside). 

extern char* _afxPchNil;                        // 0x0139C900
typedef void (__fastcall* PFNASSERTHOOK)(const char* message);
extern PFNASSERTHOOK g_pfnAssertHook;           // 0x0143BB2C

class CString {                                 // the libpk MFC-style string: one word
public:
    char* m_pchData;
    CString() : m_pchData(_afxPchNil) { }
    ~CString();                                 // 0x00C0BA40
    CString& __fastcall operator<<(const char* text);   // 0x00C0DEC0
    CString& __fastcall operator<<(long value);         // 0x00C0DF00
};

struct CDebugBreak {                            // empty temporary whose constructor breaks into the debugger
    CDebugBreak();                              // 0x00C16750
};

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

template<class T> class PKCMailbox {            // 0x20 bytes: a thread-safe front of a queue (libpk PKCMailbox.h)
public:
    PKCCriticalSection m_lock;                  // +0x00
    PKCQueue<T>*       m_pQueue;                // +0x18
    PKCSemaphore       m_available;             // +0x1c: counts the queued items
    PKCMailbox(PKCQueue<T>* queue, long capacity);          // 0x00C1AFD0 / 0x00C1D9D0
    T* Receive(T* item);                        // 0x00C1B030 / 0x00C1DA30: waits for an item, pops it into *item
    long Send(const T* item);                   // 0x00C1B130 / 0x00C1DC40: 1 when the queue is full (the caller retries)
};

template<class T> T* PKCMailbox<T>::Receive(T* item)
{
    m_available.Wait();
    PKCProtectionInstance lock(&m_lock);
    if (!(m_pQueue->GetCount() > 0)) {
        {
            CString line;
            line << "d:\\dev\\audio\\ver_fable_1.5_b\\libpk\\PKCMailbox.h" << "(" << (long)63 << ") : " << "the queue is out of step with the mailbox handler" << "\n";
            if (g_pfnAssertHook)
                g_pfnAssertHook(line.m_pchData);
        }
        CDebugBreak();
    }
    m_pQueue->Pop(item);
    return item;
}

template SFileRequest* PKCMailbox<SFileRequest>::Receive(SFileRequest*);
