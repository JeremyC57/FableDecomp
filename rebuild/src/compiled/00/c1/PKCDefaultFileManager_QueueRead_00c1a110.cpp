// PKCDefaultFileManager::QueueRead(unsigned long key, unsigned long size, void* a, void* b, void* c, void* d) -- under the manager's lock look the open file up by key (false when unknown), take its handle and count one more pending request on it, then post the seven-word request {handle, key, size, a, b, c, d} to the request mailbox; when the post fails, look the file up again under the lock, drop the pending count and return false; true when posted
// Leaf: ?QueueRead@PKCDefaultFileManager@@QAE_NKKPAX000@Z
// Flags: /EHsc
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C1A110, 283 bytes (+ int3 padding), thiscall `ret 18h`, EH-framed.
// Identity: the libpk default file manager (vtable 0x012B30E4; the landed sweep1 model of its destructor 0x00C1A230 and
//   constructor 0x00C1AC40): the critical section +4 taken by PKCProtectionInstance (0x00C0FFC0 / 0x00C0FFE0), the
//   open-file map +0x20 (PKCTRRedblackMap::Find 0x00C1AE50: 0 on a hit; the key passed by the address of its own argument
//   slot), and the request mailbox +0x58 (PKCMailbox<SFileRequest>::Send 0x00C1B130, nonzero when the queue is full) that
//   the destructor posts its quit request (key -1 in the second word) to. The file record keeps its Win32 handle at +0
//   (GetFileSize 0x00C1A070 reads it) and the pending-request count at +4. Only caller: slot 1 of the file handle vtable
//   0x012B4F4C (0x00C32F60, landed), which binds this address as `CFileManager_00c1a070::QueueRead`. The method name is
//   that caller's (descriptive: libpk has no PDB or RTTI); the class follows the sweep1 PKCDefaultFileManager model.
// Shape (the GetFileSize 0x00C1A070 lesson): Find is compiled in the same TU as a nothrow body under auto_inline(off),
//   so the first lock gets no `state = 0` store (only the -1 store before the throwing Send) and the second lock no EH
//   state at all; the second lookup's file pointer reuses the dead `size` argument slot.
// Label correction: the catalog's "FileManager_IncrementAndCheckReference" is descriptive.

struct SFileRequest {                           // seven words: handle, key and five qualifiers
    void*         m_hFile;                      // +0x00
    unsigned long m_key;                        // +0x04 (-1 = the destructor's quit request)
    unsigned long m_size;                       // +0x08
    void*         m_a;                          // +0x0c
    void*         m_b;                          // +0x10
    void*         m_c;                          // +0x14
    void*         m_d;                          // +0x18
};

struct SOpenFile {                              // the open-file record (map value)
    void* m_hFile;                              // +0x00 Win32 handle
    long  m_pendingRequests;                    // +0x04
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
    ~PKCSemaphore();                            // 0x00C29FB0
};

template<class T> class PKCQueue {              // the queue interface PKCMailbox drives (4 pure slots, no destructor slot)
public:
    virtual void Pop(T* item) = 0;              // slot 0
    virtual long Push(const T* item) = 0;       // slot 1
    virtual unsigned long GetCount() = 0;       // slot 2
    virtual long IsFull() = 0;                  // slot 3
};

template<class T> class PKCStaticQueue : public PKCQueue<T> {   // 0x14 bytes (libpk PKCStaticQueue.h)
public:
    virtual void Pop(T* item);                  // slot 0 (0x00C1B370)
    virtual long Push(const T* item);           // slot 1 (0x00C1AEC0)
    virtual unsigned long GetCount();           // slot 2 (0x00C1AFB0)
    virtual long IsFull();                      // slot 3 (0x00C1AFC0)
    long m_capacity;                            // +0x04
    T*   m_pRecords;                            // +0x08
    long m_count;                               // +0x0c
    long m_head;                                // +0x10
    ~PKCStaticQueue();                          // 0x00C1AEA0
};

template<class T> class PKCMailbox {            // 0x20 bytes: a thread-safe front of a queue (libpk PKCMailbox.h)
public:
    PKCCriticalSection m_lock;                  // +0x00
    PKCQueue<T>*       m_pQueue;                // +0x18
    PKCSemaphore       m_available;             // +0x1c
    long Send(const T* item);                   // 0x00C1B130: 1 when the queue is full
};

class CRequestQueue {                           // 0x34 bytes: the static queue and its mailbox
public:
    PKCStaticQueue<SFileRequest> m_queue;       // +0x00
    PKCMailbox<SFileRequest> m_mailbox;         // +0x14
    ~CRequestQueue();                           // 0x00C1B2A0
};

class CMapNodeAllocator {                       // vtable 0x012B30EC: the map's heap node allocator
public:
    virtual void Free(void* node);              // slot 0 (0x00C1B320)
    virtual void* Allocate();                   // slot 1 (0x00C1B330)
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
    long Find(const unsigned long* key, void** value);   // 0x00C1AE50
};

#pragma auto_inline(off)
long PKCTRRedblackMap::Find(const unsigned long* key, void** value)   // same-TU body (landed 0x00C1AE50; retail compiles it here)
{
    PKCTRRedblackNode* node = Root;
    while (node != &Sentinel) {
        if (*key == node->Key) {
            *value = node->Value;
            LastFound = node;
            return 0;
        }
        if (*key < node->Key)
            node = node->Left;
        else
            node = node->Right;
    }
    return 3;
}
#pragma auto_inline(on)

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
    virtual ~PKCDefaultFileManager();
    bool QueueRead(unsigned long key, unsigned long size, void* a, void* b, void* c, void* d);
};

bool PKCDefaultFileManager::QueueRead(unsigned long key, unsigned long size, void* a, void* b, void* c, void* d)
{
    SFileRequest request;
    {
        PKCProtectionInstance lock(&m_lock);
        SOpenFile* file;
        if (m_files.Find(&key, (void**)&file) != 0)
            return false;
        request.m_hFile = file->m_hFile;
        ++file->m_pendingRequests;
    }
    request.m_key = key;
    request.m_size = size;
    request.m_a = a;
    request.m_b = b;
    request.m_c = c;
    request.m_d = d;
    if (m_requests.m_mailbox.Send(&request) != 0) {
        PKCProtectionInstance lock(&m_lock);
        SOpenFile* file;
        if (m_files.Find(&key, (void**)&file) == 0)
            --file->m_pendingRequests;
        return false;
    }
    return true;
}