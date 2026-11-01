// CFileInstaller::ThreadMain() -- the installer worker loop: wait for files to copy, then until told to exit alternate read and write passes under the lock, servicing pause requests and completed reads, and finally mark the thread finished
// Leaf: ?ThreadMain@CFileInstaller@@IAEXXZ
// Retail-volatile: PCompletedRead (written by the read-completion thread): 0x009C5748 cmp [esi+74h],ebx ; je ; 0x009C574D mov edx,[esi+74h]
//   (re-read after the null test, as the landed 0x009C4F20). ExitThread (written by the main thread's shutdown / SetDirtyDiscError):
//   re-read every loop test, 0x009C56A4 / 0x009C56E2 / 0x009C56F2 / 0x009C582D. Running and ThreadFinished (read by the main
//   thread's IsRunning / shutdown wait): the stores stay in source order around the volatile reads, 0x009C56EE mov byte ptr [esi+7Bh],1
//   before 0x009C56F2 cmp [esi+7Ah],bl, and 0x009C5844 mov byte ptr [esi+78h],1 ahead of the lock release, then 0x009C584C mov [esi+7Bh],bl.
//   Owner 2026-11-01: recreate retail (precedent 0x009C4F20); without the four qualifiers 468v469, PCompletedRead alone 469v469 (8 lines).
// Retail: Fable.exe @ 0x009C5690, 469 bytes. Ego_r: protected ?ThreadMain@CFileInstaller@@IAEXXZ. Callees: CCriticalSectionLock
//   ctor/Lock/Release/dtor, PostFileReadCompletion, ProcessActiveFileForWrite, ProcessReads, ProcessAlways (PDB), Win32
//   WaitForSingleObjectEx / SetEvent through the import table.
// Shape: ChatGPT pass 10's body; only the member qualifiers change.
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
#include "engine/CCountedPointer.h"
#define CCriticalSectionLock CCriticalSectionLockFields
#include "engine/CCriticalSectionLock.h"
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
 CActiveRead*volatile PCompletedRead;
 volatile bool ThreadFinished;bool DecompressionThreadFinished;volatile bool ExitThread;volatile bool Running;
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