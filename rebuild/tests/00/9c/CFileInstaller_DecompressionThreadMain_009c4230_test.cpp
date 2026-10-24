#include <cstdio>
#include <cstring>

struct _RTL_CRITICAL_SECTION { int Depth; int Entered; };   // stand-in: counts entries

class CCriticalSectionLock {
public:
    CCriticalSectionLock(_RTL_CRITICAL_SECTION& section, bool tryLock) : CriticalSection(&section), Locked(true)
    { (void)tryLock; ++section.Depth; ++section.Entered; }
    ~CCriticalSectionLock() { Release(); }
    void Release() { if (Locked) { --CriticalSection->Depth; Locked = false; } }
    _RTL_CRITICAL_SECTION* CriticalSection;
    bool Locked;
};

class CFileInstaller;
static CFileInstaller* g_installer = 0;
static int g_waits = 0;
static unsigned long g_lastTimeout = 0;
static int g_lastAlertable = 0;
static void* g_lastHandle = 0;
static void SimulateWait(CFileInstaller* fi);
static unsigned long WaitForSingleObjectEx(void* handle, unsigned long milliseconds, int alertable)   // stand-in
{
    ++g_waits; g_lastHandle = handle; g_lastTimeout = milliseconds; g_lastAlertable = alertable;
    SimulateWait(g_installer);
    return 0;
}

class CCompressorZlib {
public:
    bool HasStreamInput() const { return StreamInput != 0; }
    int StreamInput;
};

namespace _STL {
struct _List_node_base {
    _List_node_base* _M_next;
    _List_node_base* _M_prev;
};
template <class _Tp> class list {
public:
    bool empty() const { return _M_node->_M_next == _M_node; }
    _List_node_base* _M_node;
};
}

class CFileInstaller {
public:
    class CActiveRead;
    class CActiveFile {
    public:
        CCompressorZlib           Decompressor;
        _STL::list<void*>         PendingWrites;
        _STL::list<CActiveRead*>  PendingDecompression;
    };
    void ProcessActiveFileForDecompression(CActiveFile& file)   // stand-in: consume one unit of input, free a buffer
    {
        ++Processed;
        if (&file != PCurrentWriteFile) BadFile = true;
        if (CriticalSection.Depth != 0) BadFile = true;           // the lock is released before acting
        if (file.Decompressor.StreamInput > 0) --file.Decompressor.StreamInput;
        else if (!file.PendingDecompression.empty()) file.PendingDecompression._M_node->_M_next = file.PendingDecompression._M_node;
        --AvailableBuffers;
    }
    void DecompressionThreadMain()
    {
        while (!ExitThread) {
            CCriticalSectionLock lock(CriticalSection, false);
            bool process = false;
            CActiveFile* file = PCurrentWriteFile;
            if (file && (file->Decompressor.HasStreamInput() || !file->PendingDecompression.empty()) && AvailableBuffers > 0)
                process = true;
            lock.Release();
            if (process)
                ProcessActiveFileForDecompression(*PCurrentWriteFile);
            else
                WaitForSingleObjectEx(DecompressionEvent, 100, 1);
        }
        DecompressionThreadFinished = true;
    }
    _RTL_CRITICAL_SECTION CriticalSection;
    long                  AvailableBuffers;
    CActiveFile*          PCurrentWriteFile;
    bool                  DecompressionThreadFinished;
    bool                  ExitThread;
    void*                 DecompressionEvent;
    int                   Processed;
    bool                  BadFile;
};

static CFileInstaller::CActiveFile g_file;
static _STL::_List_node_base g_pendingSentinel, g_pendingNode;
static void SimulateWait(CFileInstaller* fi)
{
    if (g_waits == 1) { fi->PCurrentWriteFile = &g_file; }              // a write file appears (2 stream units)
    else if (g_waits == 2) { fi->AvailableBuffers = 5; }                // buffers are freed
    else if (g_waits == 3) {                                            // one queued read to decompress
        g_pendingSentinel._M_next = &g_pendingNode; g_pendingNode._M_next = &g_pendingSentinel;
    }
    else fi->ExitThread = true;
}

int main()
{
    CFileInstaller fi;
    std::memset(&fi.CriticalSection, 0, sizeof(fi.CriticalSection));
    g_installer = &fi;
    g_file.Decompressor.StreamInput = 2;
    g_pendingSentinel._M_next = g_pendingSentinel._M_prev = &g_pendingSentinel;
    g_file.PendingDecompression._M_node = &g_pendingSentinel;
    g_file.PendingWrites._M_node = 0;
    fi.AvailableBuffers = 1;
    fi.PCurrentWriteFile = 0;
    fi.DecompressionThreadFinished = false;
    fi.ExitThread = false;
    fi.DecompressionEvent = (void*)&g_file;
    fi.Processed = 0;
    fi.BadFile = false;
    fi.DecompressionThreadMain();
    // wait (no file) -> file appears; 1 unit with 1 buffer; wait (no buffer) -> buffers; 1 unit; wait (no input) ->
    // a queued read; 1 unit; wait -> exit
    if (!fi.DecompressionThreadFinished || fi.Processed != 3 || g_waits != 4 || fi.BadFile || g_lastTimeout != 100 ||
        g_lastAlertable != 1 || g_lastHandle != (void*)&g_file || fi.CriticalSection.Depth != 0 || fi.CriticalSection.Entered != 7) {
        std::printf("009c4230_TEST FAIL\n"); return 1;
    }
    CFileInstaller idle;                                                // already told to exit: no lock, just the flag
    std::memset(&idle, 0, sizeof(idle));
    idle.ExitThread = true;
    idle.DecompressionThreadMain();
    if (!idle.DecompressionThreadFinished || idle.CriticalSection.Entered != 0) { std::printf("009c4230_TEST FAIL\n"); return 1; }
    std::printf("009c4230_TEST PASS\n");
    return 0;
}