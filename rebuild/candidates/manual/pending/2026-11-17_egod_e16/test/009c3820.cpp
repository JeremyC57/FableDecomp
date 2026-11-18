// Behaviour test for CFileInstaller::OnFileWriteCompletion 0x009C3820 (row source copied as plain C++ with stand-in callees).
// Verifier fix (e16 audit): the lock stub snapshots the protected state when each section is entered, SetEvent snapshots the
// byte count / delete count / dirty-disc state, and the stand-in delete records whether the signal already happened, so
// lock-scope / order mutants fail.
#include <cstdio>
#include <cstring>
#include <new>

struct _RTL_CRITICAL_SECTION { int Depth; int Entered; };

static void (*g_onLock)(_RTL_CRITICAL_SECTION* section);
class CCriticalSectionLock {
public:
    CCriticalSectionLock(_RTL_CRITICAL_SECTION& section, bool tryLock) : CriticalSection(&section), Locked(!tryLock)
    { if (g_onLock) g_onLock(&section); ++section.Depth; ++section.Entered; }
    ~CCriticalSectionLock() { if (Locked) --CriticalSection->Depth; }
    _RTL_CRITICAL_SECTION* CriticalSection;
    bool Locked;
};

struct _OVERLAPPED { unsigned long Internal, InternalHigh, Offset, OffsetHigh; void* hEvent; };

template <class T> class CTBaseSingleton {
public:
    static T& Get() { ++Calls; return TheInstance; }
    static T TheInstance;
    static int Calls;
};

static int g_deletes; static void* g_deleted; static int g_depthAtDelete; static int g_signalsAtDelete;
static _RTL_CRITICAL_SECTION* g_ownerSection;
static int g_setEventCalls;

class CFileInstaller {
public:
    class CActiveFile { public: void* SrcFileHandle; void* DstFileHandle; unsigned long CurrentSrcOffset, NoBytesRead, NoBytesWritten, InputSize; };
    class CActiveWrite {
    public:
        CActiveFile* PActiveFile; void* PBufferChunk;
        _OVERLAPPED OverlappedInfo;
        CFileInstaller* PFileInstaller;
        static void operator delete(void* p)
        { ++g_deletes; g_deleted = p; g_depthAtDelete = g_ownerSection->Depth; g_signalsAtDelete = g_setEventCalls; }
    };
    _RTL_CRITICAL_SECTION CriticalSection;
    long NoActiveWrites;
    bool ExitThread;
    void* Event;
    static bool DirtyDisc;
    static void SetDirtyDiscError(bool on);
    static void OnFileWriteCompletion(unsigned long err, unsigned long no_bytes, _OVERLAPPED* pover);
};
bool CFileInstaller::DirtyDisc = false;
template <class T> T CTBaseSingleton<T>::TheInstance;
template <class T> int CTBaseSingleton<T>::Calls = 0;

static void* g_setEventArg; static int g_depthAtSignal; static long g_writesAtSignal;
static unsigned long g_bytesAtSignal; static int g_deletesAtSignal; static bool g_dirtyAtSignal, g_exitAtSignal;
static CFileInstaller* g_signalled; static CFileInstaller::CActiveFile* g_file;
int SetEvent(void* event)
{
    ++g_setEventCalls; g_setEventArg = event;
    g_depthAtSignal = g_signalled->CriticalSection.Depth; g_writesAtSignal = g_signalled->NoActiveWrites;
    g_bytesAtSignal = g_file->NoBytesWritten; g_deletesAtSignal = g_deletes;
    g_dirtyAtSignal = CFileInstaller::DirtyDisc; g_exitAtSignal = CTBaseSingleton<CFileInstaller>::TheInstance.ExitThread;
    return 1;
}

// state seen when each lock is entered
static int g_ownerLocks, g_singleLocks;
static unsigned long g_bytesAtOwnerLock; static long g_writesAtOwnerLock; static int g_deletesAtOwnerLock;
static bool g_dirtyAtOwnerLock, g_exitAtOwnerLock; static int g_singleDepthAtOwnerLock;
static int g_ownerDepthAtSingleLock; static bool g_dirtyAtSingleLock, g_exitAtSingleLock;
static void onLock(_RTL_CRITICAL_SECTION* section)
{
    CFileInstaller& single = CTBaseSingleton<CFileInstaller>::TheInstance;
    if (section == &g_signalled->CriticalSection) {
        ++g_ownerLocks; g_bytesAtOwnerLock = g_file->NoBytesWritten; g_writesAtOwnerLock = g_signalled->NoActiveWrites;
        g_deletesAtOwnerLock = g_deletes; g_dirtyAtOwnerLock = CFileInstaller::DirtyDisc; g_exitAtOwnerLock = single.ExitThread;
        g_singleDepthAtOwnerLock = single.CriticalSection.Depth;
    } else if (section == &single.CriticalSection) {
        ++g_singleLocks; g_ownerDepthAtSingleLock = g_signalled->CriticalSection.Depth; g_dirtyAtSingleLock = CFileInstaller::DirtyDisc;
        g_exitAtSingleLock = single.ExitThread;
    }
}

void CFileInstaller::SetDirtyDiscError(bool on)
{
    CCriticalSectionLock cs(CTBaseSingleton<CFileInstaller>::Get().CriticalSection, false);
    DirtyDisc = on;
    if (CTBaseSingleton<CFileInstaller>::Get().ExitThread || on)
        on = true;
    CTBaseSingleton<CFileInstaller>::Get().ExitThread = on;
}

void CFileInstaller::OnFileWriteCompletion(unsigned long err, unsigned long no_bytes, _OVERLAPPED* pover)
{
    CActiveWrite* pwrite = (CActiveWrite*)pover->hEvent;
    if (err)
        SetDirtyDiscError(true);
    CCriticalSectionLock cs(pwrite->PFileInstaller->CriticalSection, false);
    pwrite->PActiveFile->NoBytesWritten += no_bytes;
    --pwrite->PFileInstaller->NoActiveWrites;
    SetEvent(pwrite->PFileInstaller->Event);
    delete pwrite;
}

static bool run(unsigned long err, bool exitBefore, unsigned long bytes)
{
    CFileInstaller& single = CTBaseSingleton<CFileInstaller>::TheInstance;
    std::memset(&single, 0, sizeof(single));
    single.ExitThread = exitBefore; single.NoActiveWrites = 7;
    CFileInstaller owner; std::memset(&owner, 0, sizeof(owner));
    owner.NoActiveWrites = 3;
    int eventToken = 0;
    owner.Event = &eventToken;
    CFileInstaller::CActiveFile file; std::memset(&file, 0, sizeof(file));
    file.NoBytesWritten = 100; file.NoBytesRead = 55; file.InputSize = 66;
    CFileInstaller::CActiveWrite write; std::memset(&write, 0, sizeof(write));
    write.PFileInstaller = &owner; write.PActiveFile = &file;
    _OVERLAPPED other; std::memset(&other, 0, sizeof(other));
    other.hEvent = &write;
    CFileInstaller::DirtyDisc = false;
    CTBaseSingleton<CFileInstaller>::Calls = 0;
    bool expectError = err != 0;
    g_setEventCalls = 0; g_setEventArg = 0; g_depthAtSignal = -1; g_writesAtSignal = -1; g_signalled = &owner; g_file = &file;
    g_bytesAtSignal = 0xFFFFFFFFUL; g_deletesAtSignal = -1; g_dirtyAtSignal = !expectError; g_exitAtSignal = !(exitBefore || expectError);
    g_deletes = 0; g_deleted = 0; g_depthAtDelete = -1; g_signalsAtDelete = -1; g_ownerSection = &owner.CriticalSection;
    g_ownerLocks = g_singleLocks = 0; g_bytesAtOwnerLock = 0; g_writesAtOwnerLock = -1; g_deletesAtOwnerLock = -1;
    g_dirtyAtOwnerLock = !expectError; g_exitAtOwnerLock = !(exitBefore || expectError); g_singleDepthAtOwnerLock = -1;
    g_ownerDepthAtSingleLock = -1; g_dirtyAtSingleLock = true; g_exitAtSingleLock = !exitBefore;
    g_onLock = onLock;
    CFileInstaller::OnFileWriteCompletion(err, bytes, &other);
    g_onLock = 0;
    bool ok = file.NoBytesWritten == 100 + bytes && file.NoBytesRead == 55 && file.InputSize == 66 &&
              owner.NoActiveWrites == 2 && single.NoActiveWrites == 7 &&
              g_setEventCalls == 1 && g_setEventArg == &eventToken && g_depthAtSignal == 1 && g_writesAtSignal == 2 &&
              g_deletes == 1 && g_deleted == &write && g_depthAtDelete == 1 &&
              owner.CriticalSection.Depth == 0 && owner.CriticalSection.Entered == 1 &&
              CFileInstaller::DirtyDisc == expectError && single.ExitThread == (exitBefore || expectError) &&
              single.CriticalSection.Entered == (expectError ? 1 : 0) && single.CriticalSection.Depth == 0 &&
              CTBaseSingleton<CFileInstaller>::Calls == (expectError ? 3 : 0) && owner.ExitThread == false &&
              // signal: bytes already added, record not yet freed, dirty-disc state already published
              g_bytesAtSignal == 100 + bytes && g_deletesAtSignal == 0 && g_signalsAtDelete == 1 &&
              g_dirtyAtSignal == expectError && g_exitAtSignal == (exitBefore || expectError) &&
              // owner lock: entered once before any protected update, after the dirty-disc lock was released
              g_ownerLocks == 1 && g_bytesAtOwnerLock == 100 && g_writesAtOwnerLock == 3 && g_deletesAtOwnerLock == 0 &&
              g_dirtyAtOwnerLock == expectError && g_exitAtOwnerLock == (exitBefore || expectError) && g_singleDepthAtOwnerLock == 0;
    if (expectError)   // SetDirtyDiscError: taken without the owner lock; both stores happen inside its own lock
        ok = ok && g_singleLocks == 1 && g_ownerDepthAtSingleLock == 0 && g_dirtyAtSingleLock == false &&
             g_exitAtSingleLock == exitBefore;
    else
        ok = ok && g_singleLocks == 0;
    return ok;
}

int main()
{
    bool ok = run(0, false, 0x200) && run(0, true, 1) && run(1, false, 0) && run(6, false, 0x10000) &&
              run(0x80000000UL, true, 3) && run(2, true, 0x1000);
    std::printf(ok ? "009c3820_TEST PASS\n" : "009c3820_TEST FAIL\n");
    return ok ? 0 : 1;
}
