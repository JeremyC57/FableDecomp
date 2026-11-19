// agent680: src/009c3660.cpp body pasted unchanged; ExitThread volatile as in the src.
#include <stdio.h>
#include <string.h>
#include <stddef.h>
#define protected public
#ifndef _MSC_VER
#define __cdecl
#define __stdcall
#define __fastcall
#define __forceinline inline
#endif
#define __declspec(x)
#define CreateFileW TestCreateFileW
#define GetFileSize TestGetFileSize
#define SetFilePointer TestSetFilePointer
#define SetEndOfFile TestSetEndOfFile
#define EnterCriticalSection TestEnterCriticalSection
#define LeaveCriticalSection TestLeaveCriticalSection
// 009c3660 CFileInstaller::StartWritingFile
// Leaf: ?StartWritingFile@CFileInstaller@@IAEXAAVCActiveFile@1@K@Z
#include <new>
// Win32 ABI declarations: original PDB section type and Microsoft SDK prototypes.
struct _RTL_CRITICAL_SECTION_DEBUG;struct _SECURITY_ATTRIBUTES;
struct _RTL_CRITICAL_SECTION{_RTL_CRITICAL_SECTION_DEBUG*DebugInfo;long LockCount,RecursionCount;void*OwningThread;void*LockSemaphore;unsigned long SpinCount;};
extern "C" __declspec(dllimport) void __stdcall EnterCriticalSection(_RTL_CRITICAL_SECTION*);
extern "C" __declspec(dllimport) void __stdcall LeaveCriticalSection(_RTL_CRITICAL_SECTION*);
extern "C" __declspec(dllimport) void* __stdcall CreateFileW(const unsigned short*,unsigned long,unsigned long,_SECURITY_ATTRIBUTES*,unsigned long,unsigned long,void*);
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
#define FABLE_STATIC_ASSERT(expression) typedef char FABLE_JOIN(fable_layout_check_, __LINE__)[(expression) ? 1 : -1]

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
#define FABLE_STATIC_ASSERT(expression) typedef char FABLE_JOIN(fable_layout_check_, __LINE__)[(expression) ? 1 : -1]

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
class CCriticalSectionLock:public CCriticalSectionLockFields{public:CCriticalSectionLock(_RTL_CRITICAL_SECTION&,bool);~CCriticalSectionLock();};
class CWideStringData;
// Layout: release string contains its original data pointer; debug-only cache omitted.
class CWideString{protected:CWideStringData*PStringData;public:CWideString();CWideString(const CWideString&);~CWideString();operator const unsigned short*()const;};
class CAFile{public:static long __fastcall GetDiskSectorSize(const CWideString&);static void __fastcall CreateDirectoryForFile(const CWideString&);};
double __fastcall GFGetTime();
template<class T>class CTBaseSingleton{public:virtual~CTBaseSingleton();static T&__fastcall Get();};
// Layout: original singleton base supplies the native vptr. Native section4,
// current read50 and exit flag7a are independently visible in the assigned spans.
class CFileInstaller:public CTBaseSingleton<CFileInstaller>{
protected:
 class CFileInfo{public:CWideString SrcPath,DstPath;};
 // Layout: observed original active-file prefix through StartTime30; later
 // decompressor and queues are not constructed or treated as a complete layout.
 class CActiveFile{public:void*SrcFileHandle;void*DstFileHandle;unsigned long CurrentSrcOffset,NoBytesRead,NoBytesWritten,InputSize,OutputSize;CCountedPointer<const CFileInfo>PFileInfo;unsigned long MinWriteSize,MinReadSize;double StartTime;};
 _RTL_CRITICAL_SECTION CriticalSection;
 unsigned char unobservedBeforeRead[0x50-0x1c];CActiveFile*PCurrentReadFile;
 unsigned char unobservedBeforeExit[0x7a-0x54];volatile bool ExitThread;
 static bool DirtyDisc;
 static void __fastcall SetDirtyDiscError(bool error){
  CCriticalSectionLock lock(CTBaseSingleton<CFileInstaller>::Get().CriticalSection,false);
  DirtyDisc=error;
  CTBaseSingleton<CFileInstaller>::Get().ExitThread=CTBaseSingleton<CFileInstaller>::Get().ExitThread||error;
 }
 void StartReadingFile(CActiveFile&file);
 void StartWritingFile(CActiveFile&file,unsigned long outputSize);
};
void CFileInstaller::StartWritingFile(CActiveFile&file,unsigned long outputSize){
 CWideString path(file.PFileInfo->DstPath);
 LeaveCriticalSection(&CriticalSection);
 CAFile::CreateDirectoryForFile(path);
 file.OutputSize=outputSize;file.MinWriteSize=CAFile::GetDiskSectorSize(path);
 file.DstFileHandle=CreateFileW(path,0x40000000,0,0,2,0x68000000,0);
 file.StartTime=GFGetTime();
 if(file.DstFileHandle==(void*)-1)SetDirtyDiscError(true);
 unsigned long size=(outputSize+file.MinWriteSize-1)/file.MinWriteSize*file.MinWriteSize;
 SetFilePointer(file.DstFileHandle,size,0,0);SetEndOfFile(file.DstFileHandle);
 if(GetFileSize(file.DstFileHandle,0)!=size)SetDirtyDiscError(true);
 EnterCriticalSection(&CriticalSection);
}


#undef protected
class CWideStringData{public:unsigned short text[4];};
static CFileInstaller* instance;
static int failures,events[32],eventCount,copies,destroys,createFails,sizeMismatch,getCalls;
static unsigned long expectedAccess,expectedSharing,expectedCreation,expectedAttributes,inputSize,sectorSize,positionValue;
static const unsigned short* expectedPath;
static void* handleValue;
static void Check(bool ok){if(!ok)++failures;}
static void Event(int kind){Check(eventCount<32);if(eventCount<32)events[eventCount++]=kind;}
CWideString::CWideString():PStringData(0){}
CWideString::CWideString(const CWideString&o):PStringData(o.PStringData){++copies;}
CWideString::~CWideString(){++destroys;}
CWideString::operator const unsigned short*()const{return PStringData->text;}
template<class T> CTBaseSingleton<T>::~CTBaseSingleton(){}
template<class T> T& __fastcall CTBaseSingleton<T>::Get(){++getCalls;return static_cast<T&>(*instance);}
bool CFileInstaller::DirtyDisc=false;
CCriticalSectionLock::CCriticalSectionLock(_RTL_CRITICAL_SECTION&cs,bool tryOnly){CriticalSection=&cs;Locked=true;Check(!tryOnly && &cs==&instance->CriticalSection);Event(5);}
CCriticalSectionLock::~CCriticalSectionLock(){Check(Locked);Event(6);}
extern "C" void __stdcall LeaveCriticalSection(_RTL_CRITICAL_SECTION*cs){Check(cs==&instance->CriticalSection);Event(1);}
extern "C" void __stdcall EnterCriticalSection(_RTL_CRITICAL_SECTION*cs){Check(cs==&instance->CriticalSection);Event(9);}
extern "C" void* __stdcall CreateFileW(const unsigned short*p,unsigned long access,unsigned long sharing,_SECURITY_ATTRIBUTES*sa,unsigned long creation,unsigned long attributes,void*temp){Check(p==expectedPath && access==expectedAccess && sharing==expectedSharing && !sa && creation==expectedCreation && attributes==expectedAttributes && !temp);Event(2);return createFails?(void*)-1:handleValue;}
extern "C" unsigned long __stdcall GetFileSize(void*h,unsigned long*high){Check(h==(createFails?(void*)-1:handleValue));Event(3);if(high){*high=0x1234;return inputSize;}return positionValue+(sizeMismatch?1:0);}
extern "C" unsigned long __stdcall SetFilePointer(void*h,long p,long*high,unsigned long method){Check(h==(createFails?(void*)-1:handleValue) && !high && method==0);positionValue=(unsigned long)p;Event(7);return positionValue;}
extern "C" int __stdcall SetEndOfFile(void*h){Check(h==(createFails?(void*)-1:handleValue));Event(8);return 1;}
long __fastcall CAFile::GetDiskSectorSize(const CWideString&p){Check((const unsigned short*)p==expectedPath);Event(4);return(long)sectorSize;}
void __fastcall CAFile::CreateDirectoryForFile(const CWideString&p){Check((const unsigned short*)p==expectedPath);Event(10);}
double __fastcall GFGetTime(){Event(11);return 12.5;}
#ifdef _MSC_VER
typedef char read_offset[(offsetof(CFileInstaller,PCurrentReadFile)==0x50)?1:-1];
typedef char exit_offset[(offsetof(CFileInstaller,ExitThread)==0x7a)?1:-1];
typedef char time_offset[(offsetof(CFileInstaller::CActiveFile,StartTime)==0x30)?1:-1];
#endif
int main(){
 unsigned long sizes[6]={0,1,511,512,513,4096};unsigned long sectors[4]={1,2,512,2048};int token,cases=0;
 for(int si=0;si<6;++si)for(int ss=0;ss<4;++ss)for(int fail=0;fail<2;++fail)for(int exiting=0;exiting<2;++exiting)for(int mismatch=0;mismatch<2;++mismatch){
 CFileInstaller installer;instance=&installer;CFileInstaller::CActiveFile file;CFileInstaller::CFileInfo info;CWideStringData data;
 memset(data.text,0,sizeof(data.text));data.text[0]='x';info.SrcPath.PStringData=&data;info.DstPath.PStringData=&data;
 file.PFileInfo.Data=&info;file.PFileInfo.Info=0;file.InputSize=17;file.OutputSize=19;file.MinReadSize=23;file.MinWriteSize=29;file.StartTime=31.5;
 installer.ExitThread=exiting!=0;installer.PCurrentReadFile=0;CFileInstaller::DirtyDisc=false;
 expectedPath=data.text;handleValue=&token;inputSize=sizes[si];sectorSize=sectors[ss];createFails=fail;sizeMismatch=mismatch;
 eventCount=copies=destroys=getCalls=0;positionValue=0;
 expectedAccess=0x40000000;expectedSharing=0;expectedCreation=2;expectedAttributes=0x68000000;
 installer.StartWritingFile(file,inputSize);
 Check(file.OutputSize==inputSize && file.MinWriteSize==sectorSize && file.StartTime==12.5);Check(file.DstFileHandle==(fail?(void*)-1:handleValue));
 Check(positionValue==(inputSize+sectorSize-1)/sectorSize*sectorSize);
 int at=1;Check(events[at++]==10 && events[at++]==4 && events[at++]==2 && events[at++]==11);
 if(fail){Check(events[at++]==5 && events[at++]==6);}
 Check(events[at++]==7 && events[at++]==8 && events[at++]==3);
 if(mismatch){Check(events[at++]==5 && events[at++]==6);}Check(at+1==eventCount);
 Check(copies==1 && destroys==1);Check(events[0]==1 && events[eventCount-1]==9);
 Check(CFileInstaller::DirtyDisc==(fail!=0 || mismatch!=0));Check(installer.ExitThread==(exiting!=0 || fail!=0 || mismatch!=0));
 Check(getCalls==3*((fail?1:0)+(mismatch?1:0)));
 ++cases;
 }
 Check(cases==192);
 if(failures){fprintf(stderr,"failures: %d\n",failures);return 1;}
 puts("009c3660_TEST PASS");return 0;
}