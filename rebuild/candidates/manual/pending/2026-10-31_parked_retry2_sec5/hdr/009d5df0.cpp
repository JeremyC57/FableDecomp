// CBankFileAsyncData::CBankFileAsyncData(unsigned long length, unsigned long skipBytes, CMovableResourceMemoryPool* pool, CBankFileAsync& bank) -- async read into pool memory: record length/skip/pool/bank, no buffer yet, default load descriptor, clear the flags and register the instance with the bank under its lock
// Leaf: ??0CBankFileAsyncData@@
// Grade: DIFFER(183v183) -- base moved onto engine/CBaseClassNonCopyable.h (ctor 0x0099A2F0; bytes unchanged); residue
//   (as the twin 0x009D5D30): retail interleaves the +0x46/+0x47 flag stores after the lock's push/lea
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009D5DF0, length 181 bytes (+11 int3), ret 0x10. Ego_r.pdb
// ??0CBankFileAsyncData@@AAE@KKPAVCMovableResourceMemoryPool@@AAVCBankFileAsync@@@Z (private there; public here).
// Callees: CBase::CBase @ 0x0099A2F0, CCriticalSectionLock ctor/dtor @ 0x00A62920 / 0x00A629A0, set<CBankFileAsyncData*>::insert
// @ 0x009D7CF0 (catalog "BinaryTree_FindOrInsert"; the value `this` homed in the dead length-argument slot).
// Shape: as the buffer overload 0x009D5D30: CBase + CMovableResource (+4) bases, members in declaration order,
// the flags assigned at the top of the body, InternalAllocatedBufferSize assigned after the insert inside the lock's scope.
// Layout = Ego_r.pdb CBankFileAsyncData (0x48; the retail offsets agree).
struct CRITICAL_SECTION_Fable {                    // Win32 RTL_CRITICAL_SECTION, 24 bytes
    void* DebugInfo; long LockCount; long RecursionCount; void* OwningThread; void* LockSemaphore; unsigned long SpinCount;
};
struct CCriticalSectionLock {                      // 8 bytes on the stack
    CRITICAL_SECTION_Fable* m_pSection;            // +0x00
    bool                    m_locked;              // +0x04
    CCriticalSectionLock(CRITICAL_SECTION_Fable* section, bool tryOnly);   // 0x00A62920
    ~CCriticalSectionLock();                       // 0x00A629A0
};
#include "engine/CBaseClassNonCopyable.h"     // CBaseClassNonCopyable() 0x0099A2F0
class CMovableResource {                           // vtable 0x0129B860, interface base at +4
public:
    virtual void v00();
    CMovableResource() {}
};
class CThreadedFileCallback;
enum EThreadedFilePriority { THREADED_FILE_PRIORITY_DEFAULT = 3 };
struct CThreadedFileObjectDesc {                   // 0x18 bytes
    void* PDst;                                    // +0x00
    unsigned long FilePos;                         // +0x04
    unsigned long Size;                            // +0x08
    CThreadedFileCallback* PCallback;              // +0x0c
    bool QueueWhenFinished;                        // +0x10
    EThreadedFilePriority Priority;                // +0x14
    CThreadedFileObjectDesc() : PDst(0), FilePos(0), Size(0), PCallback(0), QueueWhenFinished(true),
                                Priority(THREADED_FILE_PRIORITY_DEFAULT) {}
};
class CBankFileAsyncData;
struct SInsertResult { void* iterator; bool inserted; };
struct CAsyncDataSet {                             // set<CBankFileAsyncData*> (STLport, 12 bytes)
    void* _M_header; unsigned long _M_node_count; unsigned char _M_compare;
    SInsertResult insert(CBankFileAsyncData* const& value);   // 0x009D7CF0
};
class CBankFileAsync {
public:
    unsigned char _pad[0x128];
    CAsyncDataSet AsyncDataInstances;              // +0x128
    unsigned char _pad134[0x18];                   // +0x134 .. +0x14b
    CRITICAL_SECTION_Fable CriticalSection;        // +0x14c
};
class CMovableResourceMemoryPool;

class CBankFileAsyncData : public CBaseClassNonCopyable, public CMovableResource {   // vtables 0x0129B8B4 / 0x0129B8A0
public:
    void* Data;                                    // +0x08
    void* Buffer;                                  // +0x0c
    unsigned long Length;                          // +0x10
    unsigned long SkipBytes;                       // +0x14
    unsigned long ThreadedFileUID;                 // +0x18
    CMovableResourceMemoryPool* MemoryPool;        // +0x1c
    long MemoryHandle;                             // +0x20
    unsigned long InternalAllocatedBufferSize;     // +0x24
    CBankFileAsync* BankFile;                      // +0x28
    CThreadedFileObjectDesc LoadDesc;              // +0x2c
    bool Finished;                                 // +0x44
    bool BufferLocked;                             // +0x45
    bool AllowRestart;                             // +0x46
    bool Aborted;                                  // +0x47
    virtual void v00();
    CBankFileAsyncData(unsigned long length, unsigned long skipBytes, CMovableResourceMemoryPool* pool, CBankFileAsync& bank);
};

CBankFileAsyncData::CBankFileAsyncData(unsigned long length, unsigned long skipBytes, CMovableResourceMemoryPool* pool, CBankFileAsync& bank)
    : Data(0), Buffer(0), Length(length), SkipBytes(skipBytes), ThreadedFileUID(0), MemoryPool(pool), MemoryHandle(-1),
      BankFile(&bank)
{
    Finished = false;
    BufferLocked = false;
    AllowRestart = false;
    Aborted = false;
    CCriticalSectionLock lock(&BankFile->CriticalSection, false);
    BankFile->AsyncDataInstances.insert(this);
    InternalAllocatedBufferSize = 0;
}
