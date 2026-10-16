// Grade: DIFFER(191v196) -- control-flow shape: retail keeps the result in bl (0 before the restart-set erase, 1 after) with ONE shared lock-dtor tail for the cancel failure and the end; ours either duplicates the tail (return form, 194v196), materialises setne (|| form, 200v196) or zero-registers ebx (this goto form) | tried: return inside if, gone flag with ||, goto done | next: result assigned from the erase/cancel chain into a bool declared before the lock
// CBankFileAsyncData::Abort() -- under the bank lock: a finished read is trivially aborted; otherwise drop a pending restart or cancel the queued file read (failure = cannot abort yet), unregister the instance, free its pool memory and mark it aborted
// Leaf: Abort
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009D6350, length 196 bytes (+12 int3), returns bool.
// Callees: CCriticalSectionLock ctor/dtor @ 0x00A62920 / 0x00A629A0, set<CBankFileAsyncData*>::erase(const key&) @ 0x009D7F40
// (returns the count; `this` is copied into a frame temporary for the reference), CThreadedFile::CancelRead(unsigned long)
// @ 0x0098EC90, CMovableResourceMemoryPool::Free(long) @ 0x00A62ED0.
// Shape: the early `return true` has its own lock-dtor tail; the result is a bool local in bl (false until the read is
// known to be gone); the cancel failure jumps (goto) to the shared end -- a `return` there duplicates the lock tail.
// Layout = Ego_r.pdb CBankFileAsyncData: Data +8, Buffer +0xc, ThreadedFileUID +0x18, MemoryPool +0x1c, MemoryHandle +0x20,
// BankFile +0x28, Finished +0x44, Aborted +0x47; retail CBankFileAsync (ctor 0x009D5F80): ThreadedFile +0x110,
// AsyncDataInstances +0x128, ReadsToRestart +0x134, CriticalSection +0x14c.
struct CRITICAL_SECTION_Fable {                    // Win32 RTL_CRITICAL_SECTION, 24 bytes
    void* DebugInfo; long LockCount; long RecursionCount; void* OwningThread; void* LockSemaphore; unsigned long SpinCount;
};
struct CCriticalSectionLock {                      // 8 bytes on the stack
    CRITICAL_SECTION_Fable* m_pSection;            // +0x00
    bool                    m_locked;              // +0x04
    CCriticalSectionLock(CRITICAL_SECTION_Fable* section, bool tryOnly);   // 0x00A62920
    ~CCriticalSectionLock();                       // 0x00A629A0
};
class CBankFileAsyncData;
struct CAsyncDataSet {                             // set<CBankFileAsyncData*> (STLport, 12 bytes)
    void* _M_header; unsigned long _M_node_count; unsigned char _M_compare;
    unsigned long erase(CBankFileAsyncData* const& key);   // 0x009D7F40
};
struct CThreadedFile {
    bool CancelRead(unsigned long uid);            // 0x0098EC90
};
struct CMovableResourceMemoryPool {
    void Free(long handle);                        // 0x00A62ED0
};
template <class T> struct CCountedPointer { T* Data; void* Info; };
class CBankFileAsync {
public:
    unsigned char _pad[0x110];
    CCountedPointer<CThreadedFile> ThreadedFile;   // +0x110
    unsigned char _pad118[0x10];                   // +0x118 LoadingMemoryPool, MemoryFailureHandler
    CAsyncDataSet AsyncDataInstances;              // +0x128
    CAsyncDataSet ReadsToRestart;                  // +0x134
    unsigned char _pad140[0x0c];                   // +0x140 .. +0x14b
    CRITICAL_SECTION_Fable CriticalSection;        // +0x14c
};

class CBankFileAsyncData {
public:
    virtual void v00();
    void* _base4;                                  // +0x04 CMovableResource vptr
    void* Data;                                    // +0x08
    void* Buffer;                                  // +0x0c
    unsigned long Length;                          // +0x10
    unsigned long SkipBytes;                       // +0x14
    unsigned long ThreadedFileUID;                 // +0x18
    CMovableResourceMemoryPool* MemoryPool;        // +0x1c
    long MemoryHandle;                             // +0x20
    unsigned long InternalAllocatedBufferSize;     // +0x24
    CBankFileAsync* BankFile;                      // +0x28
    unsigned char LoadDesc[0x18];                  // +0x2c CThreadedFileObjectDesc
    bool Finished;                                 // +0x44
    bool BufferLocked;                             // +0x45
    bool AllowRestart;                             // +0x46
    bool Aborted;                                  // +0x47
    bool Abort();
};

bool CBankFileAsyncData::Abort()
{
    CCriticalSectionLock lock(&BankFile->CriticalSection, false);
    if (Finished)
        return true;
    bool result = false;
    if (BankFile->ReadsToRestart.erase(this) == 0) {
        if (!BankFile->ThreadedFile.Data->CancelRead(ThreadedFileUID))
            goto done;
        ThreadedFileUID = 0;
    }
    result = true;
    BankFile->AsyncDataInstances.erase(this);
    if (MemoryPool && MemoryHandle != -1) {
        MemoryPool->Free(MemoryHandle);
        MemoryHandle = -1;
        Buffer = 0;
        Data = 0;
    }
    Aborted = true;
done:
    return result;
}
