// Behaviour test for CFileInstaller::OnFileReadCompletion 0x009C37A0 (row source copied as plain C++ with stand-in callees).
// Verifier fix (e16 audit): the lock stub snapshots the protected state when each section is entered and SetEvent snapshots the
// dirty-disc state, so lock-scope / order mutants fail; one run has err != 0 with no_bytes == 0.
#include <cstdio>
#include <cstring>

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

class CFileInstaller {
public:
    class CActiveRead {
    public:
        void* PActiveFile; long FilePos; long TransferSize; void* PBufferChunk;
        _OVERLAPPED OverlappedInfo;
        CFileInstaller* PFileInstaller;
    };
    _RTL_CRITICAL_SECTION CriticalSection;
    CActiveRead* PCompletedRead;
    bool ExitThread;
    void* Event;
    static bool DirtyDisc;
    static void SetDirtyDiscError(bool on);
    static void OnFileReadCompletion(unsigned long err, unsigned long no_bytes, _OVERLAPPED* pover);
};
bool CFileInstaller::DirtyDisc = false;
template <class T> T CTBaseSingleton<T>::TheInstance;
template <class T> int CTBaseSingleton<T>::Calls = 0;

static int g_setEventCalls; static void* g_setEventArg; static CFileInstaller::CActiveRead* g_completedAtSignal; static int g_depthAtSignal;
static bool g_dirtyAtSignal, g_exitAtSignal;
static CFileInstaller* g_signalled;
int SetEvent(void* event)
{
    ++g_setEventCalls; g_setEventArg = event;
    g_completedAtSignal = g_signalled->PCompletedRead; g_depthAtSignal = g_signalled->CriticalSection.Depth;
    g_dirtyAtSignal = CFileInstaller::DirtyDisc; g_exitAtSignal = CTBaseSingleton<CFileInstaller>::TheInstance.ExitThread;
    return 1;
}

// state seen when each lock is entered
static int g_ownerLocks, g_singleLocks;
static CFileInstaller::CActiveRead* g_completedAtOwnerLock; static bool g_dirtyAtOwnerLock, g_exitAtOwnerLock; static int g_singleDepthAtOwnerLock;
static int g_ownerDepthAtSingleLock; static bool g_dirtyAtSingleLock, g_exitAtSingleLock;
static void onLock(_RTL_CRITICAL_SECTION* section)
{
    CFileInstaller& single = CTBaseSingleton<CFileInstaller>::TheInstance;
    if (section == &g_signalled->CriticalSection) {
        ++g_ownerLocks; g_completedAtOwnerLock = g_signalled->PCompletedRead; g_dirtyAtOwnerLock = CFileInstaller::DirtyDisc;
        g_exitAtOwnerLock = single.ExitThread; g_singleDepthAtOwnerLock = single.CriticalSection.Depth;
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

void CFileInstaller::OnFileReadCompletion(unsigned long err, unsigned long no_bytes, _OVERLAPPED* pover)
{
    CActiveRead* pread = (CActiveRead*)pover->hEvent;
    if (err)
        SetDirtyDiscError(true);
    CCriticalSectionLock cs(pread->PFileInstaller->CriticalSection, false);
    pread->PFileInstaller->PCompletedRead = pread;
    SetEvent(pread->PFileInstaller->Event);
}

static bool run(unsigned long err, bool exitBefore, unsigned long no_bytes)
{
    CFileInstaller& single = CTBaseSingleton<CFileInstaller>::TheInstance;
    std::memset(&single, 0, sizeof(single));
    single.ExitThread = exitBefore;
    CFileInstaller owner; std::memset(&owner, 0, sizeof(owner));
    int eventToken = 0;
    owner.Event = &eventToken;
    CFileInstaller::CActiveRead read; std::memset(&read, 0, sizeof(read));
    read.PFileInstaller = &owner;
    _OVERLAPPED other; std::memset(&other, 0, sizeof(other));
    other.hEvent = &read;                      // the routine reads hEvent of the OVERLAPPED it was given
    CFileInstaller::DirtyDisc = false;
    CTBaseSingleton<CFileInstaller>::Calls = 0;
    g_setEventCalls = 0; g_setEventArg = 0; g_completedAtSignal = 0; g_depthAtSignal = -1; g_signalled = &owner;
    g_dirtyAtSignal = !(err != 0); g_exitAtSignal = !(exitBefore || err != 0);
    g_ownerLocks = g_singleLocks = 0; g_completedAtOwnerLock = &read; g_dirtyAtOwnerLock = !(err != 0);
    g_exitAtOwnerLock = !(exitBefore || err != 0); g_singleDepthAtOwnerLock = -1;
    g_ownerDepthAtSingleLock = -1; g_dirtyAtSingleLock = true; g_exitAtSingleLock = !exitBefore;
    g_onLock = onLock;
    CFileInstaller::OnFileReadCompletion(err, no_bytes, &other);
    g_onLock = 0;
    bool expectError = err != 0;
    bool ok = owner.PCompletedRead == &read && g_setEventCalls == 1 && g_setEventArg == &eventToken &&
              g_completedAtSignal == &read && g_depthAtSignal == 1 && owner.CriticalSection.Depth == 0 &&
              owner.CriticalSection.Entered == 1 && single.PCompletedRead == 0 &&
              CFileInstaller::DirtyDisc == expectError &&
              single.ExitThread == (exitBefore || expectError) &&
              single.CriticalSection.Entered == (expectError ? 1 : 0) && single.CriticalSection.Depth == 0 &&
              CTBaseSingleton<CFileInstaller>::Calls == (expectError ? 3 : 0) &&
              owner.ExitThread == false &&
              // the dirty-disc state is published before the owner lock and before the signal
              g_dirtyAtSignal == expectError && g_exitAtSignal == (exitBefore || expectError) &&
              // owner lock: entered once, nothing stored yet, dirty-disc work already done and its lock released
              g_ownerLocks == 1 && g_completedAtOwnerLock == 0 && g_dirtyAtOwnerLock == expectError &&
              g_exitAtOwnerLock == (exitBefore || expectError) && g_singleDepthAtOwnerLock == 0;
    if (expectError)   // SetDirtyDiscError: taken without the owner lock; both stores happen inside its own lock
        ok = ok && g_singleLocks == 1 && g_ownerDepthAtSingleLock == 0 && g_dirtyAtSingleLock == false &&
             g_exitAtSingleLock == exitBefore;
    else
        ok = ok && g_singleLocks == 0;
    return ok;
}

int main()
{
    bool ok = run(0, false, 0x1234) && run(0, true, 0x1234) && run(1, false, 0x1234) && run(5, false, 0x1234) &&
              run(0x80000000UL, true, 0x1234) && run(2, true, 0x1234) &&
              run(1, false, 0) && run(3, true, 0) && run(0, false, 0);   // err != 0 with no bytes transferred still flags the disc
    std::printf(ok ? "009c37a0_TEST PASS\n" : "009c37a0_TEST FAIL\n");
    return ok ? 0 : 1;
}