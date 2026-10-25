// _global::GFLogMemoryUse() -- when memory logging is on, at most every 300 seconds: note the time, measure the physical memory in use (MB) and ask the global allocator to dump a numbered Memlog%4d.log
// Leaf: ?GFLogMemoryUse@@
// Flags: /GS /Gr
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00415E85, 311 bytes, plain `ret`; a /Os TU (unaligned entry, `push 6 ; pop edx`)
// with /GS (cookie 0x0139C8A8 around the 256-byte file-name buffer, check 0x00BFE9F9).
// Callees: GFGetTime 0x009E1BC0 (double on the FPU), GlobalMemoryStatus through the IAT (0x0143FE20), CCharString::FormatDouble(long,
// double) 0x009A08B0 (x2), operator+(const char*, const CCharString&) 0x0099F690, operator+(const CCharString&, const char*)
// 0x0099F600, operator+(const CCharString&, const CCharString&) 0x0099F570, ~CCharString 0x0099EAE0 (x4, plus the result),
// sprintf 0x00BFEB10, the global allocator's (0x013CAA90) dump hook 0x009F2080 (an empty `ret 4` body in retail: labelled
// Nop_ret4; descriptive name DumpLog).
// Data: the log-memory switch 0x013B85F1, the Memlog file index 0x013B86A8, the last log time (double) 0x01375600; constants 0.0
// 0x0122ED70, 300.0 0x0122ED68, 1/1048576.0f 0x0122ED60, 2^32 0x0122DCB4 (the unsigned-to-float fix-up).
// Shape: `last < 0.0 || now - last > 300.0` on the FPU (fcom keeps `last` for the subtraction); the used-memory figure is a float
// expression (dword constant) passed as a double; the text line is built right to left and never used (its consumer is
// compiled out in retail).  Frame: the biased /Os frame `lea ebp,[esp-0C0h]` comes from `optimize("s", on)` alone -- adding
// `optimize("y", off)` gives a plain `mov ebp,esp` frame (GOTCHAS chunk 247/471).
#pragma optimize("s", on)
#include <stddef.h>
struct _MEMORYSTATUS {
    unsigned long dwLength;
    unsigned long dwMemoryLoad;
    unsigned long dwTotalPhys;
    unsigned long dwAvailPhys;
    unsigned long dwTotalPageFile;
    unsigned long dwAvailPageFile;
    unsigned long dwTotalVirtual;
    unsigned long dwAvailVirtual;
};
extern "C" __declspec(dllimport) void __stdcall GlobalMemoryStatus(_MEMORYSTATUS* status);   // [0x0143FE20]
extern "C" int __cdecl sprintf(char* buffer, const char* format, ...);                      // 0x00BFEB10

class CCharString {
public:
    void* PStringData;
    ~CCharString();                                                     // 0x0099EAE0
    static CCharString FormatDouble(long digits, double value);         // 0x009A08B0
};
CCharString operator+(const char* lhs, const CCharString& rhs);          // 0x0099F690
CCharString operator+(const CCharString& lhs, const char* rhs);          // 0x0099F600
CCharString operator+(const CCharString& lhs, const CCharString& rhs);   // 0x0099F570

class CMemoryAllocator {
public:
    void DumpLog(const char* fileName);                                 // 0x009F2080 (empty in retail)
};
extern CMemoryAllocator* g_pGlobalAllocator;                            // 0x013CAA90

double GFGetTime();                                                     // 0x009E1BC0
extern bool   g_bLogMemoryUse;                                          // 0x013B85F1
extern long   g_MemLogIndex;                                            // 0x013B86A8
extern double g_LastMemLogTime;                                         // 0x01375600

void GFLogMemoryUse()
{
    if (!g_bLogMemoryUse)
        return;
    double now = GFGetTime();
    if (g_LastMemLogTime < 0.0 || now - g_LastMemLogTime > 300.0) {
        g_LastMemLogTime = now;
        _MEMORYSTATUS status;
        GlobalMemoryStatus(&status);
        float usedMB = (float)(status.dwTotalPhys - status.dwAvailPhys) / (1024.0f * 1024.0f);
        CCharString text = "Mem use log at " + CCharString::FormatDouble(6, now) + " is " + CCharString::FormatDouble(6, usedMB);
        char fileName[256];
        sprintf(fileName, "Memlog%4d.log", g_MemLogIndex);
        g_pGlobalAllocator->DumpLog(fileName);
        ++g_MemLogIndex;
    }
}