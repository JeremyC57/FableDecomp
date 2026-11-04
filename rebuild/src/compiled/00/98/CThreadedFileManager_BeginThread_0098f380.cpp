// CThreadedFileManager::BeginThread(void* manager) -- _beginthread entry of the threaded-file I/O thread: run above normal priority, then the manager's ThreadMain
// Leaf: ?BeginThread@CThreadedFileManager@@CAXPAX@Z
// Flags: none
// Retail: Fable.exe (TLC, VC7.1) @ 0x0098F380, 24 bytes (int3 padding to 0x0098F3A0), __cdecl static (the argument at [esp+4]),
//   tail jmp ThreadMain with ecx = the argument.
// Identity (two sources): (1) Ego_r.pdb ?BeginThread@CThreadedFileManager@@CAXPAX@Z (private static, the _beginthread signature) sits
//   between ?WaitForObjectToFinish@CThreadedFile@@QAEXK@Z and ??0CThreadedFileManager@@QAE@XZ (0x00AEA100 / 0x00AEA110 / 0x00AEA130),
//   as retail 0x0098F370 / 0x0098F380 / 0x0098F3A0; (2) it forwards to 0x0098F2A0 = CThreadedFileManager::ThreadMain (Ego_r
//   ?ThreadMain@CThreadedFileManager@@AAEXXZ, private), the same shape as CFileInstaller::BeginThread 0x009C5870. The catalog label
//   "CThreadedFileManager_InitiatePriority" is a stand-in.
// Callees: kernel32 GetCurrentThread (IAT 0x0143FDCC), kernel32 SetThreadPriority (IAT 0x0143FDD0), CThreadedFileManager::ThreadMain()
//   0x0098F2A0 (tail).
// Layout: only the vptr matters here (RTTI .?AVCThreadedFileManager@@, vtable 0x0129A164); no shared header exists.
// Shape: THREAD_PRIORITY_ABOVE_NORMAL (1) from winbase.h, the void* argument cast to the manager.
#include "rebuild_abi.h"
extern "C" __declspec(dllimport) void* __stdcall GetCurrentThread();                       // IAT 0x0143FDCC
extern "C" __declspec(dllimport) int __stdcall SetThreadPriority(void* thread, int priority);   // IAT 0x0143FDD0
#define THREAD_PRIORITY_ABOVE_NORMAL 1

class CThreadedFileManager {                                    // vtable 0x0129A164
public:
    virtual ~CThreadedFileManager();
private:
    void ThreadMain();                                          // 0x0098F2A0 ?ThreadMain@CThreadedFileManager@@AAEXXZ
    static void __cdecl BeginThread(void* manager);
};

void __cdecl CThreadedFileManager::BeginThread(void* manager)
{
    SetThreadPriority(GetCurrentThread(), THREAD_PRIORITY_ABOVE_NORMAL);
    ((CThreadedFileManager*)manager)->ThreadMain();
}