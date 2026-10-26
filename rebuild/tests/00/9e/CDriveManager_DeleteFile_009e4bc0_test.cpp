#include <cstdio>
#include <cstring>
#include <cmath>
#include <cstdlib>

typedef unsigned short wchar_t16;
static int g_apiOk; static int g_apiCalls = 0, g_lastErr = 0, g_dtors = 0; static const wchar_t16* g_apiArg = 0;
int API(const wchar_t16* p) { g_apiCalls++; g_apiArg = p; return g_apiOk; }
unsigned long GetLastError() { g_lastErr++; return 5; }
class CWideString { public: wchar_t16* m_pchData; operator const wchar_t16*() { return m_pchData; } ~CWideString() { g_dtors++; } };
static wchar_t16 g_full[4] = { 'F', 'U', 'L', 0 };
CWideString ConvertPathToFullPath(const CWideString& p) { CWideString r; r.m_pchData = g_full; (void)p; return r; }
class CDriveManager { public: void* vt; void* sm; long DeleteFile(const CWideString& path) { CWideString fullPath = ConvertPathToFullPath(path); if (API(fullPath)) return 1; GetLastError(); return 0; } };
int main() { wchar_t16 raw[2] = { 'x', 0 }; CWideString in; in.m_pchData = raw; CDriveManager d; g_apiOk = 1; long r1 = d.DeleteFile(in); int d1 = g_dtors; g_apiOk = 0; long r2 = d.DeleteFile(in);
    if (r1 != 1 || r2 != 0 || g_apiCalls != 2 || g_apiArg != g_full || g_lastErr != 1 || d1 < 1) { std::printf("009e4bc0_TEST FAIL\n"); return 1; }
    std::printf("009e4bc0_TEST PASS\n"); return 0; }