// Behaviour test for CAFile::PathExists (0x00999230): expands the path against the current path and asks
// GetFileAttributesW; an existing file and directory report true, a missing name false.
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef unsigned short wchar16;

extern "C" __declspec(dllimport) unsigned long __stdcall GetFileAttributesW(const wchar16* fileName);

static int g_live = 0;

class CWideString
{
public:
    void* PData;
    CWideString() : PData(0) { ++g_live; }
    CWideString(const char* s) : PData(0) { ++g_live; Set(s); }
    CWideString(const CWideString& o) : PData(0) { ++g_live; *this = o; }
    ~CWideString() { --g_live; free(PData); }
    void Set(const char* s)
    {
        int n = (int)strlen(s), i;
        wchar16* p = (wchar16*)malloc((n + 1) * sizeof(wchar16));
        for (i = 0; i <= n; ++i) p[i] = (wchar16)(unsigned char)s[i];
        free(PData); PData = p;
    }
    CWideString& operator=(const CWideString& other)
    {
        if (this != &other) {
            const wchar16* q = (const wchar16*)other.PData;
            int n = 0;
            while (q && q[n]) ++n;
            free(PData);
            PData = malloc((n + 1) * sizeof(wchar16));
            if (q) memcpy(PData, q, (n + 1) * sizeof(wchar16)); else *(wchar16*)PData = 0;
        }
        return *this;
    }
    operator const wchar16*() const { return (const wchar16*)PData; }
};

class CAFile
{
public:
    static CWideString __fastcall GetCurrentPath() { return CWideString("."); }
    static CWideString __fastcall ExpandPath(const CWideString& path, const CWideString& base)
    {
        char buffer[260];
        const wchar16* b = base; const wchar16* p = path;
        int n = 0, i;
        for (i = 0; b[i]; ++i) buffer[n++] = (char)b[i];
        buffer[n++] = '\\';
        for (i = 0; p[i]; ++i) buffer[n++] = (char)p[i];
        buffer[n] = 0;
        return CWideString(buffer);
    }
    static bool __fastcall PathExists(const CWideString& path);
};

bool __fastcall CAFile::PathExists(const CWideString& path)
{
    CWideString fullPath;
    fullPath = ExpandPath(path, GetCurrentPath());
    if (GetFileAttributesW(fullPath) != 0xFFFFFFFF)
        return true;
    return false;
}

int main()
{
    FILE* f = fopen("pathexists_probe.tmp", "w");
    if (!f) { printf("FAIL setup\n"); return 1; }
    fputs("x", f); fclose(f);
    {
        CWideString present("pathexists_probe.tmp"), missing("no_such_file_00999230.tmp"), dir(".");
        if (!CAFile::PathExists(present)) { printf("FAIL present\n"); return 1; }
        if (CAFile::PathExists(missing)) { printf("FAIL missing\n"); return 1; }
        if (!CAFile::PathExists(dir)) { printf("FAIL directory\n"); return 1; }
        if (g_live != 3) { printf("FAIL temporaries %d\n", g_live); return 1; }
    }
    remove("pathexists_probe.tmp");
    printf("FABLETLC_PATH_EXISTS_00999230 PASS\n");
    return 0;
}
