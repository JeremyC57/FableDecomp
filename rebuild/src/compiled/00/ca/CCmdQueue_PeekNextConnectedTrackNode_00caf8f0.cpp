// NativeDeferredCommandQueue::GetDueCommand -- native projection of Microsoft DirectShow CCmdQueue::GetDueCommand.
// Provenance: microsoft/Windows-classic-samples, Win7Samples/multimedia/directshow/baseclasses/ctlutil.cpp lines2428-2480; reftime.h below is verbatim.
// ctlutil.cpp SHA256 8bb02b6f6940abbb29581992f5fe9c5fe7613eb5973fd6a39c2e8ff3da548b29.
// Source/native agree on both lists, running/stream origin, RAII lock, timestamp selection, CheckTime, COM AddRef and wait.
// Native CAF8F0,276bytes. DirectShow-style deferred commands: lists20/38, clock60, running64, stream origin68.
// CAF500 constructor proves critical section8 (24bytes), lists20/38 (24each), event50, clock60 and origin68.
// CB13A0 is actual CBaseList-style Get(position): null->null, otherwise node->Data+8, ret4.
// CAE170 IsDue(time64,streamRelativeBOOL) uses IReferenceClock::GetTime slot3, adds origin only if stream-relative.
// Command IUnknown subobject+C; timestamp18 and relativeBOOL30. IUnknown AddRef slot1 is genuine stdcall.
// Win32 IAT Enter143FE0C,Leave143FE10,WaitForSingleObject143FEAC. No game class identity asserted.
// Leaf: GetDueCommand
#include <stddef.h>
#include "rebuild_abi.h"
#ifndef NATIVE_IMPORT
#ifdef _MSC_VER
#define NATIVE_IMPORT __declspec(dllimport)
#else
#define NATIVE_IMPORT
#endif
#endif
struct NativeCriticalSection {void*DebugInfo;long LockCount,RecursionCount;void*OwningThread;void*LockSemaphore;unsigned long SpinCount;};
extern "C" {NATIVE_IMPORT void __stdcall EnterCriticalSection(NativeCriticalSection*);NATIVE_IMPORT void __stdcall LeaveCriticalSection(NativeCriticalSection*);NATIVE_IMPORT unsigned long __stdcall WaitForSingleObject(void*,unsigned long);}
// Win32 scalar aliases used by the unmodified public reference-time header.
typedef __int64 LONGLONG;
typedef __int64 REFERENCE_TIME;
typedef long LONG;
#ifndef Int32x32To64
#define Int32x32To64(a,b) ((__int64)(long)(a)*(long)(b))
#endif
// BEGIN ACTUAL MICROSOFT HEADER reftime.h (SHA256 dda8ce17126c1f7c6f9fa6ad64586b7f74922281b7879cf2dc3fe4b8fb5700b9)
//------------------------------------------------------------------------------
// File: RefTime.h
//
// Desc: DirectShow base classes - defines CRefTime, a class that manages
//       reference times.
//
// Copyright (c) 1992-2001 Microsoft Corporation. All rights reserved.
//------------------------------------------------------------------------------


//
// CRefTime
//
// Manage reference times.
// Shares same data layout as REFERENCE_TIME, but adds some (nonvirtual)
// functions providing simple comparison, conversion and arithmetic.
//
// A reference time (at the moment) is a unit of seconds represented in
// 100ns units as is used in the Win32 FILETIME structure. BUT the time
// a REFERENCE_TIME represents is NOT the time elapsed since 1/1/1601 it
// will either be stream time or reference time depending upon context
//
// This class provides simple arithmetic operations on reference times
//
// keep non-virtual otherwise the data layout will not be the same as
// REFERENCE_TIME


// -----
// note that you are safe to cast a CRefTime* to a REFERENCE_TIME*, but
// you will need to do so explicitly
// -----


#ifndef __REFTIME__
#define __REFTIME__


const LONGLONG MILLISECONDS = (1000);            // 10 ^ 3
const LONGLONG NANOSECONDS = (1000000000);       // 10 ^ 9
const LONGLONG UNITS = (NANOSECONDS / 100);      // 10 ^ 7

/*  Unfortunately an inline function here generates a call to __allmul
    - even for constants!
*/
#define MILLISECONDS_TO_100NS_UNITS(lMs) \
    Int32x32To64((lMs), (UNITS / MILLISECONDS))

class CRefTime
{
public:

    // *MUST* be the only data member so that this class is exactly
    // equivalent to a REFERENCE_TIME.
    // Also, must be *no virtual functions*

    REFERENCE_TIME m_time;

    inline CRefTime()
    {
        // default to 0 time
        m_time = 0;
    };

    inline CRefTime(LONG msecs)
    {
        m_time = MILLISECONDS_TO_100NS_UNITS(msecs);
    };

    inline CRefTime(REFERENCE_TIME rt)
    {
        m_time = rt;
    };

    inline operator REFERENCE_TIME() const
    {
        return m_time;
    };

    inline CRefTime& operator=(const CRefTime& rt)
    {
        m_time = rt.m_time;
        return *this;
    };

    inline CRefTime& operator=(const LONGLONG ll)
    {
        m_time = ll;
        return *this;
    };

    inline CRefTime& operator+=(const CRefTime& rt)
    {
        return (*this = *this + rt);
    };

    inline CRefTime& operator-=(const CRefTime& rt)
    {
        return (*this = *this - rt);
    };

    inline LONG Millisecs(void)
    {
        return (LONG)(m_time / (UNITS / MILLISECONDS));
    };

    inline LONGLONG GetUnits(void)
    {
        return m_time;
    };
};

const LONGLONG TimeZero = 0;

#endif /* __REFTIME__ */


// END ACTUAL MICROSOFT HEADER reftime.h
// Genuine CAMEvent declaration from Microsoft wxutil.h. Only platform aliases and
// annotation macros are supplied here; field and HANDLE conversion are unchanged.
typedef void* HANDLE;typedef int BOOL;typedef long HRESULT;typedef unsigned long DWORD;
#define __inout_opt
#define FALSE 0
#define INFINITE 0xffffffffUL
#define WAIT_OBJECT_0 0
#define EXECUTE_ASSERT(x) ((void)(x))
extern "C" {NATIVE_IMPORT BOOL __stdcall SetEvent(HANDLE);NATIVE_IMPORT BOOL __stdcall ResetEvent(HANDLE);}
class CAMEvent
{

    // make copy constructor and assignment operator inaccessible

    CAMEvent(const CAMEvent &refEvent);
    CAMEvent &operator=(const CAMEvent &refEvent);

protected:
    HANDLE m_hEvent;
public:
    CAMEvent(BOOL fManualReset = FALSE, __inout_opt HRESULT *phr = NULL);
    CAMEvent(__inout_opt HRESULT *phr);
    ~CAMEvent();

    // Cast to HANDLE - we don't support this as an lvalue
    operator HANDLE () const { return m_hEvent; };

    void Set() {EXECUTE_ASSERT(SetEvent(m_hEvent));};
    BOOL Wait(DWORD dwTimeout = INFINITE) {
	return (WaitForSingleObject(m_hEvent, dwTimeout) == WAIT_OBJECT_0);
    };
    void Reset() { ResetEvent(m_hEvent); };
    BOOL Check() { return Wait(0); };
};
struct NativeGuid;
struct NativeUnknown;
// Concrete ABI view of the genuine IUnknown subobject: all three stdcall slots.
// Native CDeferredCommand implements these; fixture definitions are out-of-line.
// No virtual destructor: IUnknown has exactly QueryInterface/AddRef/Release.
struct NativeUnknown {
 virtual long __stdcall QueryInterface(const NativeGuid&,void**);
 virtual unsigned long __stdcall AddRef();
 virtual unsigned long __stdcall Release();
};
struct DeferredCommandNative {unsigned char Unrecovered00[12];NativeUnknown Interface;unsigned char Unrecovered10[8];REFERENCE_TIME Time;unsigned char Unrecovered20[16];BOOL StreamRelative;CRefTime GetTime(){return Time;}BOOL IsStreamTime(){return StreamRelative;}};
struct NativeCommandNode {NativeCommandNode*Next,*Previous;DeferredCommandNative*Data;};
struct NativeCommandList {NativeCommandNode*First,*Last;long Count;unsigned char Unrecovered0C[12];int GetCount()const{return Count;}NativeCommandNode*GetHeadPosition()const{return First;}DeferredCommandNative*Get(NativeCommandNode*)const;};
struct NativeLockGuard {NativeCriticalSection*Section;NativeLockGuard(NativeCriticalSection*s):Section(s){EnterCriticalSection(Section);}~NativeLockGuard(){LeaveCriticalSection(Section);}};
struct NativeDeferredCommandQueue {unsigned char Unrecovered00[8];NativeCriticalSection Lock;NativeCommandList Commands,StreamCommands;CAMEvent Event;unsigned char Unrecovered54[12];void*Clock;BOOL Running;CRefTime StreamOrigin;
 BOOL CheckTime(CRefTime,BOOL);long GetDueCommand(DeferredCommandNative**,long);
};
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(sizeof(CAMEvent)==sizeof(void*));
FABLE_STATIC_ASSERT(sizeof(NativeCriticalSection)==24);
FABLE_STATIC_ASSERT(offsetof(DeferredCommandNative,Interface)==0xc);
FABLE_STATIC_ASSERT(offsetof(DeferredCommandNative,Time)==0x18);
FABLE_STATIC_ASSERT(offsetof(DeferredCommandNative,StreamRelative)==0x30);
FABLE_STATIC_ASSERT(offsetof(NativeDeferredCommandQueue,Commands)==0x20);
FABLE_STATIC_ASSERT(offsetof(NativeDeferredCommandQueue,StreamCommands)==0x38);
FABLE_STATIC_ASSERT(offsetof(NativeDeferredCommandQueue,Event)==0x50);
FABLE_STATIC_ASSERT(offsetof(NativeDeferredCommandQueue,Running)==0x64);
FABLE_STATIC_ASSERT(offsetof(NativeDeferredCommandQueue,StreamOrigin)==0x68);
#endif
long NativeDeferredCommandQueue::GetDueCommand(DeferredCommandNative**out,long timeout){
 for(;;){
  {NativeLockGuard guard(&Lock);DeferredCommandNative*command=0;
   if(Commands.GetCount()>0)command=Commands.Get(Commands.GetHeadPosition());
   if(Running&&StreamCommands.GetCount()>0){DeferredCommandNative*stream=StreamCommands.Get(StreamCommands.GetHeadPosition());CRefTime due=stream->GetTime()+StreamOrigin;if(!command||due<command->GetTime())command=stream;}
   if(command&&CheckTime(command->GetTime(),command->IsStreamTime())){command->Interface.AddRef();*out=command;return 0;}
  }
  if(WaitForSingleObject(Event,timeout)!=0)return (long)0x80004004;
 }
}