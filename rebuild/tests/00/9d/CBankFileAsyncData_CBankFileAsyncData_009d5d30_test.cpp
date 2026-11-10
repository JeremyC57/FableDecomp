// Standalone constructor behavior fixture. Access is public solely for observation; addresses and widths are host-native.
// The retail class declares Finished / BufferLocked / AllowRestart volatile (engine/CBankFileAsyncData.h); this host model needs no qualifier.
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <new>
#define CHECK(c) do { if (!(c)) { printf("FAIL line %d\n", __LINE__); abort(); } } while (0)
class CBankFileAsyncData;
class CBankFileAsync;
static CBankFileAsyncData* expectedData;
static CBankFileAsync* expectedBank;
static void* expectedBuffer;
static unsigned long expectedLength;
static int stage,insertCalls,lockCalls,unlockCalls,baseCalls;
static bool held;
static void checkInitialized(bool sizeIsZero);

enum EThreadedFilePriority {
    THREADED_FILE_PRIORITY_LOW=1,
    THREADED_FILE_PRIORITY_BELOW_NORMAL=2,
    THREADED_FILE_PRIORITY_NORMAL=3,
    THREADED_FILE_PRIORITY_HIGH=4,
    THREADED_FILE_PRIORITY_HIGHEST=5,
    THREADED_FILE_PRIORITY_BLOCKING=6,
    THREADED_FILE_PRIORITY_COUNT=7
};
class CBaseClassNonCopyable { public:
    CBaseClassNonCopyable() { CHECK(stage==0); ++baseCalls; stage=1; }
    virtual ~CBaseClassNonCopyable() { }
};
class CThreadedFileCallback : public CBaseClassNonCopyable { public:
    CThreadedFileCallback() { }
    virtual ~CThreadedFileCallback() { }
    virtual bool OnFileReadBegin(void**,EThreadedFilePriority) { return false; }
    virtual void OnFileReadCompletion(bool) { }
};
class CMovableResource { public:
    CMovableResource() { }
    virtual ~CMovableResource() { }
    virtual void RelocateData(long,void*,void*,long)=0;
    virtual bool CanRelocateData(long) const { return true; }
    virtual unsigned long Verify() const { return 123; }
};
class CMovableResourceMemoryPool;
class CThreadedFileObjectDesc {
public:
    void* PDst;
    unsigned long FilePos;
    unsigned long Size;
    CThreadedFileCallback* PCallback;
    bool QueueWhenFinished;
    EThreadedFilePriority Priority;
    CThreadedFileObjectDesc() : PDst(0), FilePos(0), Size(0), PCallback(0), QueueWhenFinished(true), Priority(THREADED_FILE_PRIORITY_NORMAL) { }
};

struct _RTL_CRITICAL_SECTION { unsigned char storage[24]; };
class CCriticalSectionLock { public:
    CCriticalSectionLock(_RTL_CRITICAL_SECTION&,bool);
    ~CCriticalSectionLock();
protected:
    _RTL_CRITICAL_SECTION& CriticalSection;
    bool Locked;
};
namespace std {
template<class T> struct less { };
template<class T> struct _Identity { };
template<class T> class allocator { };
template<class T> struct _Nonconst_traits { };
template<class T> struct _Const_traits { };
template<class V,class Traits> struct _Rb_tree_iterator {
    void* _M_node;
    _Rb_tree_iterator() : _M_node(0) { }
};
template<class A,class B> struct pair {
    A first; B second;
    pair() : first(A()),second(B()) { }
    pair(const A& a,const B& b) : first(a),second(b) { }
};
// Only the external tree service is stubbed; set::insert below is the shared STLport forwarding body.
template<class K,class V,class KeyOfValue,class Compare,class A> class _Rb_tree { public:
    typedef _Rb_tree_iterator<V,_Nonconst_traits<V> > iterator;
    typedef _Rb_tree_iterator<V,_Const_traits<V> > const_iterator;
    pair<iterator,bool> insert_unique(const V& value);
};
template<class K,class Compare=less<K>,class A=allocator<K> > class set { public:
    typedef _Rb_tree<K,K,_Identity<K>,Compare,A> _Rep_type;
    _Rep_type _M_t;
    typedef typename _Rep_type::const_iterator iterator;
    pair<iterator,bool> insert(const K& value){typedef typename _Rep_type::iterator RepIterator;pair<RepIterator,bool> p=_M_t.insert_unique(value);return pair<iterator,bool>((const iterator&)p.first,p.second);}
};
}
class CBankFileAsync { public:
    std::set<CBankFileAsyncData*> AsyncDataInstances;
    std::set<CBankFileAsyncData*> ReadsToRestart;
    _RTL_CRITICAL_SECTION CriticalSection;
};
class CBankFileAsyncData : public CThreadedFileCallback, public CMovableResource { public:
    virtual ~CBankFileAsyncData() { }
    virtual bool OnFileReadBegin(void**,EThreadedFilePriority) { return false; }
    virtual void OnFileReadCompletion(bool) { }
    virtual void RelocateData(long,void*,void*,long) { }
    virtual bool CanRelocateData(long) const { return true; }
    CBankFileAsyncData(void*,unsigned long,CBankFileAsync&);
    void* Data; void* Buffer;
    unsigned long Length,SkipBytes,ThreadedFileUID;
    CMovableResourceMemoryPool* MemoryPool;
    long MemoryHandle;
    unsigned long InternalAllocatedBufferSize;
    CBankFileAsync& BankFile;
    CThreadedFileObjectDesc LoadDesc;
    bool Finished;
    bool BufferLocked;
    bool AllowRestart;
    bool Aborted;
};

CBankFileAsyncData::CBankFileAsyncData(void* buffer,unsigned long length,CBankFileAsync& bankFile)
    : Data(buffer), Buffer(buffer), Length(length), SkipBytes(0), ThreadedFileUID(0), MemoryPool(0), MemoryHandle(-1),
      BankFile(bankFile), Finished(false), BufferLocked(false), AllowRestart(false), Aborted(false)
{
    CCriticalSectionLock lock(BankFile.CriticalSection,false);
    BankFile.AsyncDataInstances.insert(this);
    InternalAllocatedBufferSize=0;
}

static void checkInitialized(bool sizeIsZero)
{
    CBankFileAsyncData& data=*expectedData;
    CHECK(data.Data==expectedBuffer && data.Buffer==expectedBuffer && data.Length==expectedLength);
    CHECK(data.SkipBytes==0 && data.ThreadedFileUID==0 && data.MemoryPool==0 && data.MemoryHandle==-1);
    CHECK(&data.BankFile==expectedBank);
    CHECK(data.LoadDesc.PDst==0 && data.LoadDesc.FilePos==0 && data.LoadDesc.Size==0 && data.LoadDesc.PCallback==0);
    CHECK(data.LoadDesc.QueueWhenFinished && (int)data.LoadDesc.Priority==3);
    CHECK(!data.Finished && !data.BufferLocked && !data.AllowRestart && !data.Aborted);
    if (sizeIsZero) CHECK(data.InternalAllocatedBufferSize==0);
    else {
        // Observe representation bytes only: the ctor intentionally has not assigned this member yet.
        const unsigned char* representation=(const unsigned char*)&data.InternalAllocatedBufferSize;
        for (unsigned int i=0;i<sizeof(data.InternalAllocatedBufferSize);++i) CHECK(representation[i]==0xA5);
    }
}
CCriticalSectionLock::CCriticalSectionLock(_RTL_CRITICAL_SECTION& section,bool tryOnly)
    : CriticalSection(section),Locked(true)
{
    CHECK(stage==1 && !held && &section==&expectedBank->CriticalSection && !tryOnly);
    checkInitialized(false); held=true; ++lockCalls; stage=2;
}
CCriticalSectionLock::~CCriticalSectionLock()
{
    CHECK(stage==3 && held && Locked && &CriticalSection==&expectedBank->CriticalSection);
    checkInitialized(true); held=false; ++unlockCalls; stage=4;
}
template<class K,class V,class KeyOfValue,class Compare,class A>
std::pair<typename std::_Rb_tree<K,V,KeyOfValue,Compare,A>::iterator,bool>
std::_Rb_tree<K,V,KeyOfValue,Compare,A>::insert_unique(const V& value)
{
    CHECK(stage==2 && held);
    CHECK((const void*)this==(const void*)&expectedBank->AsyncDataInstances._M_t);
    CHECK(value==expectedData);
    checkInitialized(false); ++insertCalls; stage=3;
    return pair<iterator,bool>(iterator(),true);
}
union AlignedStorage { long double align; void* pointerAlign; unsigned char bytes[sizeof(CBankFileAsyncData)]; };
int main()
{
    const unsigned long lengths[]={0,1,0x7FFFFFFFUL,0x80000000UL,0xFFFFFFFFUL};
    unsigned char buffer[17];
    CBankFileAsync banks[2];
    for (int bankIndex=0;bankIndex<2;++bankIndex) for (int bufferKind=0;bufferKind<3;++bufferKind) for (int n=0;n<5;++n) {
        AlignedStorage raw; memset(raw.bytes,0xA5,sizeof raw.bytes); memset(buffer,0x69,sizeof buffer);
        expectedData=(CBankFileAsyncData*)raw.bytes; expectedBank=&banks[bankIndex];
        expectedBuffer=bufferKind==0?0:buffer+(bufferKind==1?0:3); expectedLength=lengths[n];
        stage=insertCalls=lockCalls=unlockCalls=baseCalls=0;held=false;
        CBankFileAsyncData* data=new(raw.bytes) CBankFileAsyncData(expectedBuffer,expectedLength,*expectedBank);
        CHECK(data==expectedData && stage==4 && !held);
        CHECK(insertCalls==1 && lockCalls==1 && unlockCalls==1 && baseCalls==1);
        checkInitialized(true);
        for (unsigned int i=0;i<sizeof buffer;++i) CHECK(buffer[i]==0x69);
        data->~CBankFileAsyncData();
    }
    puts("009d5d30_TEST PASS"); return 0;
}