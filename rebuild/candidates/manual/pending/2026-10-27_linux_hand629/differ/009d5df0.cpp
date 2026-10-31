// CBankFileAsyncData::CBankFileAsyncData(unsigned long length, unsigned long skipBytes, CMovableResourceMemoryPool* memoryPool, CBankFileAsync& bankFile) -- async read into pool memory: record length/skip/pool/bank, no buffer yet, default load descriptor, clear the flags and register the instance with the bank under its lock
// Grade: DIFFER(183v183) -- scheduling only: retail interleaves the flag stores with the lock arguments (`push ebx ; mov [esi+44h],bl ; push ecx ; mov [esi+45h],bl ; lea ecx`), ours sinks all four below `push ecx ; lea ecx`
// Leaf: ??0CBankFileAsyncData@@AAE@KKPAVCMovableResourceMemoryPool@@AAVCBankFileAsync@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009D5DF0, 183 bytes, `ret 10h`.
// Identity: Ego_r.pdb ??0CBankFileAsyncData@@AAE@PAXKAAVCBankFileAsync@@@Z (private; the buffer overload), next to the
//   pool overload 0x009D5DF0 ??0CBankFileAsyncData@@AAE@KKPAVCMovableResourceMemoryPool@@AAVCBankFileAsync@@@Z.
// Callees: CBaseClassNonCopyable::CBaseClassNonCopyable 0x0099A2F0, CCriticalSectionLock::CCriticalSectionLock(
//   _RTL_CRITICAL_SECTION&, bool) 0x00A62920 / ~CCriticalSectionLock 0x00A629A0, the set's _Rb_tree::insert_unique
//   0x009D7CF0 (pair<iterator,bool> by hidden pointer; the `this` value homed in the dead length-argument slot).
// Shape: CBaseClassNonCopyable + the CMovableResource base at +4 (its own vtable 0x0129B860 stored by its inline ctor,
//   then the derived 0x0129B8B4 / 0x0129B8A0), members in declaration order with the zero register ebx, the load
//   descriptor's inline ctor; InternalAllocatedBufferSize is assigned in the body after the insert, inside the lock.
// Tried (sweep): flags in the init list / at the top of the body, two flags in the init list, a section pointer local
//   between the flag pairs, the CBaseClassNonCopyable header. Chunk 629: /G5 /G6 /G7 /GB, the PDB `_RTL_CRITICAL_SECTION&`
//   lock parameter, flags after the lock construction, the real std::set / _Rb_tree::insert_unique model -- unchanged.
// Layout: Ego_r.pdb CBankFileAsyncData (0x48; the retail offsets agree with the landed dtor 0x009D61F0).
#include "engine/CBaseClassNonCopyable.h"

struct _RTL_CRITICAL_SECTION {                     // Win32 RTL_CRITICAL_SECTION, 24 bytes
    void* DebugInfo; long LockCount; long RecursionCount; void* OwningThread; void* LockSemaphore; unsigned long SpinCount;
};
#define CCriticalSectionLock CCriticalSectionLockLayout
#include "engine/CCriticalSectionLock.h"
#undef CCriticalSectionLock
struct CCriticalSectionLock : public CCriticalSectionLockLayout {   // 8 bytes on the stack
    CCriticalSectionLock(_RTL_CRITICAL_SECTION& criticalSection, bool tryOnly);   // 0x00A62920
    ~CCriticalSectionLock();                                                     // 0x00A629A0
};

namespace std {                                    // STLport 4.6 set<T*> (12 bytes), only what the ctor reaches
template <class T> struct less { };
template <class T> struct _Identity { };
template <class T> class allocator { public: allocator() { } };
template <class T> struct _Nonconst_traits { };
template <class A, class B> struct pair {
    A first; B second;
    pair() : first(A()), second(B()) { }
    pair(const A& a, const B& b) : first(a), second(b) { }
};
struct _Rb_tree_node_base;
struct _Rb_tree_base_iterator { _Rb_tree_node_base* _M_node; };
template <class V, class Traits> struct _Rb_tree_iterator : public _Rb_tree_base_iterator {
    _Rb_tree_iterator() { _M_node = 0; }
};
template <class K, class V, class KoV, class Cmp, class Alloc> class _Rb_tree {
public:
    typedef _Rb_tree_iterator<V, _Nonconst_traits<V> > iterator;
    _Rb_tree_node_base* _M_header;                 // _STLP_alloc_proxy<_Link_type, ...>
    unsigned int _M_node_count;
    Cmp _M_key_compare;
    pair<iterator, bool> insert_unique(const V& v);   // 0x009D7CF0
};
template <class K, class Cmp = less<K>, class A = allocator<K> > class set {
public:
    typedef _Rb_tree<K, K, _Identity<K>, Cmp, A> _Rep_type;
    typedef typename _Rep_type::iterator iterator;
    _Rep_type _M_t;
    pair<iterator, bool> insert(const K& x) { return _M_t.insert_unique(x); }
};
}

class CMovableResource {                           // vtable 0x0129B860 (dtor, Verify, ...), the second base at +4
public:
    virtual ~CMovableResource() { }
};
class CThreadedFileCallback;
class CMovableResourceMemoryPool;
enum EThreadedFilePriority { THREADED_FILE_PRIORITY_DEFAULT = 3 };
class CThreadedFileObjectDesc {                    // 0x18 bytes
public:
    void* PDst;                                    // +0x00
    unsigned long FilePos;                         // +0x04
    unsigned long Size;                            // +0x08
    CThreadedFileCallback* PCallback;              // +0x0c
    bool QueueWhenFinished;                        // +0x10
    EThreadedFilePriority Priority;                // +0x14
    CThreadedFileObjectDesc() : PDst(0), FilePos(0), Size(0), PCallback(0), QueueWhenFinished(true),
                                Priority(THREADED_FILE_PRIORITY_DEFAULT) { }
};
class CBankFileAsyncData;
class CBankFileAsync {                             // retail offsets (landed 0x009D61F0 model)
public:
    unsigned char _pad000[0x128];
    std::set<CBankFileAsyncData*> AsyncDataInstances;   // +0x128
    std::set<CBankFileAsyncData*> ReadsToRestart;       // +0x134
    unsigned char _pad140[0xc];
    _RTL_CRITICAL_SECTION CriticalSection;         // +0x14c
};

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
    virtual void OnFileReadBegin();                // slot 1  0x009D5440
    virtual void OnFileReadCompletion();           // slot 2  0x009D54A0
private:                                           // Ego_r.pdb: both ctors private (AAE)
    CBankFileAsyncData(void* buffer, unsigned long length, CBankFileAsync& bankFile);
    CBankFileAsyncData(unsigned long length, unsigned long skipBytes, CMovableResourceMemoryPool* memoryPool,
                       CBankFileAsync& bankFile);
};

CBankFileAsyncData::CBankFileAsyncData(unsigned long length, unsigned long skipBytes, CMovableResourceMemoryPool* memoryPool,
                                       CBankFileAsync& bankFile)
    : Data(0), Buffer(0), Length(length), SkipBytes(skipBytes), ThreadedFileUID(0), MemoryPool(memoryPool), MemoryHandle(-1),
      BankFile(&bankFile), Finished(false), BufferLocked(false), AllowRestart(false), Aborted(false)
{
    CCriticalSectionLock lock(BankFile->CriticalSection, false);
    BankFile->AsyncDataInstances.insert(this);
    InternalAllocatedBufferSize = 0;
}
