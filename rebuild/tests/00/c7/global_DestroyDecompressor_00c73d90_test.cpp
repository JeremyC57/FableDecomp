#include <cstdio>
#include <cstring>
#ifndef _MSC_VER
#define __cdecl
#define __stdcall
#define __fastcall
#define __declspec(x)
typedef long long __int64;
#endif
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
static int fail() { std::printf("00c73d90_TEST FAIL\n"); return 1; }
static bool nearly(float a, float b) { float d = a - b; if (d < 0) d = -d; return d < 1e-3f; }
static char g_text[256];
char* g_pDebugBuffer = g_text;
static char g_reported[256]; static int g_reports = 0, g_breaks = 0, g_streams = 0;
static void __fastcall Report(const char* text) { std::strcpy(g_reported, text); ++g_reports; }
void (__fastcall* g_pfnErrorReport)(const char* text) = Report;
CDebugStream& __fastcall CDebugStream::operator<<(const char* text) { std::strcat(m_pBuffer, text); return *this; }
CDebugStream& __fastcall CDebugStream::operator<<(long value) { std::sprintf(m_pBuffer + std::strlen(m_pBuffer), "%ld", value); return *this; }
CDebugStream::~CDebugStream() { ++g_streams; m_pBuffer[0] = 0; }
CDebugBreak::CDebugBreak() { ++g_breaks; }
CDecompressor::~CDecompressor() { }
static int g_destroyed = 0;
class CTestDecompressor : public CDecompressor {
public:
    ~CTestDecompressor() { ++g_destroyed; }
};
int main()
{
    DestroyDecompressor(new CTestDecompressor);
    if (g_destroyed != 1 || g_reports != 0 || g_breaks != 0) return fail();
    DestroyDecompressor(0);
    if (g_destroyed != 1 || g_reports != 1 || g_breaks != 1 || g_streams != 1) return fail();
    if (std::strcmp(g_reported, ".\\CDecompressorInterface.cpp(58) : Null decompressor\n") != 0) return fail();
    g_pfnErrorReport = 0;                                     // no hook installed: still breaks
    DestroyDecompressor(0);
    if (g_reports != 1 || g_breaks != 2) return fail();
    std::printf("00c73d90_TEST PASS\n");
    return 0;
}