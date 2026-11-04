// CMasterObject_00c4c8b0::LoadSlave(IScriptReader_00c4c8b0* reader, void* a, unsigned long stride, void* d) -- EH-framed under the master object's profiling lock: warn when already finalised (PKRACMasterObject.cpp line 124), new an 8-byte slave object over this (EMEM warning, line 129, when it fails), let the reader load count*stride bytes into it; on failure delete the slave and reset the object, else commit it
// Leaf: ?LoadSlave@CMasterObject_00c4c8b0@@
// Vendor naming V2 (plan phase 1, header-first): PKCProtectionInstance / PKCCriticalSection come from the shared libpk/PKCProtectionInstance.h (was a local copy).
// Vendor naming wave V1 (one address, one name): 0x00C62E20 ??0PKRASlaveObject@@QAE@PAX@Z (was ??0CSlaveObject_00c62c70@@QAE@PAX@Z)
// Flags: /EHsc
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C4CAB0, 471 bytes, thiscall `ret 10h`, /EHsc.
// Identity: libpk PKRACMasterObject.cpp (trace lines 124 / 129); same class and shape as the landed LoadScriptInfo 0x00C4C8B0.
// Callees: PKCProtectionInstance 0x00C0FFC0 / 0x00C0FFE0, the trace stream 0x00C0DEC0 / 0x00C0DF00 / 0x00C0BA40, CDebugBreak
//   0x00C16750, operator new, the slave ctor 0x00C62E20, reader slots 1 (Load) / 2 (GetCount), Reset 0x00C4C330, Commit 0x00C4C6C0.
// Shape: Load's six arguments are pushed in two halves around the GetCount() call.
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

struct CMasterObject_00c4c8b0;
struct CSlaveClass_00c4c8b0 { void* m_vtable; };   // the global slave class object at 0x0139CA38
extern CSlaveClass_00c4c8b0 g_slaveClass_0139ca38;

struct PKRASlaveObject {             // the 8-byte slave object
    virtual void Destroy(int flags);       // deleting destructor
    void* m_pOwner;                        // +0x04
    PKRASlaveObject(void* owner);    // 0x00C62E20
};

struct IScriptReader_00c4c8b0 {            // the reader interface: count at +8, load at +4
    virtual void v00();
    virtual bool Load(void* a, unsigned long stride, unsigned long size, void* d, PKRASlaveObject* slave, CSlaveClass_00c4c8b0* slaveClass);
    virtual unsigned long GetCount();
};

struct CMasterObject_00c4c8b0 {
    unsigned char _pad0[0x10];
    unsigned char m_section[0x18];          // +0x10: the profiling section
    bool m_finalised;                      // +0x28
    void Reset();                          // 0x00C4C330
    void Commit();                         // 0x00C4C6C0
    void LoadSlave(IScriptReader_00c4c8b0* reader, void* a, unsigned long stride, void* d);
};

void CMasterObject_00c4c8b0::LoadSlave(IScriptReader_00c4c8b0* reader, void* a, unsigned long stride, void* d)
{
    PKCProtectionInstance lock((PKCCriticalSection*)m_section);
    if (m_finalised) {
        {
            CString line;
            line << ".\\PKRACMasterObject.cpp" << "(" << 124L << ") : " << "Dependancy already finalised - this could be a dead object!" << "\n";
            if (g_pfnLogSink_0143bb2c)
                g_pfnLogSink_0143bb2c(line.m_pchData);
        }
        CDebugBreak();
    }
    PKRASlaveObject* slave = new PKRASlaveObject(this);
    if (!slave) {
        {
            CString line;
            line << ".\\PKRACMasterObject.cpp" << "(" << 129L << ") : " << "EMEM" << "\n";
            if (g_pfnLogSink_0143bb2c)
                g_pfnLogSink_0143bb2c(line.m_pchData);
        }
        CDebugBreak();
    }
    if (!reader->Load(a, stride, reader->GetCount() * stride, d, slave, &g_slaveClass_0139ca38)) {
        if (slave)
            slave->Destroy(1);
        Reset();
    } else {
        Commit();
    }
}