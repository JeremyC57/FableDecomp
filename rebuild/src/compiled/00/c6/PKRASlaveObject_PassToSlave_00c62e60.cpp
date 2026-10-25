// PKRASlaveObject::PassToSlave(void* data, PKRASlaveObject* slave) -- hand the data to another slave object's completion slot, reporting a null slave
// Leaf: ?PassToSlave@PKRASlaveObject@@
// Flags: /EHsc
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C62E60, 195 bytes (thiscall, `ret 8`; `this` unused). Vtable 0x012B93D4 slot 2
//   (slot 0 = the deleting destructor 0x00C62F30, slot 1 = 0x00C62E40: hand `data` to the owner 0x00C4C7A0, then delete this).
// Callees: the libpk message stream (`__fastcall operator<<` text 0x00C0DEC0 / number 0x00C0DF00, destructor 0x00C0BA40),
//   the report hook 0x0143BB2C, CDebugBreak 0x00C16750, then `slave` vtable slot 1 (+4) with (data, 0).
// Identity: the manifest's "PKRASlaveObject_ExecuteAction" names the class from the file string ".\PKRASlaveObject.cpp";
//   the message is "Null slave object" at line 0x1c (28). No PDB covers libpk: member and slot names are descriptive.
// Shape: chunk 436's PK_REPORT_ERROR macro (stream in the push-ecx slot, the CDebugBreak temporary homed in the dead
//   `slave` argument slot); execution continues into the virtual call after the report.
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
            s << ".\\PKRASlaveObject.cpp" << "(" << (long)(line) << ") : " << text << "\n"; \
            if (g_pfnErrorReport)                                                           \
                g_pfnErrorReport(s.m_pBuffer);                                              \
        }                                                                                   \
        CDebugBreak();                                                                      \
    }

class PKRASlaveObject {                     // vtable 0x012B93D4
public:
    virtual ~PKRASlaveObject();                                  // slot 0 (deleting wrapper 0x00C62F30)
    virtual void OnComplete(void* data, long flags);             // slot 1 (0x00C62E40)
    virtual void PassToSlave(void* data, PKRASlaveObject* slave);   // slot 2
    void* m_pOwner;                         // +0x04
};

void PKRASlaveObject::PassToSlave(void* data, PKRASlaveObject* slave)
{
    if (!slave)
        PK_REPORT_ERROR(0x1c, "Null slave object");
    slave->OnComplete(data, 0);
}