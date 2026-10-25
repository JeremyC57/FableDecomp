// PKCDefaultFileManager::PKCDefaultFileManager(void* owner) -- EH-framed constructor mirroring the landed destructor 0x00C1A230 (vtable 0x012B30E4; the manifest's "FileManager_CreateHandler"): the +4 critical section (@ 0x00C1C810; state 1), the node allocator at +0x1c (vtable 0x012B30EC) handed to the +0x20 PKCTRRedblackMap (@ 0x00C1AE20), the request queue at +0x44 (root @ 0x00C1B340 with capacity 0x28, its address homed on the frame for the root unwind @ 0x00C1AEA0 (state 2), the node @ 0x00C1AFD0 over the root; state 3), the PKCSemaphore at +0x78 (@ 0x00C29FC0 with (0, 1); state 4), +0x80 cleared and the owner at +0x84; the body creates the worker thread (`CreateThread`, 0x3000-byte stack, procedure 0x00C1AC30 over `this`) into +0x7c, raising the "Could not create thread" warning (PKCDefaultFileManager.cpp line 234, assert hook at 0x0143BB2C, CDebugBreak on the string's slot; state 5) when it fails, and sleeps 20 ms
// Leaf: ??0PKCDefaultFileManager@@QAE@PAX@Z
// Flags: /EHsc
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C1AC40, oracle length 337 bytes. Built under /EHsc.
// Retail bytes:  ... sub esp,8 ; push esi ; mov esi,ecx ; push edi ; mov [esp+8],esi ; lea ecx,[esi+4] ; mov dword ptr [esp+18h],0 ; mov dword ptr [esi],12B30E4h ; call PKCCriticalSection::PKCCriticalSection ; lea eax,[esi+1Ch] ; push eax ; lea ecx,[esi+20h] ; mov byte ptr [esp+1Ch],1 ; mov dword ptr [eax],12B30ECh ; call PKCTRRedblackMap::PKCTRRedblackMap ; lea edi,[esi+44h] ; push 28h ; mov ecx,edi ; mov [esp+10h],edi ; call PKCStaticQueue::PKCStaticQueue ; push 28h ; push edi ; lea ecx,[edi+14h] ; mov byte ptr [esp+20h],2 ; call PKCMailbox::PKCMailbox ; push 1 ; push 0 ; lea ecx,[esi+78h] ; mov byte ptr [esp+20h],3 ; call PKCSemaphore::PKCSemaphore ; mov eax,[esp+20h] ; push 0 ; push 0 ; push esi ; push offset ThreadProc ; push 3000h ; push 0 ; mov byte ptr [esp+30h],4 ; mov dword ptr [esi+80h],0 ; mov [esi+84h],eax ; call [__imp_CreateThread] ; test eax,eax ; mov [esi+7Ch],eax ; jne ok ; mov ecx,[_afxPchNil] ; mov [esp+20h],ecx ; mov edx,offset ".\PKCDefaultFileManager.cpp" ; lea ecx,[esp+20h] ; mov byte ptr [esp+18h],5 ; call operator<< ; ... mov edx,0EAh ; ... call operator<<Number ; ... call operator<< ("Could not create thread") ; ... call operator<< ("\n") ; mov eax,[g_pfnAssertHook] ; test eax,eax ; je skip ; mov ecx,[esp+20h] ; call eax ; skip: lea ecx,[esp+20h] ; mov byte ptr [esp+18h],4 ; call ~CString ; lea ecx,[esp+20h] ; call CDebugBreak::CDebugBreak ; ok: push 14h ; call [__imp_Sleep] ; ... mov eax,esi ; ... ret 4
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
// Label correction: the landed row was CFileManager_00c1ac40 over address-named members; the catalog's
//   "FileManager_CreateHandler" is descriptive.
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

inline CRequestQueue::CRequestQueue(long capacity)  // out-of-line copy 0x00C1B6D0; inlined here
    : m_queue(capacity), m_mailbox(&m_queue, capacity)
{
}

PKCDefaultFileManager::PKCDefaultFileManager(void* owner)
    : m_lock(), m_allocator(), m_files(&m_allocator), m_requests(0x28), m_done(0, 1), m_80(0), m_pOwner(owner)
{
    m_hThread = CreateThread(0, 0x3000, ThreadProc, this, 0, 0);
    if (!m_hThread) {
        {
            CString line;
            line << ".\\PKCDefaultFileManager.cpp" << "(" << (long)234 << ") : " << "Could not create thread" << "\n";
            if (g_pfnAssertHook)
                g_pfnAssertHook(line.m_pchData);
        }
        CDebugBreak();
    }
    Sleep(20);
}
