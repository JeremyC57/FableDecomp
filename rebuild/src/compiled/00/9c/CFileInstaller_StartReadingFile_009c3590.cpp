// CFileInstaller::StartReadingFile(CActiveFile& file) -- open the active file's source for reading outside the installer lock (dirty-disc error and thread exit on failure; otherwise its size, the disc sector read size and the start time), then make it the current read file
// Leaf: ?StartReadingFile@CFileInstaller@@IAEXAAVCActiveFile@1@@Z
// Retail-volatile: 0x009C35FC: mov al,[eax+7Ah] then 0x009C3608: mov byte ptr [eax+7Ah],1 -- the discarded read of ExitThread || error is kept, the VC7.1 codegen of a volatile member; ExitThread is written by the main thread (SetDirtyDiscError / shutdown) and polled by the installer threads. Owner 2026-11-01: recreate retail (precedent 0x009C4F20).
// Retail: Fable.exe @ 0x009C3590, 194 bytes, ret 4. Ego_r: protected ?StartReadingFile@CFileInstaller@@IAEXAAVCActiveFile@1@@Z.
//   Callees (PDB-bound): CWideString copy ctor / operator const unsigned short* / dtor, CTBaseSingleton<CFileInstaller>::Get,
//   CCriticalSectionLock ctor/dtor (the inline SetDirtyDiscError), CAFile::GetDiskSectorSize, GFGetTime; Win32 Leave/EnterCriticalSection,
//   CreateFileW, GetFileSize via the import table; CFileInstaller::DirtyDisc 0x013CA814.
// Shape: the lever ChatGPT pass 10 was not allowed: ExitThread volatile (owner rule 2026-11-01) keeps the discarded read of
//   ExitThread || error before the store of 1 (191v194 without it).
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
#include "engine/CCountedPointer.h"
#define CCriticalSectionLock CCriticalSectionLockFields
#include "engine/CCriticalSectionLock.h"
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
 unsigned char unobservedBeforeExit[0x7a-0x54];volatile bool ExitThread;   // +0x7A (volatile: see the Retail-volatile line)
 static bool DirtyDisc;
 static void __fastcall SetDirtyDiscError(bool error){
  CCriticalSectionLock lock(CTBaseSingleton<CFileInstaller>::Get().CriticalSection,false);
  DirtyDisc=error;
  CTBaseSingleton<CFileInstaller>::Get().ExitThread=CTBaseSingleton<CFileInstaller>::Get().ExitThread||error;
 }
 void StartReadingFile(CActiveFile&file);
 void StartWritingFile(CActiveFile&file,unsigned long outputSize);
};
void CFileInstaller::StartReadingFile(CActiveFile&file){
 CWideString path(file.PFileInfo->SrcPath);
 LeaveCriticalSection(&CriticalSection);
 file.SrcFileHandle=CreateFileW(path,0x80000000,1,0,3,0x68000001,0);
 if(file.SrcFileHandle==(void*)-1){SetDirtyDiscError(true);}
 else{
  unsigned long sizeHigh;
  file.InputSize=GetFileSize(file.SrcFileHandle,&sizeHigh);
  file.MinReadSize=CAFile::GetDiskSectorSize(path);
  file.StartTime=GFGetTime();
 }
 EnterCriticalSection(&CriticalSection);PCurrentReadFile=&file;
}