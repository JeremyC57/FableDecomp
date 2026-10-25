// DestroyDecompressor(CDecompressor* decompressor) -- delete a stream decompressor made by the CDecompressorInterface factory, reporting "Null decompressor" (file .\CDecompressorInterface.cpp line 58) instead when handed a null pointer
// Leaf: ?DestroyDecompressor@@
// Flags: /EHsc
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C73D90, length 196 bytes (+ int3 padding), __fastcall (decompressor in
//   ecx), plain `ret`.
// Identity: libpk (no PDB/RTTI), file string ".\CDecompressorInterface.cpp", the file of the decompressor factory just above it
//   (ending 0x00C73D8F; it news the 0x340- / 0x10-byte decompressors by type and reports at lines 40 / 45). The catalog's
//   "MessageDecompressor_Release" is propagated; names are descriptive. Caller: 0x00C6275A (a stream's cleanup, which
//   tests the +4 decompressor pointer, calls this and clears it).
// Callees: the libpk message stream (`__fastcall operator<<` text 0x00C0DEC0 / number 0x00C0DF00, destructor 0x00C0BA40),
//   the report hook 0x0143BB2C, CDebugBreak 0x00C16750; otherwise the decompressor's virtual scalar deleting
//   destructor (slot 0, flag 1).
// Shape: chunk 436's PK_REPORT_ERROR macro in the null arm of an if/else (EH state 0 stored from the zero ecx); the
//   else arm's `delete` drops its own null test.
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
#define PK_REPORT_ERROR(line, text)                                                                 \
    {                                                                                               \
        {                                                                                           \
            CDebugStream s(g_pDebugBuffer);                                                         \
            s << ".\\CDecompressorInterface.cpp" << "(" << (long)(line) << ") : " << text << "\n";  \
            if (g_pfnErrorReport)                                                                   \
                g_pfnErrorReport(s.m_pBuffer);                                                      \
        }                                                                                           \
        CDebugBreak();                                                                              \
    }

class CDecompressor {                       // polymorphic stream decompressor
public:
    virtual ~CDecompressor();               // slot 0 (scalar deleting destructor)
};

void __fastcall DestroyDecompressor(CDecompressor* decompressor)
{
    if (!decompressor)
        PK_REPORT_ERROR(58, "Null decompressor")
    else
        delete decompressor;
}