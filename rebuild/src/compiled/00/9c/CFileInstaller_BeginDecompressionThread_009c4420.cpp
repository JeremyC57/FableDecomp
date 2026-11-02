// CFileInstaller::BeginDecompressionThread(void* installer) -- _beginthread entry of the decompression thread: run at normal priority, then the installer's DecompressionThreadMain
// Leaf: ?BeginDecompressionThread@CFileInstaller@@KAXPAX@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x009C4420, 24 bytes (int3 padding to 0x009C4440), __cdecl static (the argument at [esp+4]),
//   tail jmp DecompressionThreadMain with ecx = the argument.
// Identity (proven, two sources): (1) its address is pushed as the start routine of _beginthread at 0x009C5A00 inside
//   CFileInstaller (the decompression thread; the sibling start routine 0x009C5870 = CFileInstaller::BeginThread does the same
//   with priority 1 and jumps to ThreadMain 0x009C5690), and it forwards to 0x009C4230 = CFileInstaller::DecompressionThreadMain;
//   (2) Ego_r.pdb ?BeginDecompressionThread@CFileInstaller@@KAXPAX@Z (protected static, void(void*), the _beginthread signature),
//   listed beside DecompressionThreadMain. The catalog label "CFileInstaller_ThreadEntry" is a stand-in.
// Callees: kernel32 GetCurrentThread (IAT 0x0143FDCC), kernel32 SetThreadPriority (IAT 0x0143FDD0), CFileInstaller::
//   DecompressionThreadMain() 0x009C4230 (tail).
// Layout: no shared engine/CFileInstaller.h; the class is restated with every Ego_r.pdb member at its retail offset (retail
//   sizeof 0xA0 = Ego_r 0xC4 less the Dinkumware container excess), containers as opaque storage; sizeof asserted.
// Shape: THREAD_PRIORITY_NORMAL (0) from winbase.h, the void* argument cast to the installer.
#include "rebuild_abi.h"
extern "C" __declspec(dllimport) void* __stdcall GetCurrentThread();                       // IAT 0x0143FDCC
extern "C" __declspec(dllimport) int __stdcall SetThreadPriority(void* thread, int priority);   // IAT 0x0143FDD0
#define THREAD_PRIORITY_NORMAL 0

struct _RTL_CRITICAL_SECTION { unsigned char _body[0x18]; };   // Win32 critical section, 24 bytes

template <class T> class CTBaseSingleton {                      // RTTI base of CFileInstaller (+0), vptr only
public:
    virtual ~CTBaseSingleton();
};

class CFileInstaller : public CTBaseSingleton<CFileInstaller> { // retail sizeof 0xA0 (Ego_r.pdb 0xC4 less the Dinkumware container excess)
public:
    class CBufferChunk;
    class CFileInfo;
    class CActiveFile;
    class CActiveRead;
    virtual ~CFileInstaller();
protected:
    void DecompressionThreadMain();                             // 0x009C4230
    static void __cdecl BeginDecompressionThread(void* installer);
private:
    _RTL_CRITICAL_SECTION CriticalSection;                      // +0x04
    unsigned char         BufferChunks_[0xC];                   // +0x1C CArray<CBufferChunk> BufferChunks
    unsigned char*        PReadBuffer;                          // +0x28
    unsigned char*        PWriteBuffer;                         // +0x2C
    long                  AvailableBuffers;                     // +0x30
    long                  AllocatedInputBuffers;                // +0x34
    unsigned char         FilesToCopy_[0x4];                    // +0x38 std::list<CCountedPointer<const CFileInfo> > FilesToCopy
    unsigned char         FilesToFinish_[0x4];                  // +0x3C std::list<CCountedPointer<const CFileInfo> > FilesToFinish
    unsigned char         ActiveFiles_[0x4];                    // +0x40 std::list<CActiveFile> ActiveFiles
    unsigned char         CompletedFiles_[0xC];                 // +0x44 std::set<CCharString> CompletedFiles
    CActiveFile*          PCurrentReadFile;                     // +0x50
    CActiveFile*          PCurrentWriteFile;                    // +0x54
    long                  NoActiveReads;                        // +0x58
    long                  NoActiveWrites;                       // +0x5C
    bool                  WriteMode;                            // +0x60
    unsigned long         BufferChunkSize;                      // +0x64
    unsigned long         NoBufferChunks;                       // +0x68
    long                  NoTotalBytesWritten;                  // +0x6C
    long                  TotalSizeOfAllFiles;                  // +0x70
    CActiveRead*          PCompletedRead;                       // +0x74
    bool                  ThreadFinished;                       // +0x78
    bool                  DecompressionThreadFinished;          // +0x79
    bool                  ExitThread;                           // +0x7A
    bool                  Running;                              // +0x7B
    unsigned long         Paused;                               // +0x7C
    bool                  Completed;                            // +0x80
    bool                  IgnoreTimeStampWhenCopying;           // +0x81
    void*                 Event;                                // +0x84
    void*                 DecompressionEvent;                   // +0x88
    void*                 PauseStartEvent;                      // +0x8C
    void*                 PauseFinishEvent;                     // +0x90
    unsigned char         CompressedFileTypes_[0xC];            // +0x94 std::map<CCharString, CCharString> CompressedFileTypes
};
FABLE_STATIC_ASSERT(sizeof(CFileInstaller) == 0xA0);

void __cdecl CFileInstaller::BeginDecompressionThread(void* installer)
{
    SetThreadPriority(GetCurrentThread(), THREAD_PRIORITY_NORMAL);
    ((CFileInstaller*)installer)->DecompressionThreadMain();
}