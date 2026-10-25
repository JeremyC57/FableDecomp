// PKCDefaultFileManager::~PKCDefaultFileManager() -- EH-framed destructor (vtable 0x012B30E4; the manifest's "FileManager_ThreadShutdown"): a quit request (seven words, the key -1) is submitted to the request queue's mailbox at +0x58 (PKCMailbox::Send @ 0x00C1B130) until it is taken (result 1 = retry), each retry sleeping 100 ms and raising the "Wait to send quit command..." warning (PKCDefaultFileManager.cpp line 247: an MFC-style string in a nested block handed to the assert hook at 0x0143BB2C; state 4 covers it, no trace); the PKCSemaphore at +0x78 is waited on (@ 0x00C2A090) and the thread handle at +0x7c closed; the compiler then destroys the semaphore (@ 0x00C29FB0; state 2), the +0x44 request queue (@ 0x00C1B2A0; state 1), the +4 critical section (@ 0x00C1C820; state 0) and stores the base vtable 0x012B2FB8 (inline empty base destructor, 0x00C19F60 on unwind)
// Leaf: ??1PKCDefaultFileManager@@UAE@XZ
// Flags: /EHsc
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C1A230, oracle length 310 bytes. Built under /EHsc.
// Retail bytes:  ... sub esp,24h ; push ebp ; push esi ; push edi ; mov edi,ecx ; mov [esp+10h],edi ; mov dword ptr [edi],12B30E4h ; lea eax,[esp+14h] ; lea ebp,[edi+58h] ; push eax ; mov ecx,ebp ; mov dword ptr [esp+3Ch],3 ; mov dword ptr [esp+1Ch],-1 ; call PKCMailbox::Send ; cmp eax,1 ; jne done ; retry: mov esi,[__imp_Sleep] ; push 64h ; call esi ; mov ecx,[_afxPchNil] ; mov [esp+0Ch],ecx ; mov edx,offset ".\PKCDefaultFileManager.cpp" ; lea ecx,[esp+0Ch] ; mov byte ptr [esp+38h],4 ; call operator<< ; ... mov edx,0F7h ; ... call operator<<Number ; ... call operator<< ("Wait to send quit command...") ; ... call operator<< ("\n") ; mov eax,[g_pfnAssertHook] ; test eax,eax ; je skip ; mov ecx,[esp+0Ch] ; call eax ; skip: lea ecx,[esp+0Ch] ; mov byte ptr [esp+38h],3 ; call ~CString ; lea edx,[esp+14h] ; push edx ; mov ecx,ebp ; call PKCMailbox::Send ; cmp eax,1 ; je retry ; done: lea esi,[edi+78h] ; mov ecx,esi ; call PKCSemaphore::Wait ; mov eax,[edi+7Ch] ; push eax ; call [__imp_CloseHandle] ; mov ecx,esi ; mov byte ptr [esp+38h],2 ; call PKCSemaphore::~PKCSemaphore ; lea ecx,[edi+44h] ; mov byte ptr [esp+38h],1 ; call CRequestQueue::~CRequestQueue ; lea ecx,[edi+4] ; mov byte ptr [esp+38h],0 ; call PKCCriticalSection::~PKCCriticalSection ; mov ecx,[esp+30h] ; mov dword ptr [edi],12B2FB8h ; ... ret
// Naming (sweep1, 2026-10-25): one model for the constructor 0x00C1AC40 and the destructor 0x00C1A230, which the
//   landed rows wrote over different layouts (the constructor's +4 handle table / +0x1c sink / +0x20 dispatcher and
//   the destructor's 0x40-byte "handle table" at +4 are one layout): +0x04 a critical section (0x00C1C810 / 0x00C1C820
//   wrap the InitializeCriticalSection / DeleteCriticalSection imports), +0x1c the map's node allocator (vtable
//   0x012B30EC: slot 0 0x00C1B320 operator delete, slot 1 0x00C1B330 operator new(0x18) = one map node), +0x20 the
//   PKCTRRedblackMap (0x00C1AE20 builds its sentinel, root +0x1c, last-found +0x20; FINDINGS chunk 433), +0x44 the
//   request queue (CRequestQueue: PKCStaticQueue 0x00C1B340 / 0x00C1AEA0 and the PKCMailbox at +0x58 the destructor sends to), +0x78
//   the landed PKCSemaphore (0x00C29FC0 / Wait 0x00C2A090; 0x00C29FB0 closes its handle). The class names come from
//   the strings ".\PKCDefaultFileManager.cpp" (the warnings) and "PKCStaticQueue.h"; the base (vtable 0x012B2FB8, pure
//   slots) is named descriptively PKCFileManager; no PDB covers libpk.
// Label correction: the landed row was CFileManager_00c1a230 over address-named members; the catalog's
//   "FileManager_ThreadShutdown" is descriptive.
extern "C" __declspec(dllimport) void __stdcall Sleep(unsigned long milliseconds);
extern "C" __declspec(dllimport) int __stdcall CloseHandle(void* handle);
extern "C" __declspec(dllimport) void* __stdcall CreateThread(void* attributes, unsigned long stackSize, unsigned long (__stdcall* procedure)(void*), void* parameter, unsigned long flags, unsigned long* threadId);
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

class CMapNodeAllocator {                       // vtable 0x012B30EC: the map's heap node allocator
public:
    virtual void Free(void* node);              // slot 0 (0x00C1B320: operator delete)
    virtual void* Allocate();                   // slot 1 (0x00C1B330: operator new(0x18))
};

struct PKCTRRedblackNode {                      // 0x18 bytes
    PKCTRRedblackNode* Left;                    // +0x00
    PKCTRRedblackNode* Right;                   // +0x04
    PKCTRRedblackNode* Parent;                  // +0x08
    long               Colour;                  // +0x0c
    unsigned long      Key;                     // +0x10
    void*              Value;                   // +0x14
};

class PKCTRRedblackMap {                        // 0x24 bytes
public:
    CMapNodeAllocator* PAllocator;              // +0x00
    PKCTRRedblackNode  Sentinel;                // +0x04
    PKCTRRedblackNode* Root;                    // +0x1c
    PKCTRRedblackNode* LastFound;               // +0x20
    PKCTRRedblackMap(CMapNodeAllocator* allocator);   // 0x00C1AE20
};

class PKCFileManager {                          // vtable 0x012B2FB8: the abstract file-manager interface
public:
    virtual ~PKCFileManager() { }
};

class PKCDefaultFileManager : public PKCFileManager {   // vtable 0x012B30E4
public:
    PKCCriticalSection m_lock;                  // +0x04
    CMapNodeAllocator  m_allocator;             // +0x1c
    PKCTRRedblackMap   m_files;                 // +0x20
    CRequestQueue      m_requests;              // +0x44 (its mailbox at +0x58)
    PKCSemaphore       m_done;                  // +0x78
    void*              m_hThread;               // +0x7c
    long               m_80;
    void*              m_pOwner;                // +0x84
    static unsigned long __stdcall ThreadProc(void* manager);   // 0x00C1AC30
    PKCDefaultFileManager(void* owner);
    virtual ~PKCDefaultFileManager();
};

PKCDefaultFileManager::~PKCDefaultFileManager()
{
    SFileRequest quit;
    quit.m_pKey = (void*)-1;
    while (m_requests.m_mailbox.Send(&quit) == 1) {
        Sleep(100);
        {
            CString line;
            line << ".\\PKCDefaultFileManager.cpp" << "(" << (long)247 << ") : " << "Wait to send quit command..." << "\n";
            if (g_pfnAssertHook)
                g_pfnAssertHook(line.m_pchData);
        }
    }
    m_done.Wait();
    CloseHandle(m_hThread);
}
