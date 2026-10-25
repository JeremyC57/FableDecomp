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
// CBasePin::QueryPinInfo: owning filter (AddRef'd), the pin name copied with lstrcpynW (or empty), the direction.
enum PIN_DIRECTION { PINDIR_INPUT = 0, PINDIR_OUTPUT = 1 };
struct IBaseFilter { int refs; IBaseFilter() : refs(0) {} ULONG AddRef() { return ++refs; } };
struct PIN_INFO { IBaseFilter* pFilter; PIN_DIRECTION dir; WCHAR achName[128]; };
static WCHAR* lstrcpynW(WCHAR* d, const WCHAR* s, int n)
{
    int i = 0;
    for (; i < n - 1 && s[i]; i++) d[i] = s[i];
    d[i] = 0;
    return d;
}
class CBasePin {
public:
    const WCHAR* m_pName; PIN_DIRECTION m_dir; IBaseFilter* m_pFilter;
    HRESULT QueryPinInfo(PIN_INFO* pInfo)
    {
        if (pInfo == 0) return HR(0x80004003);
        pInfo->pFilter = m_pFilter;
        if (m_pFilter) {
            m_pFilter->AddRef();
        }
        if (m_pName) {
            lstrcpynW(pInfo->achName, m_pName, sizeof(pInfo->achName) / sizeof(WCHAR));
        } else {
            pInfo->achName[0] = 0;
        }
        pInfo->dir = m_dir;
        return 0;
    }
};
static void run()
{
    static const WCHAR name[] = { 'I', 'n', 0 };
    IBaseFilter f;
    CBasePin p; p.m_pName = name; p.m_dir = PINDIR_OUTPUT; p.m_pFilter = &f;
    PIN_INFO pi; pi.achName[0] = 'x';
    CHECK(p.QueryPinInfo(&pi) == 0);
    CHECK(pi.pFilter == &f); CHECK(f.refs == 1); CHECK(pi.dir == PINDIR_OUTPUT);
    CHECK(pi.achName[0] == 'I' && pi.achName[1] == 'n' && pi.achName[2] == 0);
    WCHAR longName[200]; for (int i = 0; i < 199; i++) longName[i] = 'a'; longName[199] = 0;
    p.m_pName = longName;
    CHECK(p.QueryPinInfo(&pi) == 0); CHECK(pi.achName[127] == 0 && pi.achName[126] == 'a');
    p.m_pName = 0; p.m_pFilter = 0; p.m_dir = PINDIR_INPUT;
    CHECK(p.QueryPinInfo(&pi) == 0); CHECK(pi.achName[0] == 0); CHECK(pi.pFilter == 0); CHECK(pi.dir == PINDIR_INPUT);
    CHECK(f.refs == 2);
    CHECK(p.QueryPinInfo(0) == HR(0x80004003));
}

int main()
{
    run();
    if (g_fail) { std::printf("00ca8420_TEST FAIL\n"); return 1; }
    std::printf("00ca8420_TEST PASS\n");
    return 0;
}