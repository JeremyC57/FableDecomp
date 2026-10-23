// CMemAllocator::CreateInstance -- allocate DirectShow memory allocator, initialize lock/free-list/semaphore and buffer; retail00cac9d0 212bytes. Propagated engine label rejected: CUnknown00cacc80, criticalsection import0143fee4, CreateSemaphoreW0143fe54, vtables012bf83c/012bf810 match allocator destructor00caa410. This is no-RTTI strmbase; CMemAllocator owner inferred from allocator methods and buffer+60. No shared class header exists.
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
extern "C" __declspec(dllimport) void* __stdcall CreateSemaphoreW(void*,long,long,const unsigned short*); // import 0143fe54 = KERNEL32 CreateSemaphoreW (Fable.exe import table)
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
 __forceinline CBaseAllocatorNative(IUnknown* outer,long* result):CUnknownAllocator(0,outer),m_lFree(),m_hSem(0),m_lWaiting(0),m_lSize(0),m_lCount(0),m_lAllocated(0),m_lAlignment(0),m_lPrefix(0),m_bChanged(0),m_bCommitted(0),m_bDecommitInProgress(0),m_pNotify(0),m_fEnableReleaseCallback(1){m_hSem=CreateSemaphoreW(0,0,0x7fffffff,0);if(!m_hSem)*result=(long)0x8007000e;}
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

#include <cstdio>
#include <cstdlib>
#include <cstring>
struct IUnknown {unsigned long Marker;};
static int failures=0,allocations=0,deallocations=0,baseCalls=0,baseDestroyed=0;
static bool failAllocation=false,failSemaphore=false;
static IUnknown* expectedOuter=0;
static void check(bool b){if(!b)++failures;}
void* __cdecl operator new(size_t n){++allocations;if(failAllocation)return 0;void* p=std::malloc(n);if(p)std::memset(p,0xa5,n);return p;}
void __cdecl operator delete(void* p){if(p){++deallocations;std::free(p);}}
CUnknownAllocator::CUnknownAllocator(const char* name,IUnknown* outer):m_pUnknown(outer?outer:reinterpret_cast<IUnknown*>(this)),m_cRef(0){++baseCalls;check(!name && outer==expectedOuter);}
CUnknownAllocator::~CUnknownAllocator(){++baseDestroyed;}
#ifdef _MSC_VER
extern "C" __declspec(dllimport) int __stdcall CloseHandle(void*);
extern "C" __declspec(dllimport) void __stdcall DeleteCriticalSection(CRITICAL_SECTION_*);
#else
static int lockCalls=0,semaphoreCalls=0;
extern "C" void InitializeCriticalSection(CRITICAL_SECTION_* p){++lockCalls;check(p!=0);}
extern "C" void* CreateSemaphoreW(void* security,long initial,long maximum,const unsigned short* name){++semaphoreCalls;check(!security && initial==0 && maximum==0x7fffffff && !name);return failSemaphore?0:reinterpret_cast<void*>(0x1234);}
#endif
CMemAllocatorNative::~CMemAllocatorNative(){
#ifdef _MSC_VER
 if(m_hSem)check(CloseHandle(m_hSem)!=0);DeleteCriticalSection(&m_CritSec);
#endif
}
long CMemAllocatorNative::NonDelegatingQueryInterface(const void*,void**){return 0;}
unsigned long CMemAllocatorNative::NonDelegatingAddRef(){return ++m_cRef;}
unsigned long CMemAllocatorNative::NonDelegatingRelease(){return --m_cRef;}
long CMemAllocatorNative::QueryInterface(const void*,void**){return 0;}
unsigned long CMemAllocatorNative::AddRef(){return ++m_cRef;}
unsigned long CMemAllocatorNative::Release(){return --m_cRef;}
long CMemAllocatorNative::SetProperties(void*,void*){return 0;}
long CMemAllocatorNative::GetProperties(void*){return 0;}
long CMemAllocatorNative::Commit(){return 0;}
long CMemAllocatorNative::Decommit(){return 0;}
long CMemAllocatorNative::GetBuffer(void**,const void*,const void*,unsigned long){return 0;}
long CMemAllocatorNative::ReleaseBuffer(void*){return 0;}
long CMemAllocatorNative::SetNotify(void*){return 0;}
long CMemAllocatorNative::GetFreeCount(long*){return 0;}
static void run(bool allocationFails,bool semaphoreFails,bool outerPresent)
{
 failAllocation=allocationFails;failSemaphore=semaphoreFails;allocations=deallocations=baseCalls=baseDestroyed=0;
 IUnknown outer;outer.Marker=17;expectedOuter=outerPresent?&outer:0;long status=0x12345;
 CMemAllocatorNative* p=CreateInstance(expectedOuter,&status);
 check(allocations==1);
 if(allocationFails){check(!p && !baseCalls && !baseDestroyed && !deallocations && status==0x12345);return;}
 check(p && baseCalls==1 && !baseDestroyed && !deallocations);
 if(!p)return;
 check(p->m_pUnknown==(expectedOuter?expectedOuter:reinterpret_cast<IUnknown*>(p)) && p->m_cRef==0);
 check(!p->m_lFree.First && !p->m_lFree.Count && !p->m_lWaiting && !p->m_lSize && !p->m_lCount && !p->m_lAllocated && !p->m_lAlignment && !p->m_lPrefix && !p->m_bChanged && !p->m_bCommitted && !p->m_bDecommitInProgress && !p->m_pNotify && p->m_fEnableReleaseCallback==1 && !p->m_pBuffer);
 if(semaphoreFails)check(!p->m_hSem && (unsigned long)status==(unsigned long)(long)0x8007000e);
 else check(p->m_hSem && status==0x12345);
#ifdef _MSC_VER
 check(sizeof(CMemAllocatorNative)==0x64 && reinterpret_cast<unsigned char*>(&p->m_CritSec)-reinterpret_cast<unsigned char*>(p)==0x10 && reinterpret_cast<unsigned char*>(&p->m_pBuffer)-reinterpret_cast<unsigned char*>(p)==0x60);
#endif
 delete p;check(baseDestroyed==1 && deallocations==1);
}
int main(){run(false,false,false);run(false,false,true);run(true,false,false);
#ifndef _MSC_VER
 run(false,true,true);check(lockCalls==3 && semaphoreCalls==3);
#endif
 if(failures){std::printf("00cac9d0_TEST FAIL %d\n",failures);return 1;}std::printf("00cac9d0_TEST PASS\n");return 0;}