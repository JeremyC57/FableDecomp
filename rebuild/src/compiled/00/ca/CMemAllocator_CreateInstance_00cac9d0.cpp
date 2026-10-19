// CMemAllocator::CreateInstance -- allocate DirectShow memory allocator, initialize lock/free-list/semaphore and buffer; retail00cac9d0 212bytes. Propagated engine label rejected: CUnknown00cacc80, criticalsection import0143fee4, CreateSemaphoreA0143fe54, vtables012bf83c/012bf810 match allocator destructor00caa410. This is no-RTTI strmbase; CMemAllocator owner inferred from allocator methods and buffer+60. No shared class header exists.
// Flags: /EHsc
#ifdef _MSC_VER
#else
#define __declspec(x)
#define __stdcall
#define __forceinline inline
#define __cdecl
#endif
#include <stddef.h>
void* __cdecl operator new(size_t);
struct IUnknown;
struct CRITICAL_SECTION_ {void* DebugInfo;long LockCount,RecursionCount;void* OwningThread;void* LockSemaphore;unsigned long SpinCount;};
extern "C" __declspec(dllimport) void __stdcall InitializeCriticalSection(CRITICAL_SECTION_*); // import0143fee4
extern "C" __declspec(dllimport) void* __stdcall CreateSemaphoreA(void*,long,long,const char*); // import0143fe54
struct __declspec(novtable) INonDelegatingUnknown {
 virtual long __stdcall NonDelegatingQueryInterface(const void*,void**)=0;
 virtual unsigned long __stdcall NonDelegatingAddRef()=0;
 virtual unsigned long __stdcall NonDelegatingRelease()=0;
};
struct __declspec(novtable) CUnknownAllocator:INonDelegatingUnknown {
 IUnknown* m_pUnknown;volatile long m_cRef;
 CUnknownAllocator(const char*,IUnknown*); //00cacc80
 virtual ~CUnknownAllocator();
};
struct __declspec(novtable) IMemAllocatorCallbackTemp {
 virtual long __stdcall QueryInterface(const void*,void**)=0;
 virtual unsigned long __stdcall AddRef()=0;
 virtual unsigned long __stdcall Release()=0;
 virtual long __stdcall SetProperties(void*,void*)=0;
 virtual long __stdcall GetProperties(void*)=0;
 virtual long __stdcall Commit()=0;
 virtual long __stdcall Decommit()=0;
 virtual long __stdcall GetBuffer(void**,const void*,const void*,unsigned long)=0;
 virtual long __stdcall ReleaseBuffer(void*)=0;
 virtual long __stdcall SetNotify(void*)=0;
 virtual long __stdcall GetFreeCount(long*)=0;
};
struct CCritSecAllocator {CRITICAL_SECTION_ m_CritSec;CCritSecAllocator(){InitializeCriticalSection(&m_CritSec);}};
struct CAllocatorSampleList {void* First;long Count;CAllocatorSampleList():First(0),Count(0){}};
struct __declspec(novtable) CBaseAllocatorNative:CUnknownAllocator,IMemAllocatorCallbackTemp,CCritSecAllocator {
 CAllocatorSampleList m_lFree;
 void* m_hSem;long m_lWaiting,m_lSize,m_lCount,m_lAllocated,m_lAlignment,m_lPrefix;
 long m_bChanged,m_bCommitted,m_bDecommitInProgress;
 void* m_pNotify;long m_fEnableReleaseCallback;
 __forceinline CBaseAllocatorNative(IUnknown* outer,long* result):CUnknownAllocator(0,outer),m_lFree(),m_hSem(0),m_lWaiting(0),m_lSize(0),m_lCount(0),m_lAllocated(0),m_lAlignment(0),m_lPrefix(0),m_bChanged(0),m_bCommitted(0),m_bDecommitInProgress(0),m_pNotify(0),m_fEnableReleaseCallback(1){m_hSem=CreateSemaphoreA(0,0,0x7fffffff,0);if(!m_hSem)*result=(long)0x8007000e;}
};
struct CMemAllocatorNative:CBaseAllocatorNative {
 void* m_pBuffer;
 __forceinline CMemAllocatorNative(IUnknown* outer,long* result):CBaseAllocatorNative(outer,result),m_pBuffer(0){}
 virtual long __stdcall NonDelegatingQueryInterface(const void*,void**);
 virtual unsigned long __stdcall NonDelegatingAddRef();
 virtual unsigned long __stdcall NonDelegatingRelease();
 virtual long __stdcall QueryInterface(const void*,void**);
 virtual unsigned long __stdcall AddRef();virtual unsigned long __stdcall Release();
 virtual long __stdcall SetProperties(void*,void*);virtual long __stdcall GetProperties(void*);
 virtual long __stdcall Commit();virtual long __stdcall Decommit();
 virtual long __stdcall GetBuffer(void**,const void*,const void*,unsigned long);virtual long __stdcall ReleaseBuffer(void*);
 virtual long __stdcall SetNotify(void*);virtual long __stdcall GetFreeCount(long*);
 virtual ~CMemAllocatorNative();
};
CMemAllocatorNative* __stdcall CreateInstance(IUnknown* outer,long* result){return new CMemAllocatorNative(outer,result);}