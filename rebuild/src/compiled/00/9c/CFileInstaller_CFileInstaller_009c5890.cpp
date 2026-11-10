// CFileInstaller::CFileInstaller -- initialise installer state, events and worker threads.
// Retail: 0x009C5890, 389 bytes; thiscall.
// Grade: VERIFIED_MATCH; see ROW.json and PROBES.md for verification or remaining residue.
// Leaf: ??0CFileInstaller@@QAE@XZ
// Flags: none
// Retail-volatile: see shared CFileInstaller.h and both original PDB qualifiers.
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CWIDESTRING
#define FABLE_RETAIL_CCOMPRESSORZLIB
#define FABLE_RETAIL_CACOMPRESSOR
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_STLP_LIST
#include "engine/CFileInstaller.h"
#include <process.h>
#ifndef _MT
extern "C" uintptr_t FABLE_CDECL _beginthread(void (FABLE_CDECL *)(void*),unsigned int,void*);
#endif
struct _SECURITY_ATTRIBUTES;
#ifdef _MSC_VER
extern "C" __declspec(dllimport) void FABLE_STDCALL InitializeCriticalSection(_RTL_CRITICAL_SECTION*);
extern "C" __declspec(dllimport) void* FABLE_STDCALL CreateEventW(_SECURITY_ATTRIBUTES*,int,int,const unsigned short*);
#else
extern "C" void InitializeCriticalSection(_RTL_CRITICAL_SECTION*);
extern "C" void* CreateEventW(_SECURITY_ATTRIBUTES*,int,int,const unsigned short*);
#endif
CFileInstaller::CFileInstaller()
{
    InitializeCriticalSection(&CriticalSection);
    DecompressionThreadFinished = false;
    ThreadFinished = false;
    ExitThread = false;
    Running = false;
    Completed = true;
    Paused = 0;
    Event = CreateEventW(0, 0, 0, 0);
    DecompressionEvent = CreateEventW(0, 0, 0, 0);
    PauseStartEvent = CreateEventW(0, 0, 0, 0);
    PauseFinishEvent = CreateEventW(0, 0, 0, 0);
    PCompletedRead = 0;
    ThreadFinished = false;
    PCurrentReadFile = 0;
    PCurrentWriteFile = 0;
    PReadBuffer = 0;
    PWriteBuffer = 0;
    BufferChunkSize = 0;
    NoBufferChunks = 0;
    AvailableBuffers = 0;
    AllocatedInputBuffers = 0;
    NoTotalBytesWritten = 0;
    TotalSizeOfAllFiles = 0;
    ExitThread = false;
    NoActiveReads = 0;
    NoActiveWrites = 0;
    WriteMode = false;
    IgnoreTimeStampWhenCopying = false;
    {
        CCharString extension(".biz");
        CompressedFileTypes[extension] = ".big";
    }
    _beginthread(BeginThread, 0x3000, this);
    _beginthread(BeginDecompressionThread, 0x3000, this);
}