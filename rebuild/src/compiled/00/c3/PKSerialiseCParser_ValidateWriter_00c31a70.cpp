// PKSerialiseCParser::ValidateWriter() -- returns the parser's writer (+4); when there is none, logs the "Null writer" assertion (PKSerialiseCParser.cpp line 17) through the log sink and breaks into the trace helper first
// Leaf: ValidateWriter
// Flags: /EHsc
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C31A70, 189 bytes (+ int3 padding), __fastcall, plain `ret`, EH frame (handler 0x0121414A).
// Callees: the MFC-style string's Append 0x00C0DEC0 and AppendNumber 0x00C0DF00 (`__fastcall`, return the string),
//   its destructor 0x00C0BA40, the trace temporary's constructor 0x00C16750; log sink pointer 0x0143BB2C, shared empty
//   string data 0x0139C900. Strings: 0x012B4DC4 ".\PKSerialiseCParser.cpp", 0x0122E028 "(", 0x012B1AE4 ") : ",
//   0x012B4DE0 "Null writer", 0x0129B208 "\n".
// Shape: the assertion macro as in the landed CBufferedReader seek 0x00C4AA40 (string built in a nested block, EH state 0
//   while it lives, the empty trace temporary after the block); the writer is re-read for the return value.
extern char* g_pchNil;                              // 0x0139C900
typedef void (__fastcall* PFNLOGSINK)(const char* text);
extern PFNLOGSINK g_pfnLogSink;                     // 0x0143BB2C

struct CAssertString {                              // MFC-style string: one word, the shared empty data for the default constructor
    char* m_pchData;
    CAssertString() : m_pchData(g_pchNil) { }
    ~CAssertString();                               // 0x00C0BA40
    CAssertString* __fastcall Append(const char* text);   // 0x00C0DEC0
    CAssertString* __fastcall AppendNumber(long value);   // 0x00C0DF00
};

struct CAssertTrace {                               // empty temporary (a debug-break helper)
    CAssertTrace();                                 // 0x00C16750
};

class PKSerialiseCWriter;

class PKSerialiseCParser {
public:
    long m_0;
    PKSerialiseCWriter* m_pWriter;                  // +4
    PKSerialiseCWriter* ValidateWriter();
};

PKSerialiseCWriter* PKSerialiseCParser::ValidateWriter()
{
    if (!m_pWriter) {
        {
            CAssertString line;
            line.Append(".\\PKSerialiseCParser.cpp")->Append("(")->AppendNumber(17)->Append(") : ")->Append("Null writer")->Append("\n");
            if (g_pfnLogSink)
                g_pfnLogSink(line.m_pchData);
        }
        CAssertTrace();
    }
    return m_pWriter;
}