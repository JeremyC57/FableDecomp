// CFileInstaller::SetDirtyDiscError(bool error) -- record a dirty-disc read error and make the installer thread exit
// Leaf: ?SetDirtyDiscError@CFileInstaller@@KIX_N@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009C3530, 72 bytes, `ret` (protected static __fastcall, error in cl).
// Identity: FableWin.pdb ?SetDirtyDiscError@CFileInstaller@@KIX_N@Z; it is inlined into OnFileReadCompletion 0x009C37A0
//   and OnFileWriteCompletion 0x009C3820 (`SetDirtyDiscError(true)`, the same lock / store / Get pair).
// Callees: CTBaseSingleton<CFileInstaller>::Get 0x00404440 (out of line), CCriticalSectionLock ctor 0x00A62920 /
//   dtor 0x00A629A0.  Global: the dirty-disc flag byte 0x013CA814 (landed Pause 0x009C31E0 calls it g_installerNoPauseWait).
// Layout: retail CFileInstaller (Ego_r.pdb minus the 16->12 CArray and 12->4 list shrink): CriticalSection +0x04, ExitThread +0x7A.
// Shape: `if (Get().ExitThread || error) error = true; Get().ExitThread = error;` -- reusing the bool argument keeps it in
//   bl (`mov bl,1` on both true arms); `ExitThread = ExitThread || error` materialises 0/1 in ebx instead.
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
protected:
    static void __fastcall SetDirtyDiscError(bool error);
};

void __fastcall CFileInstaller::SetDirtyDiscError(bool error)
{
    CCriticalSectionLock lock(CTBaseSingleton<CFileInstaller>::Get().CriticalSection, false);
    DirtyDiscError = error;
    if (CTBaseSingleton<CFileInstaller>::Get().ExitThread || error)
        error = true;
    CTBaseSingleton<CFileInstaller>::Get().ExitThread = error;
}