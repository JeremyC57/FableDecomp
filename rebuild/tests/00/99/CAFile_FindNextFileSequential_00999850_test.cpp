// Behaviour test for 0x00999850 CAFile::FindNextFileSequential (model copy; CRT and path helpers are stubs).
#include <cstdio>
#ifndef _MSC_VER
#define __fastcall
#endif
typedef unsigned short wchar16;
#define _wfindnext t_wfindnext
#define _findclose t_findclose
#define _wsplitpath t_wsplitpath
#define wcscpy t_wcscpy
#define wcscat t_wcscat

struct _wfinddata_t {
    unsigned attrib; long time_create; long time_access; long time_write; unsigned long size; wchar16 name[260];
};
static void Put(wchar16* d, const char* s) { while (*s) *d++ = (wchar16)*s++; *d = 0; }
static bool Same(const wchar16* a, const char* b) { while (*b) if (*a++ != (wchar16)*b++) return false; return *a == 0; }

static int g_remaining = 1; static long g_closed = -1; static long g_lastHandle = -1;
int _wfindnext(long handle, _wfinddata_t* data) { g_lastHandle = handle; if (g_remaining-- > 0) { Put(data->name, "b.big"); return 0; } return -1; }
int _findclose(long handle) { g_closed = handle; return 0; }
void _wsplitpath(const wchar16* path, wchar16* drive, wchar16* dir, wchar16*, wchar16*)
{
    // the stub "expanded" path is always C:\data\*.big
    if (!Same(path, "C:\\data\\*.big")) { drive[0] = 0; dir[0] = 0; return; }
    Put(drive, "C:"); Put(dir, "\\data\\");
}
wchar16* wcscpy(wchar16* d, const wchar16* s) { wchar16* r = d; while ((*d++ = *s++) != 0) {} return r; }
wchar16* wcscat(wchar16* d, const wchar16* s) { wchar16* r = d; while (*d) ++d; while ((*d++ = *s++) != 0) {} return r; }

static int g_strings = 0;
namespace N_0099af00 { struct CWideString {
    wchar16* m_pchData; wchar16 buf[64];
    CWideString() : m_pchData(buf) { buf[0] = 0; ++g_strings; }
    CWideString(const CWideString& o) : m_pchData(buf) { wcscpy(buf, o.buf); ++g_strings; }
    operator const wchar16*() const { return m_pchData; }
    ~CWideString() { --g_strings; }
}; } using N_0099af00::CWideString;
CWideString __fastcall GetCurrentWorkingDirectory() { CWideString s; Put(s.buf, "C:\\data"); return s; }
CWideString __fastcall ExpandPath(const CWideString& path, const CWideString& base)
{
    CWideString s; wcscpy(s.buf, base.buf); wcscat(s.buf, (const wchar16*)"\\\0\0"); wcscat(s.buf, path.buf); return s;
}

struct SFindFileState { bool Finished; long Handle; };
class CAFile { public:
    static bool __fastcall FindNextFileSequential(const CWideString& pattern, SFindFileState& state, wchar16* outPath);
};

bool __fastcall CAFile::FindNextFileSequential(const CWideString& pattern, SFindFileState& state, wchar16* outPath)
{
    wchar16 drive[3];
    wchar16 dir[256];
    _wfinddata_t data;
    CWideString path = ExpandPath(pattern, GetCurrentWorkingDirectory());
    if (!state.Finished) {
        if (_wfindnext(state.Handle, &data) == 0) {
            _wsplitpath(path, drive, dir, 0, 0);
            wcscpy(outPath, drive);
            wcscat(outPath, dir);
            wcscat(outPath, data.name);
            return true;
        }
        state.Finished = true;
        _findclose(state.Handle);
    }
    return false;
}

static int fail() { std::printf("00999850_TEST FAIL\n"); return 1; }

int main()
{
    CWideString pattern; Put(pattern.buf, "*.big");
    SFindFileState state; state.Finished = false; state.Handle = 77;
    wchar16 out[300];
    int before = g_strings;
    if (!CAFile::FindNextFileSequential(pattern, state, out)) return fail();
    if (!Same(out, "C:\\data\\b.big") || g_lastHandle != 77 || state.Finished || g_strings != before) return fail();
    if (CAFile::FindNextFileSequential(pattern, state, out)) return fail();
    if (!state.Finished || g_closed != 77 || g_strings != before) return fail();
    g_lastHandle = -1;
    if (CAFile::FindNextFileSequential(pattern, state, out) || g_lastHandle != -1) return fail();   // finished: no more queries
    std::printf("00999850_TEST PASS\n");
    return 0;
}