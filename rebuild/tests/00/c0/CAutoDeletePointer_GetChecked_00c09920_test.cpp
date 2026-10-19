#include <stdio.h>
#include <string.h>
#ifndef _MSC_VER
#define __fastcall
#endif
// CAutoDeletePointer::GetChecked() -- diagnose an empty pointer wrapper, then return its stored pointer
// Retail 0x00C09920, 186 bytes. Caller 0x00C08F0D invokes this on a stack wrapper;
// it consumes this return as a checked auto-delete pointer. The propagated CWorldManager::OnShutdown catalog
// label is not supported by the PKCAutoDelete.h assertion string or call shape. /EHsc emits stream cleanup.
// Leaf: GetChecked
struct CDebugStream {
    char* m_pBuffer;
    CDebugStream(char* buffer) : m_pBuffer(buffer) { }
    CDebugStream& __fastcall operator<<(const char* text); // 0x00C0DEC0
    CDebugStream& __fastcall operator<<(long value); // 0x00C0DF00
    ~CDebugStream(); // 0x00C0BA40
};
struct CDebugBreak { CDebugBreak(); }; // 0x00C16750
extern char* g_pDebugBuffer; // 0x0139C900
extern void (__fastcall* g_pfnErrorReport)(const char*); // 0x0143BB2C
struct CAutoDeletePointer {
    void* m_pObject;
    void* GetChecked();
};
void* CAutoDeletePointer::GetChecked() {
    if (m_pObject == 0) {
        {
            CDebugStream s(g_pDebugBuffer);
            s << "d:\\dev\\audio\\ver_fable_1.5_b\\libpk\\PKCAutoDelete.h" << "(" << 32 << ") : " << "Should have checked first..." << "\n";
            if (g_pfnErrorReport) g_pfnErrorReport(s.m_pBuffer);
        }
        CDebugBreak();
    }
    return m_pObject;
}

static char captured[256], events[32]; static int eventCount;
char* g_pDebugBuffer = captured;
static void mark(char c) { events[eventCount++] = c; events[eventCount] = 0; }
void (__fastcall* g_pfnErrorReport)(const char*) = 0;
CDebugStream& __fastcall CDebugStream::operator<<(const char* text) { strcat(m_pBuffer, text); mark('T'); return *this; }
CDebugStream& __fastcall CDebugStream::operator<<(long value) { char n[32]; sprintf(n, "%ld", value); strcat(m_pBuffer, n); mark('N'); return *this; }
CDebugStream::~CDebugStream() { mark('D'); }
CDebugBreak::CDebugBreak() { mark('B'); }
static void __fastcall report(const char* text) {
    if (strcmp(text, "d:\\dev\\audio\\ver_fable_1.5_b\\libpk\\PKCAutoDelete.h(32) : Should have checked first...\n") != 0) mark('!');
    mark('R');
}
int main() {
    CAutoDeletePointer p; p.m_pObject = 0;
    captured[0] = 0; events[0] = 0; eventCount = 0; g_pfnErrorReport = report;
    if (p.GetChecked() != 0 || strcmp(events, "TTNTTTRDB") != 0) return 1;
    captured[0] = 0; events[0] = 0; eventCount = 0; g_pfnErrorReport = 0;
    if (p.GetChecked() != 0 || strcmp(events, "TTNTTTDB") != 0) return 2;
    p.m_pObject = (void*)0x1234; captured[0] = 0; events[0] = 0; eventCount = 0; g_pfnErrorReport = report;
    if (p.GetChecked() != (void*)0x1234 || eventCount != 0) return 3;
    puts("00c09920_TEST PASS"); return 0;
}