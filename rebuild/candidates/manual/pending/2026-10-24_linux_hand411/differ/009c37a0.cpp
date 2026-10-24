// CFileInstaller::OnFileReadCompletion(unsigned long errorCode, unsigned long bytesTransferred, _OVERLAPPED* overlapped) -- read I/O completion routine: flag a dirty-disc error, then hand the finished read to the installer and signal it
// Leaf: ?OnFileReadCompletion@CFileInstaller@@KGXKKPAU_OVERLAPPED@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009C37A0, 128 bytes, `ret 0Ch` (protected static __stdcall).
// Grade: DIFFER(125v128) -- retail keeps a dead `mov cl,byte ptr [eax+7Ah]` load of ExitThread inside the inlined SetDirtyDiscError(true)
//   (the signature of an ExitThread flag carrying the cv-qualifier the lane bans; every unqualified form drops the load) and the
//   following lock/SetEvent block then rotates edx/eax/ecx where ours uses ecx/eax/edx.  Everything else matches.
// Identity: FableWin.pdb ?OnFileReadCompletion@CFileInstaller@@KGXKKPAU_OVERLAPPED@@@Z (a ReadFileEx completion routine);
//   catalogued as `CodeSection_SetReadyAndSignal`.
// The OVERLAPPED's hEvent (+0x10) carries the CActiveRead (Ego_r.pdb: PActiveFile +0, FilePos +4, TransferSize +8,
//   PBufferChunk +0xC, OverlappedInfo +0x10, PFileInstaller +0x24).
// Callees: CTBaseSingleton<CFileInstaller>::Get 0x00404440, CCriticalSectionLock ctor 0x00A62920 / dtor 0x00A629A0,
//   KERNEL32 SetEvent (IAT 0x0143FDE0).  The inline SetDirtyDiscError(true) is the out-of-line 0x009C3530 body
//   (its dead `mov cl,[eax+7Ah]` load of ExitThread survives the constant `|| true`).
// Layout: retail CFileInstaller CriticalSection +0x04, PCompletedRead +0x74, ExitThread +0x7A, Event +0x84.
extern "C" __declspec(dllimport) int __stdcall SetEvent(void* event);     // IAT 0x0143FDE0

struct _OVERLAPPED {
    unsigned long Internal;                     // +0x00
    unsigned long InternalHigh;                 // +0x04
    unsigned long Offset;                       // +0x08
    unsigned long OffsetHigh;                   // +0x0C
    void*         hEvent;                       // +0x10
};

struct _RTL_CRITICAL_SECTION { unsigned char _body[0x18]; };   // Win32 critical section, 24 bytes

class CCriticalSectionLock {                    // 8 bytes on the stack (Ego_r.pdb)
public:
    CCriticalSectionLock(_RTL_CRITICAL_SECTION& section, bool tryLock);   // 0x00A62920
    ~CCriticalSectionLock();                                              // 0x00A629A0
    void Release();                                                       // 0x00A62960
    _RTL_CRITICAL_SECTION* CriticalSection;     // +0x00
    bool Locked;                                // +0x04
};

template <class T> class CTBaseSingleton {
public:
    static T& __fastcall Get();                 // CFileInstaller: 0x00404440 (out of line)
};

class CFileInstaller {
public:
    class CActiveFile;
    void*                 _padding_;            // +0x00
    _RTL_CRITICAL_SECTION CriticalSection;      // +0x04
    unsigned char         _pad1c[0x14];         // +0x1C BufferChunks (CArray, 12), PReadBuffer, PWriteBuffer
    long                  AvailableBuffers;     // +0x30
    long                  AllocatedInputBuffers;// +0x34
    void*                 FilesToCopy;          // +0x38 STLport lists are 4 bytes
    void*                 FilesToFinish;        // +0x3C
    void*                 ActiveFiles;          // +0x40
    unsigned char         CompletedFiles[0xC];  // +0x44 STLport set (12 bytes)
    CActiveFile*          PCurrentReadFile;     // +0x50
    CActiveFile*          PCurrentWriteFile;    // +0x54
    long                  NoActiveReads;        // +0x58
    long                  NoActiveWrites;       // +0x5C
    bool                  WriteMode;            // +0x60
    unsigned long         BufferChunkSize;      // +0x64
    unsigned long         NoBufferChunks;       // +0x68
    long                  NoTotalBytesWritten;  // +0x6C
    long                  TotalSizeOfAllFiles;  // +0x70
    void*                 PCompletedRead;       // +0x74
    bool                  ThreadFinished;       // +0x78
    bool                  DecompressionThreadFinished; // +0x79
    bool                  ExitThread;           // +0x7A
    bool                  Running;              // +0x7B
    unsigned long         Paused;               // +0x7C
    bool                  Completed;            // +0x80
    bool                  IgnoreTimeStampWhenCopying;  // +0x81
    void*                 Event;                // +0x84
    void*                 DecompressionEvent;   // +0x88
    static bool           DirtyDiscError;       // 0x013CA814
    class CActiveRead {
    public:
        CActiveFile*    PActiveFile;            // +0x00
        long            FilePos;                // +0x04
        long            TransferSize;           // +0x08
        void*           PBufferChunk;           // +0x0C
        _OVERLAPPED     OverlappedInfo;         // +0x10
        CFileInstaller* PFileInstaller;         // +0x24
    };
protected:
    static void __fastcall SetDirtyDiscError(bool error)
    {
        CCriticalSectionLock lock(CTBaseSingleton<CFileInstaller>::Get().CriticalSection, false);
        DirtyDiscError = error;
        if (CTBaseSingleton<CFileInstaller>::Get().ExitThread || error)
            error = true;
        CTBaseSingleton<CFileInstaller>::Get().ExitThread = error;
    }
    static void __stdcall OnFileReadCompletion(unsigned long errorCode, unsigned long bytesTransferred, _OVERLAPPED* overlapped);
};

void __stdcall CFileInstaller::OnFileReadCompletion(unsigned long errorCode, unsigned long bytesTransferred, _OVERLAPPED* overlapped)
{
    CActiveRead* read = (CActiveRead*)overlapped->hEvent;
    if (errorCode != 0)
        SetDirtyDiscError(true);
    CCriticalSectionLock lock(read->PFileInstaller->CriticalSection, false);
    read->PFileInstaller->PCompletedRead = read;
    SetEvent(read->PFileInstaller->Event);
}
