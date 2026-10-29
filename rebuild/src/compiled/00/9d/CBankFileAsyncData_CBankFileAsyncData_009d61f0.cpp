// CBankFileAsyncData::~CBankFileAsyncData() -- unless aborted: under the bank's critical section, drop this request from the bank's instance and restart sets, spin (unlocking around Sleep(0)) until any outstanding threaded read has finished, and give the pool handle back; then return an internally allocated buffer to the global allocator (destroying the allocator when it empties and is flagged to die) and run the base destructors
// Leaf: ??1CBankFileAsyncData@@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009D61F0, length 348 bytes (+ int3 padding), ebp frame aligned to 8, plain ret.
// Callees: CCriticalSectionLock ctor @ 0x00A62920 / Unlock @ 0x00A62960 / Lock @ 0x00A62980 / dtor @ 0x00A629A0,
//          set<CBankFileAsyncData*>::erase(const key&) @ 0x009D7F40 (_Rb_tree, twice), CThreadedFile::HasReadFinished(ulong)
//          @ 0x0098EC90 (a thunk onto the global thread manager that ignores `this`), Sleep (kernel32 import 0x0143FE1C,
//          hoisted into edi), CHandleBasedMemoryPool::Free @ 0x00A62ED0, GFEnterAllocatorLock @ 0x009F2000 /
//          GFLeaveAllocatorLock @ 0x009F2010, CMemoryAllocator::InternalDeallocate @ 0x009F2920 / GetAllocationCount
//          @ 0x009F1F10 / ~CMemoryAllocator @ 0x009F2310 / operator delete @ 0x009F1F90 (the landed
//          GFFreeFromGlobalAllocator body, inlined), CBaseClassNonCopyable::~CBaseClassNonCopyable @ 0x0099A300. Vtables 0x0129B8B4 (primary) /
//          0x0129B8A0 (the threaded-file request base inside CBankFileAsyncData) / 0x0129B860 (that base's own).
// Globals: the loading allocator @ 0x013CAA90 and its destroy-when-empty flag @ 0x013CAA94.
// Layout: Ego_r.pdb CBankFileAsyncData (0x48; two vptrs): Data +8, Buffer +0xc, Length +0x10, SkipBytes +0x14,
//          ThreadedFileUID +0x18, MemoryPool +0x1c, MemoryHandle +0x20, InternalAllocatedBufferSize +0x24, BankFile +0x28,
//          Finished +0x44, BufferLocked +0x45, AllowRestart +0x46, Aborted +0x47; the bank's sets at +0x128 / +0x134, its
//          critical section at +0x14c, its threaded file (counted pointer) at +0x110.
// Shape: the erase key is the implicit `this` temporary (one frame word reused by both erases); the lock local is
//          8-aligned (`and esp,-8`); the wait is a peeled test plus a do-while that unlocks around Sleep(0), the threaded
//          file reached through an inline accessor so the object loads before the UID push (a direct member chain pushes
//          first); the buffer free is the inlined GFFreeFromGlobalAllocator with the buffer read into edi before the lock.
extern "C" __declspec(dllimport) void __stdcall Sleep(unsigned long milliseconds);   // import 0x0143FE1C

struct CRITICAL_SECTION_Fable { unsigned char _body[0x18]; };
struct __declspec(align(8)) CCriticalSectionLock_Retail {   // 8 bytes on the stack
    CRITICAL_SECTION_Fable* m_pSection;
    bool                    m_locked;
    CCriticalSectionLock_Retail(CRITICAL_SECTION_Fable* section, bool tryOnly);   // 0x00A62920
    ~CCriticalSectionLock_Retail();                                               // 0x00A629A0
    void Unlock();                                                                // 0x00A62960
    void Lock();                                                                  // 0x00A62980
};

class CMemoryAllocator { public:
    void InternalDeallocate(void* block);          // 0x009F2920
    long GetAllocationCount();                     // 0x009F1F10
    ~CMemoryAllocator();                           // 0x009F2310
    static void __fastcall operator delete(void* block);   // 0x009F1F90
};
void __cdecl GFEnterAllocatorLock();               // 0x009F2000
void __cdecl GFLeaveAllocatorLock();               // 0x009F2010
extern CMemoryAllocator* g_pGlobalAllocator_013caa90;          // 0x013CAA90
extern bool              g_bDestroyAllocatorWhenEmpty_013caa94; // 0x013CAA94
__forceinline void GFFreeFromGlobalAllocator(void* block)
{
    GFEnterAllocatorLock();
    if (block && g_pGlobalAllocator_013caa90) {
        g_pGlobalAllocator_013caa90->InternalDeallocate(block);
        if (g_bDestroyAllocatorWhenEmpty_013caa94) {
            if (g_pGlobalAllocator_013caa90->GetAllocationCount() == 0) {
                delete g_pGlobalAllocator_013caa90;
                g_pGlobalAllocator_013caa90 = 0;
            }
        }
    }
    GFLeaveAllocatorLock();
}

class CHandleBasedMemoryPool { public: void Free_00a62ed0(long handle); };   // 0x00A62ED0
class CThreadedFile {
public:
    bool HasReadFinished(unsigned long readUID);   // 0x0098EC90
};
class CBankFileAsyncData;
struct CAsyncDataSet {                             // set<CBankFileAsyncData*> (STLport _Rb_tree)
    void* _M_header;
    unsigned int _M_node_count;
    unsigned char _M_key_compare;
    unsigned int erase(CBankFileAsyncData* const& key);   // 0x009D7F40
};
class CBankFileAsync { public:
    unsigned char _pad000[0x110];
    CThreadedFile* ThreadedFile;                   // +0x110 (counted pointer object)
    unsigned char _pad114[0x14];
    CAsyncDataSet AsyncDataInstances;              // +0x128
    CAsyncDataSet ReadsToRestart;                  // +0x134
    unsigned char _pad140[0xc];
    CRITICAL_SECTION_Fable CriticalSection;        // +0x14c
    CThreadedFile* GetThreadedFile() const { return ThreadedFile; }
};

class CBaseClassNonCopyable {
protected:
    virtual ~CBaseClassNonCopyable();                              // 0x0099A300
};
struct CThreadedFileRequest {                      // the second base (vtable 0x0129B860): the file thread's callback interface
    virtual ~CThreadedFileRequest() { }
};
class CBankFileAsyncData : public CBaseClassNonCopyable, public CThreadedFileRequest {   // vtables 0x0129B8B4 / 0x0129B8A0
public:
    void*                   Data;                  // +0x08
    void*                   Buffer;                // +0x0c
    unsigned long           Length;                // +0x10
    unsigned long           SkipBytes;             // +0x14
    unsigned long           ThreadedFileUID;       // +0x18
    CHandleBasedMemoryPool* MemoryPool;            // +0x1c
    long                    MemoryHandle;          // +0x20
    unsigned long           InternalAllocatedBufferSize;   // +0x24
    CBankFileAsync*         BankFile;              // +0x28
    unsigned char           LoadDesc[0x18];        // +0x2c
    bool                    Finished;              // +0x44
    bool                    BufferLocked;          // +0x45
    bool                    AllowRestart;          // +0x46
    bool                    Aborted;               // +0x47
    virtual ~CBankFileAsyncData();
};

CBankFileAsyncData::~CBankFileAsyncData()
{
    if (!Aborted) {
        CCriticalSectionLock_Retail lock(&BankFile->CriticalSection, false);
        BankFile->AsyncDataInstances.erase(this);
        BankFile->ReadsToRestart.erase(this);
        if (ThreadedFileUID) {
            if (!BankFile->GetThreadedFile()->HasReadFinished(ThreadedFileUID)) {
                do {
                    lock.Unlock();
                    Sleep(0);
                    lock.Lock();
                } while (!BankFile->GetThreadedFile()->HasReadFinished(ThreadedFileUID));
            }
        }
        if (MemoryPool && MemoryHandle != -1) {
            MemoryPool->Free_00a62ed0(MemoryHandle);
            MemoryHandle = -1;
            Buffer = 0;
            Data = 0;
        }
    }
    if (InternalAllocatedBufferSize > 0)
        GFFreeFromGlobalAllocator(Buffer);
}