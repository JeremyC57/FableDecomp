// CMemAllocator::~CMemAllocator() -- the DirectShow base-class destructor: Decommit() (statically bound, on the
// IMemAllocatorCallbackTemp subobject), ReallyFree(), then ~CBaseAllocator(); EH-framed
// Vendor-proof: strmbase-dxsdk-sum2004(BaseClasses/amfilter.cpp) ??1CMemAllocator@@UAE@XZ -- 96 bytes verified against Fable.exe 0x00CAA410
// Leaf: ??1CMemAllocator@@UAE@XZ
// Flags: /EHsc
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00CAA410, length 96 bytes. Built under /EHsc.
// Identity: the vendor proof above (amfilter.cpp: `CMemAllocator::~CMemAllocator() { Decommit(); ReallyFree(); }`); its primary
//   vtable 0x012BF83C (no RTTI: slots 0..2 the landed CBaseAllocator::NonDelegatingQueryInterface 0x00CA9D70 / CUnknown AddRef /
//   Release, 3 the deleting destructor 0x00CABC60, 5 the landed CMemAllocator::Alloc 0x00CAC640) and its interface vtable 0x012BF810
//   at +0x0C; the callees are the landed CBaseAllocator::Decommit 0x00CAA160, CMemAllocator::ReallyFree 0x00CAA3C0 (the landed
//   0x00CAC640 binds ?ReallyFree@CMemAllocator@@IAEXXZ) and CBaseAllocator::~CBaseAllocator 0x00CA9D00. The catalog's
//   "CVtable_012bf83c" was a vtable placeholder; agentFAM08 respelled the local vendor model with the strmbase names.
// Shape: in a destructor the call to the virtual Decommit is bound statically and the `this` adjustment to the interface is computed
//   once (`lea eax,[esi+0Ch]`) for the vtable store and the push; EH state 0 covers the body because the base destructor is out of
//   line. Local vendor model (strmbase has no shared header): combase.h CUnknown / CMyBaseObject (Fable's renamed CBaseObject),
//   amfilter.h CBaseAllocator / CMemAllocator.
struct _AllocatorProperties;
struct IMediaSample;
struct IMemAllocatorNotifyCallbackTemp;

class CMyBaseObject {                          // combase.h CBaseObject (renamed in Fable's strmbase): object counter, no data here
public:
    ~CMyBaseObject();                          // 0x00CACAF0
};

struct IUnknown {
    virtual long __stdcall QueryInterface(const void* iid, void** object) = 0;
    virtual unsigned long __stdcall AddRef() = 0;
    virtual unsigned long __stdcall Release() = 0;
};

struct IMemAllocator : IUnknown {
    virtual long __stdcall SetProperties(_AllocatorProperties* request, _AllocatorProperties* actual) = 0;
    virtual long __stdcall GetProperties(_AllocatorProperties* props) = 0;
    virtual long __stdcall Commit() = 0;
    virtual long __stdcall Decommit() = 0;     // slot 6
};
struct IMemAllocatorCallbackTemp : IMemAllocator { };

struct CUnknown : IUnknown {                   // 0x0c bytes: vptr, the object counter, reference count
    virtual ~CUnknown();                       // slot 3
    CMyBaseObject m_baseObject;                // +0x04
    long          m_cRef;                      // +0x08
};

struct CBaseAllocator : CUnknown, IMemAllocatorCallbackTemp {   // the interface at +0x0c
    unsigned char m_body[0x60 - 0x10];         // +0x10 critical section, free list, events, notify pointer
    virtual long __stdcall Decommit();         // 0x00CAA160
    ~CBaseAllocator();                         // 0x00CA9D00
};

struct CMemAllocator : CBaseAllocator {        // vtables 0x012BF83C / 0x012BF810
protected:
    unsigned char* m_pBuffer;                  // +0x60
    void ReallyFree();                         // 0x00CAA3C0 (protected: ?ReallyFree@CMemAllocator@@IAEXXZ)
public:
    virtual long __stdcall QueryInterface(const void* iid, void** object);   // 0x00CA9D70
    virtual unsigned long __stdcall AddRef();  // 0x00CACD00
    virtual unsigned long __stdcall Release(); // 0x00CACD20
    virtual long __stdcall SetProperties(_AllocatorProperties* request, _AllocatorProperties* actual);
    virtual long __stdcall GetProperties(_AllocatorProperties* props);
    virtual long __stdcall Commit();
    virtual ~CMemAllocator();
};

CMemAllocator::~CMemAllocator()
{
    Decommit();
    ReallyFree();
}
