// Behaviour test (standalone; compiled separately from the source). Plain C++98: the COM interfaces are modelled
// as ordinary virtual classes (no calling-convention keywords), the Win32 imports as local fakes.
#include <cstdio>
#include <cstring>
typedef int HRESULT;
typedef unsigned int ULONG;
typedef unsigned int DWORD;
typedef int LONG;
typedef int BOOL;
typedef void* HANDLE;
typedef unsigned short WCHAR;
typedef unsigned char BYTE;
#ifdef _MSC_VER
typedef __int64 LONGLONG;
#else
typedef long long LONGLONG;
#endif
typedef LONGLONG REFERENCE_TIME;
#ifdef _MSC_VER
#define LL(x) x##i64
#else
#define LL(x) x##LL
#endif
struct GUID { unsigned int Data1; unsigned short Data2; unsigned short Data3; unsigned char Data4[8]; };
typedef GUID IID;
inline bool operator==(const GUID& a, const GUID& b) { return std::memcmp(&a, &b, sizeof(GUID)) == 0; }
#define HR(x) ((HRESULT)(x))
static int g_fail = 0;
#define CHECK(c) do { if (!(c)) { g_fail = 1; std::printf("check failed at line %d\n", __LINE__); } } while (0)
// LoadOLEAut32: load OleAut32.dll once and cache the module handle.
struct HINSTANCE__ { int unused; };
typedef HINSTANCE__* HINSTANCE;
static HINSTANCE hlibOLEAut32 = 0; static int g_loads = 0; static HINSTANCE__ g_lib; static BOOL g_fail_load = 0;
static WCHAR g_seen[32];
static HINSTANCE LoadLibraryW(const WCHAR* name)
{
    int i = 0; for (; name[i] && i < 31; i++) g_seen[i] = name[i]; g_seen[i] = 0;
    g_loads++; return g_fail_load ? 0 : &g_lib;
}
static const WCHAR szOle32Aut[] = { 'O','l','e','A','u','t','3','2','.','d','l','l', 0 };
static HINSTANCE LoadOLEAut32()
{
    if (hlibOLEAut32 == 0) {
        hlibOLEAut32 = LoadLibraryW(szOle32Aut);
    }
    return hlibOLEAut32;
}
static void run()
{
    g_fail_load = 1;
    CHECK(LoadOLEAut32() == 0); CHECK(g_loads == 1);
    g_fail_load = 0;
    CHECK(LoadOLEAut32() == &g_lib); CHECK(g_loads == 2);
    CHECK(LoadOLEAut32() == &g_lib); CHECK(g_loads == 2);
    CHECK(g_seen[0] == 'O' && g_seen[3] == 'A' && g_seen[8] == '.' && g_seen[12] == 0);
}

int main()
{
    run();
    if (g_fail) { std::printf("00cacb20_TEST FAIL\n"); return 1; }
    std::printf("00cacb20_TEST PASS\n");
    return 0;
}