// PKCDefaultFileManager::ThreadProc(void* manager) -- the Win32 thread procedure of the default file manager: run its asynchronous read loop, then exit with 0
// Leaf: ?ThreadProc@PKCDefaultFileManager@@SGKPAX@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C1AC30, 14 bytes: mov ecx,[esp+4] ; call 0x00C1A9C0 ; xor eax,eax ; ret 4
// Identity: the landed PKCDefaultFileManager constructor 0x00C1AC40 / destructor 0x00C1A230 declare it as
//   `static unsigned long __stdcall ThreadProc(void* manager); // 0x00C1AC30` and pass it to CreateThread (its only
//   reference, the `push offset` at 0x00C1ACC8); the class name comes from the ".\PKCDefaultFileManager.cpp" warning
//   strings. The catalog's "FileManager_AsyncReadThreadWrapper" is descriptive; the stale reloc binding
//   ?FileManagerThread_00c1ac30@@YGKPAX@Z predates the constructor's rename.
// Callees: the worker loop @ 0x00C1A9C0 (EH-framed; the manifest's "FileManager_AsyncReadThread", not landed),
//   thiscall on the manager, no arguments.
class PKCDefaultFileManager {
public:
    static unsigned long __stdcall ThreadProc(void* manager);
    void AsyncReadThread();                     // 0x00C1A9C0
};

unsigned long __stdcall PKCDefaultFileManager::ThreadProc(void* manager)
{
    ((PKCDefaultFileManager*)manager)->AsyncReadThread();
    return 0;
}