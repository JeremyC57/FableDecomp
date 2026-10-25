// CShaderResource::LoadPixelShaderData(CString& data) -- serialise a text blob through the PK parser: when reading, fetch the length and the bytes into a NUL-terminated buffer (logging EMEM on allocation failure) and store them in the string; when writing, send the string's length and bytes
// Leaf: ?LoadPixelShaderData@CShaderResource@@QAE_NAAVCString@@@Z
// Flags: /EHsc
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C31B90, length 425 bytes (`ret 4`, /EHsc frame: one state for the log line).
// Label: the catalog name is kept, but the body is a libpk serialiser string transfer (the EMEM log names
// .\PKSerialiseCParser.cpp line 100; called from 0x00C3B7AB / 0x00C3B86E / 0x00C3B9A1): `this` holds the reader at +0.
// Callees: TransferSize 0x00C31B50 (manifest "CMainGameComponent::BeginInputLoading", propagated), GetWriter 0x00C31A70
// (manifest "PKSerialiseCParser::ValidateWriter"), CString::operator=(const char*) 0x00C0C200, CString::SafeStrlen
// 0x00C0BCC0 (static `__stdcall`), operator new 0x00BFEA1A / operator delete 0x00BFE9BC, and the EMEM logging of the
// landed PKCAutoDeleteArray::Resize 0x00C31810 (CString::operator<<(const char*) 0x00C0DEC0 / operator<<(long) 0x00C0DF00
// / ~CString 0x00C0BA40, log sink 0x0143BB2C, empty-string global 0x0139C900, the CTrace temporary 0x00C16750).
// Callee names (sweep1, 2026-10-25): the CString library members as renamed this pass (FINDINGS chunk 437: 0x00C0C200 =
//   CString::operator=(const char*); chunk 438: 0x00C0DEC0/0x00C0DF00 are libpk CString stream operators; the landed
//   CString::SafeStrlen 0x00C0BCC0 -- FINDINGS line 565: it was `StringLength` here, `SafeStrlen` in its own source);
//   CString is a class, as in those sources.
// Shape: reading: size via TransferSize, empty -> "" , else a size+1 buffer (logged but not abandoned when null),
// the reader's slot-0 transfer, NUL, assign, delete. Writing: a block-scoped size (homed in the dead `data`
// argument slot) from the string length; the writer's slot 0 returns an unsigned char tested `? true : false`
// (`test al,al ; setne al`) and takes the text through the string's inline `operator const char*` (so the text is
// read before the size, as retail).
extern char* g_pchNil;                             // 0x0139C900
typedef void (__fastcall* PFNLOGSINK)(const char* text);
extern PFNLOGSINK g_pfnLogSink;                    // 0x0143BB2C
class CString {                                    // MFC-style string: one word
public:
    char* m_pchData;
    operator const char*() const { return m_pchData; }
    CString() : m_pchData(g_pchNil) { }
    ~CString();                                    // 0x00C0BA40
    CString& __fastcall operator<<(const char* text);   // 0x00C0DEC0
    CString& __fastcall operator<<(long value);    // 0x00C0DF00
    const CString& operator=(const char* lpsz);    // 0x00C0C200
    static int __stdcall SafeStrlen(const char* lpsz);   // 0x00C0BCC0
};
struct CTrace {                                    // empty debug-break temporary
    CTrace();                                      // 0x00C16750
};
struct CReader {
    virtual bool Transfer(void* buffer, unsigned long size);   // slot 0
};
struct CWriter {
    virtual unsigned char Transfer(const char* text, unsigned long size);   // slot 0
};
struct CShaderResource {
    CReader* m_pReader;                            // +0
    bool TransferSize(unsigned long* size);        // 0x00C31B50
    CWriter* GetWriter();                          // 0x00C31A70
    bool LoadPixelShaderData(CString& data);
};

bool CShaderResource::LoadPixelShaderData(CString& data)
{
    if (m_pReader) {
        unsigned long size;
        if (!TransferSize(&size))
            return false;
        if (size == 0) {
            data = "";
            return true;
        }
        char* buffer = (char*)operator new(size + 1);
        if (!buffer) {
            {
                CString line;
                line << ".\\PKSerialiseCParser.cpp" << "(" << 100L << ") : " << "EMEM" << "\n";
                if (g_pfnLogSink)
                    g_pfnLogSink(line.m_pchData);
            }
            CTrace();
        }
        if (!m_pReader->Transfer(buffer, size)) {
            operator delete(buffer);
            return false;
        }
        buffer[size] = 0;
        data = buffer;
        operator delete(buffer);
        return true;
    }
    {
        unsigned long size = CString::SafeStrlen(data.m_pchData);
        if (!TransferSize(&size))
            return false;
        if (size == 0)
            return true;
        return GetWriter()->Transfer(data, size) ? true : false;
    }
}