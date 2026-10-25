// Behaviour test for 0x00402130 FableCheckMyDocumentsWritePermissions: a UNC ("\\\\...") My Documents path reports the
// REDIRECTED error key's CRC; otherwise a probe file (path + the warning sentence) is created, reopened "w+" and deleted,
// and any failing step reports the RESTRICTED key's CRC; a writable local folder returns 0. The probe is always removed
// when it could be deleted, and every handle/stream opened is closed.
#include <cstdio>
#include <cstring>
#include <cwchar>

static int g_failStep = 0;          // 1 CreateFileW fails, 2 _wfopen fails, 3 DeleteFileW fails
static int g_openHandles = 0, g_openStreams = 0, g_created = 0, g_deleted = 0;
static wchar_t g_lastName[256];

static void* CreateFileW_(const wchar_t* name, unsigned long access, unsigned long share, void*, unsigned long disp, unsigned long, void*)
{
    std::wcscpy(g_lastName, name);
    if (g_failStep == 1 || access != 0x10000000 || share != 3 || disp != 2) return (void*)-1;
    ++g_openHandles; ++g_created; return (void*)0x1234;
}
static int CloseHandle_(void* h) { if (h == (void*)0x1234) --g_openHandles; return 1; }
static int g_stream;
static void* wfopen_(const wchar_t* name, const wchar_t* mode)
{
    if (g_failStep == 2 || std::wcscmp(mode, L"w+") != 0 || std::wcscmp(name, g_lastName) != 0) return 0;
    ++g_openStreams; return &g_stream;
}
static int fclose_(void* f) { if (f == &g_stream) --g_openStreams; return 0; }
static int DeleteFileW_(const wchar_t* name) { if (g_failStep == 3 || std::wcscmp(name, g_lastName) != 0) return 0; ++g_deleted; return 1; }

struct CWideString {
    wchar_t s[256];
    bool null;
    CWideString() : null(true) { s[0] = 0; }
    CWideString(const wchar_t* t) : null(false) { std::wcscpy(s, t); }
    CWideString GetSubString(unsigned long start, unsigned long end) const
    {
        CWideString r; r.null = false; unsigned long n = 0;
        for (unsigned long i = start; i < end && s[i]; ++i) r.s[n++] = s[i];
        r.s[n] = 0; return r;
    }
    const wchar_t* GetAsCharArray() const { return s; }
    bool operator==(const wchar_t* t) const { return null ? std::wcscmp(t, L"") == 0 : std::wcscmp(s, t) == 0; }
};
static CWideString operator+(const CWideString& a, const wchar_t* b) { CWideString r(a.s); std::wcscat(r.s, b); return r; }
struct CCharString {
    char s[128];
    CCharString() { s[0] = 0; }
    CCharString& operator=(const char* t) { std::strcpy(s, t); return *this; }
    bool operator!=(const char* t) const { return std::strcmp(s, t) != 0; }
};
static unsigned long crc(const char* p)
{
    unsigned long c = 0;
    for (; *p; ++p) { c ^= (unsigned char)*p; for (int k = 0; k < 8; ++k) c = (c >> 1) ^ (0xEDB88320u & (0u - (c & 1))); }
    return c;
}

// the function under test (shape of 0x00402130)
static unsigned long FableCheckMyDocumentsWritePermissions(const CWideString& myDocumentsPath)
{
    CCharString errorText;
    if (myDocumentsPath.GetSubString(0, 2) == L"\\\\")
        errorText = "TEXT_GUI_WINLOGO_ERROR_MY_DOCS_FOLDER_REDIRECTED";
    else {
        CWideString testFile = myDocumentsPath + L"This directory has restricted permissions, and Fable cannot run properly";
        void* handle = CreateFileW_(testFile.GetAsCharArray(), 0x10000000, 3, 0, 2, 0, 0);
        if (handle == (void*)-1)
            errorText = "TEXT_GUI_WINLOGO_ERROR_MY_DOCS_FOLDER_RESTRICTED";
        else {
            CloseHandle_(handle);
            void* file = wfopen_(testFile.GetAsCharArray(), L"w+");
            if (file == 0)
                errorText = "TEXT_GUI_WINLOGO_ERROR_MY_DOCS_FOLDER_RESTRICTED";
            else {
                fclose_(file);
                if (!DeleteFileW_(testFile.GetAsCharArray()))
                    errorText = "TEXT_GUI_WINLOGO_ERROR_MY_DOCS_FOLDER_RESTRICTED";
            }
        }
    }
    if (errorText != "")
        return crc(errorText.s);
    return 0;
}

int main()
{
    int ok = 1;
    const unsigned long redirected = crc("TEXT_GUI_WINLOGO_ERROR_MY_DOCS_FOLDER_REDIRECTED");
    const unsigned long restricted = crc("TEXT_GUI_WINLOGO_ERROR_MY_DOCS_FOLDER_RESTRICTED");
    CWideString local(L"C:\\Users\\Hero\\Documents\\");
    CWideString unc(L"\\\\server\\share\\Documents\\");
    g_failStep = 0;
    if (FableCheckMyDocumentsWritePermissions(local) != 0) ok = 0;
    if (g_created != 1 || g_deleted != 1) ok = 0;
    if (std::wcscmp(g_lastName, L"C:\\Users\\Hero\\Documents\\This directory has restricted permissions, and Fable cannot run properly") != 0) ok = 0;
    if (FableCheckMyDocumentsWritePermissions(unc) != redirected || g_created != 1) ok = 0;   // no probe for a UNC path
    for (int step = 1; step <= 3; ++step) {
        g_failStep = step;
        if (FableCheckMyDocumentsWritePermissions(local) != restricted) ok = 0;
    }
    if (g_deleted != 1) ok = 0;
    // a one-character or empty path is not UNC
    g_failStep = 0;
    if (FableCheckMyDocumentsWritePermissions(CWideString(L"\\")) != 0) ok = 0;
    if (FableCheckMyDocumentsWritePermissions(CWideString()) != 0) ok = 0;
    if (g_openHandles != 0 || g_openStreams != 0) ok = 0;
    std::printf(ok ? "00402130_TEST PASS\n" : "00402130_TEST FAIL\n");
    return ok ? 0 : 1;
}