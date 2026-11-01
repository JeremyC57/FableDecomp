#include <stdio.h>
#include <stddef.h>
#define protected public
#ifndef _MSC_VER
#define __cdecl
#define __stdcall
#define __fastcall
#define __forceinline inline
#endif
#define __declspec(x)
#define WaitForSingleObjectEx TestWaitForSingleObjectEx
#define SetEvent TestSetEvent
// 009c5690 CFileInstaller::ThreadMain
// Leaf: ?ThreadMain@CFileInstaller@@IAEXXZ
#include <new>
// Win32 ABI declarations: original PDB section type and Microsoft SDK prototypes.
struct _RTL_CRITICAL_SECTION_DEBUG;struct _SECURITY_ATTRIBUTES;
struct _RTL_CRITICAL_SECTION{_RTL_CRITICAL_SECTION_DEBUG*DebugInfo;long LockCount,RecursionCount;void*OwningThread;void*LockSemaphore;unsigned long SpinCount;};
extern "C" __declspec(dllimport) void __stdcall EnterCriticalSection(_RTL_CRITICAL_SECTION*);
extern "C" __declspec(dllimport) void __stdcall LeaveCriticalSection(_RTL_CRITICAL_SECTION*);
extern "C" __declspec(dllimport) void* __stdcall CreateFileW(const wchar_t*,unsigned long,unsigned long,_SECURITY_ATTRIBUTES*,unsigned long,unsigned long,void*);
extern "C" __declspec(dllimport) unsigned long __stdcall GetFileSize(void*,unsigned long*);
extern "C" __declspec(dllimport) unsigned long __stdcall SetFilePointer(void*,long,long*,unsigned long);
extern "C" __declspec(dllimport) int __stdcall SetEndOfFile(void*);
// Begin shared header engine/CCountedPointer.h
#ifndef FABLE_ENGINE_CCOUNTEDPOINTER_H
#define FABLE_ENGINE_CCOUNTEDPOINTER_H
// Retail model (hand-written 2026-10-30, class-model migration wave 9): the engine's reference-counted owning pointer and
// its shared count block. Not polymorphic (no vtable, no RTTI); every member is inline in the release build.
//
// CCPPointerInfo (Ego_r.pdb, 12 bytes): RefCount +0x0 (ulong), DeleteFunc +0x4, Data +0x8 (void*). DeleteFunc is
//   __fastcall: every retail release loads `mov ecx,[eax+8]; call [eax+4]` (0x006E7AB0, 0x00CE1000, 0x00CE1040, 0x008AB2B0)
//   and the deleters it points at are `SIXPAX@Z` (static __fastcall void(void*)) CCountedPointer<T>::DeleteData in
//   FableWin.pdb (343 instantiations); Ego_r.pdb's `_func___cdecl_void_void_ptr` is the PDB's generic spelling.
//   Ctor: FableWin.pdb ??0CCPPointerInfo@@QAE@KP6IXPAX@Z0@Z = (unsigned long, void (__fastcall*)(void*), void*); retail
//   Reset 0x008AB2B0 expands it after `new(0xc)`: [eax]=1, [eax+4]=deleter, [eax+8]=Data (member order).
//
// CCountedPointer<T> (Ego_r.pdb, 8 bytes for every T): Data +0x0 (T*), Info +0x4 (CCPPointerInfo*). Member set = FableWin.pdb
//   (the debug build emits them; Ego_r keeps out of line only ShareData / ReleaseData / Reset / DeleteData, the rest inline):
//   ??0 (), (T*), (const&), member template ??$?0 (const CCountedPointer<U>&); ??1; ??4 (const&), member template ??$?4;
//   ??C operator-> QBE, ??D operator* QBE; DeleteData SIXPAX@Z; Get / get QBE; GetReferenceCount QBEKXZ; IsNull QBE_NXZ;
//   ReleaseData QAEXXZ; Reset / reset QAEXPAV; ShareData QAEXPAV..PAVCCPPointerInfo@@@Z; Validate QBEXXZ (debug check);
//   member template ??$SetStaticCast (63). Not modelled: the free __fastcall templates ??$?8 / ??$?9 (operator== / !=
//   of two CCountedPointers, YI_NABV..0@Z) -- no landed includer compares two pointers.
// Bodies (retail bytes; no ICF in retail: 732 separate /O2 dtor COMDATs, one per T):
//   ~CCountedPointer = 0x006E7AB0 (MovieBase) / 0x00CE1000 (CEntityScriptBase) and the other 730: `if (Info) { dec [Info];
//     reload Info; cmp [Info],0; jne; ecx = Info->Data; call Info->DeleteFunc; operator delete(Info) }`, then Data = 0
//     and Info = 0 in that order. It is its own body, not a ReleaseData call (below).
//   ReleaseData: the same release, then Info = 0 and Data = 0 in THAT order. Retail never emits it out of line (no
//     release COMDAT with the Info-first tail exists); expanded at 0x00A45330 / 0x00A5F250 / 0x006633A0 (`mov [x+4],edi;
//     mov [x],edi`). Reset and ShareData are written over it (their dead zero stores vanish):
//   Reset(T*) = 0x008AB2B0 (and 197 more): <ReleaseData> leaves only `mov [esi+4],0`, then Data = data; if (data)
//     Info = new CCPPointerInfo(1, DeleteData, Data) (the block reads Data back; a failed new leaves Info 0).
//   ShareData = 0x00CE1040 (CCountedPointer<CScriptThing>, Ego_r.pdb name): `if (Info != info) { <release>; Data = data;
//     Info = info; if (info) ++info->RefCount; }`.
//   operator= = 0x00704580 (CScriptThing): loads o.Data / o.Info first, then the ShareData body, returns *this.
//   The (T*) ctor is the allocation without the release (out of line at 0x008C58F0 / 0x008C5980, `ret 4`).
//   Copy ctor: Data, Info copied, then `if (Info) ++Info->RefCount` (expanded in every CScriptThing copy 0x004ABE90).
//   DeleteData = `delete static_cast<T*>(data)`: 0x00CDEE00 (polymorphic T: `test ecx,ecx; je; mov eax,[ecx]; push 1;
//     call [eax]`), 0x00674E80 (CExpressionSet: the inline implicit dtor, then operator delete). Only its address is
//     stored (the count block); the landed DeleteData rows explicitly instantiate it.
// Model test (wave 9, 955 landed rows, logs/mc_cpvar_*.txt): this split (dtor own body, ReleaseData zeroing Info first)
//   gives 15 DIFFER; a dtor that calls a zeroing ReleaseData gives the same leaf bytes at more sites but names ReleaseData
//   where retail calls the Data-first dtor COMDAT, and `ReleaseData(); Data = 0; Info = 0;` gives 20 DIFFER.
// Inlining: every member is plain `inline` (in-class), never __forceinline / noinline. Retail calls the COMDAT copies at
//   some sites and expands them at others; VC7.1 RTM 3077 reproduces that per-site choice from these bodies on the landed
//   rows (wave 9 HEADERS.md lists the shapes it does not reproduce). unproven: the source order of Reset's two stores and
//   the bodies of GetReferenceCount / reset / Get / get / IsNull (FableWin names only, no retail out-of-line copy found).
// Begin shared header rebuild_abi.h
#ifndef TEST_EMBED_3AC84EBD4026
#define TEST_EMBED_3AC84EBD4026


// ABI primitives for reconstructed FableTLC modules. Keep this header valid
// under VC7.1; modern-only language/library features belong outside Tier 0.
#if defined(_MSC_VER)
#  define FABLE_CDECL __cdecl
#  define FABLE_STDCALL __stdcall
#  define FABLE_FASTCALL __fastcall
#  define FABLE_THISCALL __thiscall
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
#define FABLE_STATIC_ASSERT(expression) typedef char FABLE_JOIN(fable_sassert_, __LINE__)[(expression) ? 1 : -1]

#endif

// End shared header rebuild_abi.h

class CCPPointerInfo {
public:
    unsigned long RefCount;                                              // +0x0
    void (FABLE_FASTCALL* DeleteFunc)(void*);                             // +0x4
    void* Data;                                                          // +0x8
    CCPPointerInfo(unsigned long refCount, void (FABLE_FASTCALL* deleteFunc)(void*), void* data)
        : RefCount(refCount), DeleteFunc(deleteFunc), Data(data) { }     // FableWin.pdb ??0CCPPointerInfo@@QAE@KP6IXPAX@Z0@Z
};
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(sizeof(CCPPointerInfo) == 0xc);
#endif
#endif

template <class T> class CCountedPointer {
public:
    T* Data;                                                             // +0x0
    CCPPointerInfo* Info;                                                // +0x4

    CCountedPointer() : Data(0), Info(0) { }
    CCountedPointer(T* data) : Data(data)
    {
        if (data)
            Info = new CCPPointerInfo(1, DeleteData, (void*)Data);
        else
            Info = 0;
    }
    CCountedPointer(const CCountedPointer& other) : Data(other.Data), Info(other.Info)
    {
        if (Info)
            ++Info->RefCount;
    }
    template <class U> CCountedPointer(const CCountedPointer<U>& other) : Data(other.Data), Info(other.Info)
    {
        if (Info)
            ++Info->RefCount;
    }
    ~CCountedPointer()
    {
        if (Info) {
            --Info->RefCount;
            if (Info->RefCount == 0) {
                Info->DeleteFunc(Info->Data);
                delete Info;
            }
        }
        Data = 0;
        Info = 0;
    }
    CCountedPointer& operator=(const CCountedPointer& other)
    {
        ShareData(other.Data, other.Info);
        return *this;
    }
    template <class U> CCountedPointer& operator=(const CCountedPointer<U>& other)
    {
        ShareData(other.Data, other.Info);
        return *this;
    }
    T* operator->() const { return Data; }
    T& operator*() const { return *Data; }
    T* Get() const { return Data; }
    T* get() const { return Data; }
    bool IsNull() const { return Data == 0; }
    unsigned long GetReferenceCount() const { return Info ? Info->RefCount : 0; }   // unproven body (FableWin name only)
    void Validate() const;                                               // FableWin.pdb only (debug build check)
    void ReleaseData()
    {
        if (Info) {
            --Info->RefCount;
            if (Info->RefCount == 0) {
                Info->DeleteFunc(Info->Data);
                delete Info;
            }
        }
        Info = 0;
        Data = 0;
    }
    void ShareData(T* data, CCPPointerInfo* info)
    {
        if (Info != info) {
            ReleaseData();
            Data = data;
            Info = info;
            if (info)
                ++info->RefCount;
        }
    }
    void Reset(T* data)
    {
        ReleaseData();
        Data = data;
        if (data)
            Info = new CCPPointerInfo(1, DeleteData, (void*)Data);
    }
    template <class U> void SetStaticCast(const CCountedPointer<U>& other)   // FableWin.pdb ??$SetStaticCast@..@?$CCountedPointer@..@@QAEXABV?$CCountedPointer@..@@@Z (63)
    {
        ShareData(static_cast<T*>(other.Data), other.Info);              // landed 0x00A32620 (C3DMeshFile::GetRoot) expands it
    }
    void reset(T* data) { Reset(data); }                                 // unproven body (FableWin name only)
    static void FABLE_FASTCALL DeleteData(void* data) { delete static_cast<T*>(data); }                    // per-T deleter, address stored in the count block
};

#endif // FABLE_ENGINE_CCOUNTEDPOINTER_H

// End shared header engine/CCountedPointer.h
#define CCriticalSectionLock CCriticalSectionLockFields
// Begin shared header engine/CCriticalSectionLock.h
#ifndef FABLE_ENGINE_CCRITICALSECTIONLOCK_H
#define FABLE_ENGINE_CCRITICALSECTIONLOCK_H
// GENERATED by tools/decomp_pipeline/gen_class_headers.py -- do not edit by hand.
// Donor: Ego_r.pdb (ghidra_out/struct_layouts_egor.tsv) sizeof CCriticalSectionLock = 0x8.
// Retail evidence: 0 trusted landed function(s); reconciliation clean.
// Members are original Lionhead names; `_pad_*` are unnamed bytes; opaque
// `unsigned char X[N]` members carry their real (composite/template) type in a comment.
#include <stddef.h>
// Begin shared header rebuild_abi.h
#ifndef TEST_EMBED_3AC84EBD4026
#define TEST_EMBED_3AC84EBD4026


// ABI primitives for reconstructed FableTLC modules. Keep this header valid
// under VC7.1; modern-only language/library features belong outside Tier 0.
#if defined(_MSC_VER)
#  define FABLE_CDECL __cdecl
#  define FABLE_STDCALL __stdcall
#  define FABLE_FASTCALL __fastcall
#  define FABLE_THISCALL __thiscall
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
#define FABLE_STATIC_ASSERT(expression) typedef char FABLE_JOIN(fable_sassert_, __LINE__)[(expression) ? 1 : -1]

#endif

// End shared header rebuild_abi.h

struct _RTL_CRITICAL_SECTION;

#pragma pack(push, 1)
struct CCriticalSectionLock {
    _RTL_CRITICAL_SECTION* CriticalSection;                  // +0x00
    bool                   Locked;                           // +0x04
    unsigned char          _pad_0x05[0x3];                   // +0x05
};
#pragma pack(pop)

#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(sizeof(CCriticalSectionLock) == 0x8);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CCriticalSectionLock, CriticalSection) == 0x0);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CCriticalSectionLock, Locked) == 0x4);
#endif
#endif

#endif // FABLE_ENGINE_CCRITICALSECTIONLOCK_H

// End shared header engine/CCriticalSectionLock.h
#undef CCriticalSectionLock
// Layout: shared lock storage supplies its two native fields; original reference
// constructor and destructor are restored from both PDBs and a62920/a629a0.
class CCriticalSectionLock:public CCriticalSectionLockFields{public:CCriticalSectionLock(_RTL_CRITICAL_SECTION&,bool);~CCriticalSectionLock();void Release();void Lock();};

extern "C" __declspec(dllimport) unsigned long __stdcall WaitForSingleObjectEx(void*,unsigned long,int);
extern "C" __declspec(dllimport) int __stdcall SetEvent(void*);
// Layout: STLport 4.6 list holds its sentinel pointer; empty compares next to it.
namespace std{template<class T>class allocator;struct _List_node_base{_List_node_base*_M_next;_List_node_base*_M_prev;};template<class T,class A=allocator<T> >class list{protected:_List_node_base*_M_node;public:bool empty()const{return _M_node->_M_next==_M_node;}};}
template<class T>class CTBaseSingleton{public:virtual~CTBaseSingleton();static T&__fastcall Get();};
// Layout: singleton vptr0, section4; native member accesses prove the release
// offsets below. Byte ranges retain unobserved fields without guessing types.
class CFileInstaller:public CTBaseSingleton<CFileInstaller>{
protected:
 class CFileInfo;class CActiveRead;class CPendingWrite;
 class CActiveFile{public:unsigned char unobservedPrefix[0x90];std::list<CPendingWrite>PendingWrites;std::list<CActiveRead*>PendingDecompression;};
 _RTL_CRITICAL_SECTION CriticalSection;
 unsigned char unobservedBuffers[0x34-0x1c];
 long AllocatedInputBuffers;
 std::list<CCountedPointer<const CFileInfo> >FilesToCopy,FilesToFinish;
 unsigned char unobservedCollections[0x50-0x40];
 CActiveFile*PCurrentReadFile;CActiveFile*PCurrentWriteFile;
 long NoActiveReads,NoActiveWrites;bool WriteMode;
 unsigned long BufferChunkSize,NoBufferChunks;
 long NoTotalBytesWritten,TotalSizeOfAllFiles;
 CActiveRead*PCompletedRead;
 bool ThreadFinished,DecompressionThreadFinished,ExitThread,Running;
 unsigned long Paused;bool Completed,IgnoreTimeStampWhenCopying;
 void*Event;void*DecompressionEvent;void*PauseStartEvent;void*PauseFinishEvent;
 void ProcessActiveFileForWrite(CActiveFile&);
 void ProcessReads();void PostFileReadCompletion(CActiveRead*);void ProcessAlways();
 void ThreadMain();
};
void CFileInstaller::ThreadMain()
{
    while (!ExitThread) {
        CCriticalSectionLock lock(CriticalSection, false);
        if (!FilesToCopy.empty())
            break;
        lock.Release();
        WaitForSingleObjectEx(Event, 0xffffffff, 1);
    }
    Running = true;
    while (!ExitThread) {
        CCriticalSectionLock lock(CriticalSection, false);
        if (Paused) {
            while (NoActiveReads > 0 || NoActiveWrites > 0) {
                lock.Release();
                WaitForSingleObjectEx(Event, 0xffffffff, 1);
                lock.Lock();
                CCriticalSectionLock readLock(CriticalSection, false);
                if (PCompletedRead) {
                    PostFileReadCompletion(PCompletedRead);
                    PCompletedRead = 0;
                }
            }
            lock.Release();
            SetEvent(PauseStartEvent);
            WaitForSingleObjectEx(PauseFinishEvent, 0xffffffff, 1);
            lock.Lock();
        }
        if (WriteMode) {
            if (PCurrentWriteFile && NoActiveWrites + NoActiveReads < 1)
                ProcessActiveFileForWrite(*PCurrentWriteFile);
        } else {
            ProcessReads();
        }
        ProcessAlways();
        if (WriteMode) {
            if (AllocatedInputBuffers * 2 < NoBufferChunks
                && (PCurrentWriteFile == 0 || (PCurrentWriteFile->PendingWrites.empty() && PCurrentWriteFile->PendingDecompression.empty())))
                WriteMode = false;
        } else {
            if (AllocatedInputBuffers * 2 >= NoBufferChunks || (PCurrentReadFile == 0 && !FilesToFinish.empty()))
                WriteMode = true;
        }
        lock.Release();
        WaitForSingleObjectEx(Event, 100, 1);
    }
    CCriticalSectionLock lock(CriticalSection, false);
    ThreadFinished = true;
    Running = false;
    lock.Release();
}


#undef protected
class CFileInstaller::CActiveRead {};
template<class T> CTBaseSingleton<T>::~CTBaseSingleton(){}
static CFileInstaller*instance;
static int failures,locks,constructed,released,waitInitial,waitDrain,waitPause,waitCycle,readCalls,writeCalls,processCalls,postCalls,pauseSignals;
static std::_List_node_base*copyHead;static std::_List_node_base copyNode;
static int initialEmpty;
static void Check(bool ok){if(!ok)++failures;}
CCriticalSectionLock::CCriticalSectionLock(_RTL_CRITICAL_SECTION&cs,bool onlyTry){CriticalSection=&cs;Locked=true;++locks;++constructed;Check(&cs==&instance->CriticalSection && !onlyTry);}
CCriticalSectionLock::~CCriticalSectionLock(){if(Locked){--locks;++released;Locked=false;}}
void CCriticalSectionLock::Release(){Check(Locked);Locked=false;--locks;++released;}
void CCriticalSectionLock::Lock(){Check(!Locked);Locked=true;++locks;++constructed;}
extern "C" unsigned long __stdcall WaitForSingleObjectEx(void*event,unsigned long ms,int alertable){
 Check(locks==0 && alertable==1);
 if(!instance->Running){Check(event==instance->Event && ms==0xffffffffUL);++waitInitial;copyNode._M_next=copyHead;copyNode._M_prev=copyHead;copyHead->_M_next=&copyNode;copyHead->_M_prev=&copyNode;}
 else if(ms==100){Check(event==instance->Event);++waitCycle;instance->ExitThread=true;}
 else if(event==instance->PauseFinishEvent){Check(pauseSignals==1 && ms==0xffffffffUL);++waitPause;}
 else{Check(event==instance->Event && ms==0xffffffffUL);++waitDrain;instance->NoActiveReads=instance->NoActiveWrites=0;}
 return 0;
}
extern "C" int __stdcall SetEvent(void*event){Check(event==instance->PauseStartEvent && locks==0);++pauseSignals;return 1;}
void CFileInstaller::PostFileReadCompletion(CActiveRead*p){Check(p==PCompletedRead && p!=0 && locks==2);++postCalls;}
void CFileInstaller::ProcessActiveFileForWrite(CActiveFile&f){Check(&f==PCurrentWriteFile && locks==1);++writeCalls;}
void CFileInstaller::ProcessReads(){Check(locks==1);++readCalls;}
void CFileInstaller::ProcessAlways(){Check(locks==1);++processCalls;}
#ifdef _MSC_VER
typedef char exit_offset[(offsetof(CFileInstaller,ExitThread)==0x7a)?1:-1];
typedef char complete_offset[(offsetof(CFileInstaller,PCompletedRead)==0x74)?1:-1];
typedef char pending_offset[(offsetof(CFileInstaller::CActiveFile,PendingWrites)==0x90)?1:-1];
#endif
int main(){int token[4],cases=0;
 for(int exitNow=0;exitNow<2;++exitNow)for(int empty=0;empty<2;++empty)for(int pause=0;pause<2;++pause)for(int busy=0;busy<2;++busy)for(int mode=0;mode<2;++mode)for(int allocated=0;allocated<3;++allocated)for(int combination=0;combination<8;++combination){
 CFileInstaller f;instance=&f;CFileInstaller::CActiveFile file;CFileInstaller::CActiveRead activeRead;
 std::_List_node_base copies,finish,pending,decompression,finishNode,pendingNode;
 copyNode._M_next=copyNode._M_prev=&copies;copies._M_next=copies._M_prev=empty?&copies:&copyNode;copyHead=&copies;
 bool finishNonempty=(combination&1)!=0;bool filePresent=(combination&2)!=0;bool pendingNonempty=(combination&4)!=0;
 finishNode._M_next=finishNode._M_prev=&finish;pendingNode._M_next=pendingNode._M_prev=&pending;finish._M_next=finish._M_prev=finishNonempty?&finishNode:&finish;pending._M_next=pending._M_prev=pendingNonempty?&pendingNode:&pending;decompression._M_next=decompression._M_prev=&decompression;
 f.FilesToCopy._M_node=&copies;f.FilesToFinish._M_node=&finish;file.PendingWrites._M_node=&pending;file.PendingDecompression._M_node=&decompression;
 f.PCurrentReadFile=0;f.PCurrentWriteFile=filePresent?&file:0;f.NoActiveReads=busy;f.NoActiveWrites=0;
 f.PCompletedRead=&activeRead;
 f.ThreadFinished=false;f.DecompressionThreadFinished=false;f.ExitThread=exitNow!=0;f.Running=false;f.Paused=pause;f.WriteMode=mode!=0;
 f.AllocatedInputBuffers=allocated;f.NoBufferChunks=2;f.Event=&token[0];f.PauseStartEvent=&token[1];f.PauseFinishEvent=&token[2];
 locks=constructed=released=waitInitial=waitDrain=waitPause=waitCycle=readCalls=writeCalls=processCalls=postCalls=pauseSignals=0;
 f.ThreadMain();
 Check(f.ThreadFinished && !f.Running && f.ExitThread && locks==0 && constructed==released);
 if(exitNow){Check(!waitInitial && !waitDrain && !waitPause && !waitCycle && !processCalls && !readCalls && !writeCalls && !postCalls && !pauseSignals);Check(f.WriteMode==(mode!=0));}
 else{
 Check(waitInitial==empty && waitDrain==(pause&&busy?1:0) && waitPause==pause && waitCycle==1 && pauseSignals==pause);
 Check(processCalls==1 && readCalls==(mode?0:1) && writeCalls==(mode&&filePresent&&(!busy||pause)?1:0));
 Check(postCalls==(pause&&busy?1:0));if(postCalls)Check(f.PCompletedRead==0);
 bool expected=mode!=0;if(mode){if(allocated*2<2 && (!filePresent || !pendingNonempty))expected=false;}else if(allocated*2>=2 || finishNonempty)expected=true;
 Check(f.WriteMode==expected);
 }
 ++cases;
 }
 Check(cases==768);
 if(failures){fprintf(stderr,"failures: %d\n",failures);return 1;}
 puts("009c5690_TEST PASS");return 0;
}
