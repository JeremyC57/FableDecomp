// PKCSemaphore::Wait() -- wait without a timeout for one unit of the semaphore (WaitForSingleObject(handle, INFINITE)) and report "Waiting for semaphore unit failed" (file .\PKCSemaphore.cpp line 20) when the wait fails (WAIT_FAILED)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C2A090, length 192 bytes, __fastcall, plain `ret`.
// Callees: KERNEL32 WaitForSingleObject (IAT 0x0143FEAC), the libpk message stream's `__fastcall operator<<` (text @ 0x00C0DEC0, number @ 0x00C0DF00)
//   and destructor @ 0x00C0BA40, the report hook pointer @ 0x0143BB2C, CDebugBreak::CDebugBreak @ 0x00C16750.
// Identity: libpk (the d:\dev\audio\ver_fable_1.5_b\libpk audio library), file `.\PKCSemaphore.cpp`; the message is
//   "Waiting for semaphore unit failed" at line 20. No PDB covers libpk: the class and member names are descriptive (the catalog's PKCSemaphore_* labels
//   name the class from the same file string).
// Shape: built under /EHsc (EH state 0 while the message stream lives). The error report is the library's macro (as in the
//   landed 0x00C37610 error handler): the stream over the shared nil buffer, `file << "(" << line << ") : " << text << "\n"`,
//   the hook call, the stream destroyed, then an empty CDebugBreak temporary in the high byte of the frame word (`lea ecx,[esp+3]`).
// Leaf: ?Wait@PKCSemaphore
// Flags: /EHsc
struct CDebugStream {                        // 4 bytes: the message text (the library CString over the shared nil buffer)
    char* m_pBuffer;
    CDebugStream(char* buffer) : m_pBuffer(buffer) { }
    CDebugStream& __fastcall operator<<(const char* text);   // 0x00C0DEC0
    CDebugStream& __fastcall operator<<(long value);         // 0x00C0DF00
    ~CDebugStream();                        // 0x00C0BA40
};

struct CDebugBreak {                        // empty class whose constructor breaks into the debugger
    CDebugBreak();                          // 0x00C16750
};

extern char* g_pDebugBuffer;                // 0x0139C900 (the nil string's buffer)
extern void (__fastcall* g_pfnErrorReport)(const char* text);   // 0x0143BB2C

// libpk's error report: "<file>(<line>) : <text>\n" to the installed report hook, then a debugger break.
#define PK_REPORT_ERROR(line, text)                                                         \
    {                                                                                       \
        {                                                                                   \
            CDebugStream s(g_pDebugBuffer);                                                 \
            s << ".\\PKCSemaphore.cpp" << "(" << (long)(line) << ") : " << text << "\n";  \
            if (g_pfnErrorReport)                                                           \
                g_pfnErrorReport(s.m_pBuffer);                                              \
        }                                                                                   \
        CDebugBreak();                                                                      \
    }

extern "C" __declspec(dllimport) void* __stdcall CreateSemaphoreA(void* attributes, long initialCount, long maximumCount,
                                                                const char* name);          // IAT 0x0143FEA8
extern "C" __declspec(dllimport) unsigned long __stdcall WaitForSingleObject(void* handle, unsigned long milliseconds);   // IAT 0x0143FEAC
extern "C" __declspec(dllimport) int __stdcall ReleaseSemaphore(void* handle, long releaseCount, long* previousCount);   // IAT 0x0143FEB0

class PKCSemaphore {
public:
    PKCSemaphore(long initialCount, long maximumCount);
    void Wait();
    void Release();
private:
    void* m_hSemaphore;                     // +0x00 the Win32 semaphore
};

void PKCSemaphore::Wait()
{
    if (WaitForSingleObject(m_hSemaphore, 0xffffffff) == 0xffffffff)
        PK_REPORT_ERROR(20, "Waiting for semaphore unit failed");
}