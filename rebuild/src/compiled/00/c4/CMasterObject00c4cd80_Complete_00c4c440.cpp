// CMasterObject_00c4cd80::Complete() -- EH-framed under the master object's profiling lock: warn (PKRACMasterObject.cpp lines 29-32) when nothing was sent, objects are still out, the callback is null or the dependency list is still open, then call the completion callback with success = no failures and the stored context
// Leaf: ?Complete@CMasterObject_00c4cd80@@QAEXXZ
// Vendor naming V2 (plan phase 1, header-first): PKCProtectionInstance / PKCCriticalSection come from the shared libpk/PKCProtectionInstance.h (was a local copy).
// Flags: /EHsc
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C4C440, 625 bytes, thiscall, /EHsc.
// Identity: libpk PKRACMasterObject.cpp (trace lines 29..32); landed Finalize 0x00C4CD80 calls it as ?Complete@CMasterObject_00c4cd80@@QAEXXZ.
// Strings: "No objects have been sent.... deeeerrrr", "There are still objects out there...", "Null callback",
//   "Dependancy list still being created...".
// Shape: four trace blocks, then the callback slot 0 with (failures == 0, context).
#include "libpk/PKCProtectionInstance.h"
extern char* g_pchNil_0139c900;            // 0x0139C900
typedef void (__fastcall* PFNLOGSINK_00c4c8b0)(const char* text);
extern PFNLOGSINK_00c4c8b0 g_pfnLogSink_0143bb2c;   // 0x0143BB2C

class CString { public:                  // MFC-style string: one word, the shared empty data for the default constructor
    char* m_pchData;
    CString() : m_pchData(g_pchNil_0139c900) { }
    ~CString();                       // 0x00C0BA40
    CString& __fastcall operator<<(const char* text);   // 0x00C0DEC0
    CString& __fastcall operator<<(long value);   // 0x00C0DF00
};
struct CDebugBreak {                   // empty temporary (a debug-break helper): lands on a dead frame byte
    CDebugBreak();                         // 0x00C16750
};

struct ICompletionCallback_00c4cd80 {      // the master object's completion callback (+0x0C)
    virtual void OnComplete(bool success, void* context);   // slot 0
};

struct CMasterObject_00c4cd80 {
    unsigned long m_count;                 // +0x00 objects received
    unsigned long m_target;                // +0x04 objects sent
    long          m_failures;              // +0x08
    ICompletionCallback_00c4cd80* m_pCallback;   // +0x0c
    unsigned char m_section[0x18];         // +0x10: the profiling section
    bool          m_finalised;             // +0x28
    void*         m_pContext;              // +0x2c
    void Complete();
};

void CMasterObject_00c4cd80::Complete()
{
    PKCProtectionInstance lock((PKCCriticalSection*)m_section);
    if (m_count <= 0) {
        {
            CString line;
            line << ".\\PKRACMasterObject.cpp" << "(" << 29L << ") : " << "No objects have been sent.... deeeerrrr" << "\n";
            if (g_pfnLogSink_0143bb2c)
                g_pfnLogSink_0143bb2c(line.m_pchData);
        }
        CDebugBreak();
    }
    if (m_count != m_target) {
        {
            CString line;
            line << ".\\PKRACMasterObject.cpp" << "(" << 30L << ") : " << "There are still objects out there..." << "\n";
            if (g_pfnLogSink_0143bb2c)
                g_pfnLogSink_0143bb2c(line.m_pchData);
        }
        CDebugBreak();
    }
    if (m_pCallback == 0) {
        {
            CString line;
            line << ".\\PKRACMasterObject.cpp" << "(" << 31L << ") : " << "Null callback" << "\n";
            if (g_pfnLogSink_0143bb2c)
                g_pfnLogSink_0143bb2c(line.m_pchData);
        }
        CDebugBreak();
    }
    if (!m_finalised) {
        {
            CString line;
            line << ".\\PKRACMasterObject.cpp" << "(" << 32L << ") : " << "Dependancy list still being created..." << "\n";
            if (g_pfnLogSink_0143bb2c)
                g_pfnLogSink_0143bb2c(line.m_pchData);
        }
        CDebugBreak();
    }
    m_pCallback->OnComplete(m_failures == 0, m_pContext);
}