#define NATIVE_IMPORT
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <new>
#include <cstdio>
#include <cstring>
#ifndef _MSC_VER
#define __forceinline inline
#define __fastcall
#define __stdcall
#define __cdecl
#define __declspec(x)
#define __int64 long long
#endif
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
#pragma once

// ABI primitives for reconstructed FableTLC modules. Keep this header valid
// under VC7.1; modern-only language/library features belong outside Tier 0.
#if defined(_MSC_VER)
#  define FABLE_CDECL __cdecl
#  define FABLE_STDCALL __stdcall
#  define FABLE_FASTCALL __fastcall
#  define FABLE_THISCALL __fastcall
#else
#  define FABLE_CDECL
#  define FABLE_STDCALL
#  define FABLE_FASTCALL
#  define FABLE_THISCALL
#endif

typedef signed char fable_i8;
typedef unsigned char fable_u8;
typedef signed short fable_i16;
typedef unsigned short fable_u16;
typedef signed int fable_i32;
typedef unsigned int fable_u32;

typedef char fable_require_i8[sizeof(fable_i8) == 1 ? 1 : -1];
typedef char fable_require_i16[sizeof(fable_i16) == 2 ? 1 : -1];
typedef char fable_require_i32[sizeof(fable_i32) == 4 ? 1 : -1];
#ifdef _MSC_VER
typedef char fable_require_ptr32[sizeof(void*) == 4 ? 1 : -1];
#endif

#define FABLE_JOIN_INNER(a, b) a##b
#define FABLE_JOIN(a, b) FABLE_JOIN_INNER(a, b)
#ifndef _MSC_VER
#define FABLE_STATIC_ASSERT(expression)
#else
#define FABLE_STATIC_ASSERT(expression) \
    typedef char FABLE_JOIN(fable_abi_assert_, __LINE__)[(expression) ? 1 : -1]
#endif

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



#include <assert.h>
#include <new>
CAMEvent::CAMEvent(BOOL,HRESULT*):m_hEvent(0){}
CAMEvent::CAMEvent(HRESULT*):m_hEvent(0){}
CAMEvent::~CAMEvent(){}
static void SetEventHandle(CAMEvent&e,void*h){memcpy(&e,&h,sizeof h);}
static NativeDeferredCommandQueue*owner;static int mode,enters,leaves,waits,lookups,dueCalls,addCalls,locked;static DeferredCommandNative presentation,stream,outputSentinel;static NativeCommandNode pn,sn;static DeferredCommandNative**outputSlot;static __int64 checkedTime;static long checkedRelative;
extern "C" void __stdcall EnterCriticalSection(NativeCriticalSection*p){assert(p==&owner->Lock&&!locked);locked=1;++enters;}
extern "C" void __stdcall LeaveCriticalSection(NativeCriticalSection*p){assert(p==&owner->Lock&&locked);locked=0;++leaves;}
extern "C" unsigned long __stdcall WaitForSingleObject(void*event,unsigned long timeout){assert(!locked&&timeout==37);assert(event==(mode==13&&waits?&sn:&pn));++waits;assert(waits<3);if(mode==11){owner->Commands.Count=1;return 0;}if(mode==13&&waits==1){SetEventHandle(owner->Event,&sn);return 0;}return mode==12?0xffffffffUL:258;}
DeferredCommandNative*NativeCommandList::Get(NativeCommandNode*p)const{assert(locked&&((this==&owner->Commands&&p==&pn)||(this==&owner->StreamCommands&&p==&sn)));++lookups;if(mode==14)throw 14;return p?p->Data:0;}
BOOL NativeDeferredCommandQueue::CheckTime(CRefTime time,BOOL relative){assert(this==owner&&locked);++dueCalls;checkedTime=time;checkedRelative=relative;if(mode==15)throw 15;return mode!=10;}
static unsigned long __stdcall AddReference(NativeUnknown*p){assert(locked&&(p==&presentation.Interface||p==&stream.Interface));++addCalls;if(mode==16)throw 16;if(mode==17){owner->Commands.First=&sn;*outputSlot=&outputSentinel;}return 12;}
long __stdcall NativeUnknown::QueryInterface(const NativeGuid&,void**){assert(false);return -1;}
unsigned long __stdcall NativeUnknown::AddRef(){return AddReference(this);}
unsigned long __stdcall NativeUnknown::Release(){assert(false);return 0;}
int main(){
 for(mode=0;mode<19;++mode){
#if defined(_MSC_VER) && !defined(_CPPUNWIND)
 // Canonical /Od does not enable exception unwinding. The three throwing modes
 // remain in the host and explicitly /EHsc author/root runs, which cover all19.
 if(mode>=14&&mode<=16)continue;
#endif
 NativeDeferredCommandQueue q;memset(&q,0,sizeof q);owner=&q;memset(&presentation,0,sizeof presentation);memset(&stream,0,sizeof stream);new(&presentation.Interface)NativeUnknown;new(&stream.Interface)NativeUnknown;presentation.Time=100;stream.Time=20;stream.StreamRelative=1;pn.Data=&presentation;sn.Data=&stream;q.Commands.First=&pn;q.StreamCommands.First=&sn;SetEventHandle(q.Event,&pn);q.Running=1;q.StreamOrigin=50;q.Commands.Count=1;
 if(mode==0||mode==3||mode==11||mode==12||mode==13)q.Commands.Count=0;if(mode==18)q.Commands.Count=-1;if(mode==2){q.Commands.Count=0;q.StreamCommands.Count=1;}if(mode==3){q.Running=0;q.StreamCommands.Count=1;}if(mode>=4&&mode<=9)q.StreamCommands.Count=1;
 if(mode==5)stream.Time=50;if(mode==6)stream.Time=70;if(mode==7){presentation.Time=-5;stream.Time=-70;}if(mode==8){presentation.Time=((__int64)1<<32)+9;stream.Time=((__int64)1<<32)-1;q.StreamOrigin=2;}if(mode==9){presentation.Time=-10;stream.Time=15;q.StreamOrigin=-40;}
 enters=leaves=waits=lookups=dueCalls=addCalls=locked=0;DeferredCommandNative*out=&outputSentinel;outputSlot=&out;bool threw=false;long result=0;try{result=q.GetDueCommand(&out,37);}catch(int ex){assert(ex==mode&&(mode==14||mode==15||mode==16));threw=true;}
 bool exceptional=mode>=14&&mode<=16;bool success=!(mode==0||mode==3||mode==10||mode==12||mode==13||mode==18||exceptional);assert(threw==exceptional&&locked==0&&enters==leaves);assert(enters==(mode==11||mode==13?2:1));assert(waits==(mode==13?2:mode==0||mode==3||mode==10||mode==11||mode==12||mode==18?1:0));
 DeferredCommandNative*chosen=mode==2||mode==4||mode==7||mode==8||mode==9?&stream:&presentation;if(success){assert(result==0&&out==chosen&&addCalls==1&&dueCalls==1&&checkedTime==chosen->Time&&checkedRelative==chosen->StreamRelative);}else{assert(out==&outputSentinel);if(!exceptional)assert((unsigned long)result==0x80004004UL);assert(addCalls==(mode==16?1:0));}
 }
#if defined(_MSC_VER) && !defined(_CPPUNWIND)
 puts("00caf8f0_TEST PASS 16 clock-order/tie/64-bit/wake/timeout/COM cases; 3 exception-unwind cases require /EHsc");
#else
 puts("00caf8f0_TEST PASS 19 clock-order/tie/64-bit/wake/timeout/COM/EH-unlock cases");
#endif
 return 0;}