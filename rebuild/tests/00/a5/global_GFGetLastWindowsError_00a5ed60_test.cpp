// Behaviour test for 0x00A5ED60 GFGetLastWindowsError()
#include <cstdio>
#include <cstdlib>
#include <cstring>

// stand-ins for the KERNEL32 imports (GetLastError / FormatMessageW / LocalFree)
static unsigned long g_lastError = 0;
static int g_localFrees = 0;
static unsigned long GetLastErrorStub() { return g_lastError; }
static unsigned long FormatMessageWStub(unsigned long flags, const void*, unsigned long messageId, unsigned long,
                                        wchar_t* buffer, unsigned long, char**)
{
    if (flags != 0x1100 || messageId != 5) return 0;             // only "access denied" has system text here
    const char* text = "Access is denied.";
    unsigned long n = (unsigned long)std::strlen(text);
    wchar_t* mem = (wchar_t*)std::malloc((n + 1) * sizeof(wchar_t));
    for (unsigned long i = 0; i <= n; ++i) mem[i] = (wchar_t)text[i];
    *(wchar_t**)buffer = mem;                                    // FORMAT_MESSAGE_ALLOCATE_BUFFER
    return n;
}
static void* LocalFreeStub(void* mem) { std::free(mem); ++g_localFrees; return 0; }

class CCharString {
public:
    char* PStringData;
    CCharString(const char* text, long length = -1)
    {
        unsigned long n = length < 0 ? (unsigned long)std::strlen(text) : (unsigned long)length;
        PStringData = (char*)std::malloc(n + 1);
        std::memcpy(PStringData, text, n);
        PStringData[n] = 0;
    }
    CCharString(const CCharString& other)
    {
        unsigned long n = (unsigned long)std::strlen(other.PStringData);
        PStringData = (char*)std::malloc(n + 1);
        std::memcpy(PStringData, other.PStringData, n + 1);
    }
    ~CCharString() { std::free(PStringData); }
};
class CWideString {
public:
    const wchar_t* PStringData;
    CWideString(const wchar_t* text) : PStringData(text) { }
    ~CWideString() { }
    CCharString ConvertToCharString() const                      // narrows each UTF-16 unit
    {
        char tmp[256];
        int i = 0;
        for (; PStringData[i] != 0 && i < 255; ++i) tmp[i] = (char)PStringData[i];
        tmp[i] = 0;
        return CCharString(tmp);
    }
};

CCharString GFGetLastWindowsError()
{
    unsigned long error = GetLastErrorStub();
    wchar_t* buffer;
    if (FormatMessageWStub(0x1100, 0, error, 0, (wchar_t*)&buffer, 0, 0) > 0) {
        CCharString message = CWideString(buffer).ConvertToCharString();
        LocalFreeStub(buffer);
        return message;
    }
    return CCharString("");
}

int main()
{
    g_lastError = 5;
    CCharString a = GFGetLastWindowsError();
    g_lastError = 0;
    CCharString b = GFGetLastWindowsError();
    if (std::strcmp(a.PStringData, "Access is denied.") != 0 || b.PStringData[0] != 0 || g_localFrees != 1) {
        std::printf("00a5ed60_TEST FAIL\n");
        return 1;
    }
    std::printf("00a5ed60_TEST PASS\n");
    return 0;
}