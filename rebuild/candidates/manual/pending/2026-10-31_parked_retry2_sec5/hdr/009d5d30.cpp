// CBankFileAsyncData::CBankFileAsyncData(void* buffer, unsigned long length, CBankFileAsync& bank) -- async read into a caller-supplied buffer: record buffer/length/bank, default load descriptor, clear the flags and register the instance with the bank under its lock
// Leaf: ??0CBankFileAsyncData@@
// Grade: DIFFER(179v179) -- base moved onto engine/CBaseClassNonCopyable.h (ctor 0x0099A2F0; bytes unchanged); residue:
//   retail interleaves the +0x46/+0x47 flag stores after the lock's push/lea, ours stores all four flags first
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009D5D30, length 179 bytes (+13 int3), ret 0xc. Ego_r.pdb
// ??0CBankFileAsyncData@@AAE@PAXKAAVCBankFileAsync@@@Z (private there; public here, the code is the same).
// Callees: CBase::CBase @ 0x0099A2F0, CCriticalSectionLock ctor/dtor @ 0x00A62920 / 0x00A629A0, set<CBankFileAsyncData*>::insert
// @ 0x009D7CF0 (catalog "BinaryTree_FindOrInsert"; pair result by hidden pointer, the value `this` homed in the dead
// buffer-argument slot).
// Shape: CBase + the CMovableResource interface base at +4 (its own vtable 0x0129B860 stored, then the derived 0x0129B8A0),
// members in declaration order (Data and Buffer both the buffer, the load descriptor's inline ctor), the zero register ebx;
// the four flags are assigned at the top of the body (initialisers schedule them differently around the lock's pushes);
// InternalAllocatedBufferSize is assigned in the body after the insert, inside the lock's scope.
// Layout = Ego_r.pdb CBankFileAsyncData (0x48; the retail offsets agree): Data +8 .. Aborted +0x47.
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
    CBankFileAsyncData(void* buffer, unsigned long length, CBankFileAsync& bank);
};

CBankFileAsyncData::CBankFileAsyncData(void* buffer, unsigned long length, CBankFileAsync& bank)
    : Data(buffer), Buffer(buffer), Length(length), SkipBytes(0), ThreadedFileUID(0), MemoryPool(0), MemoryHandle(-1),
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
