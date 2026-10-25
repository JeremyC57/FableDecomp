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
// CMediaSample::SetMediaType: drop the old copy, NULL resets the type-changed flag, otherwise store a copy
// (E_OUTOFMEMORY when the copy fails) and set the flag.
struct AM_MEDIA_TYPE { int id; };
static int g_deletes = 0; static BOOL g_failCreate = 0;
static void DeleteMediaType(AM_MEDIA_TYPE* p) { g_deletes++; delete p; }
static AM_MEDIA_TYPE* CreateMediaType(const AM_MEDIA_TYPE* p) { if (g_failCreate) return 0; AM_MEDIA_TYPE* c = new AM_MEDIA_TYPE; *c = *p; return c; }
class CMediaSample {
public:
    enum { Sample_TypeChanged = 0x08 };
    DWORD m_dwFlags; AM_MEDIA_TYPE* m_pMediaType;
    CMediaSample() : m_dwFlags(0x01), m_pMediaType(0) {}
    HRESULT SetMediaType(AM_MEDIA_TYPE* pMediaType)
    {
        if (m_pMediaType) {
            DeleteMediaType(m_pMediaType);
            m_pMediaType = 0;
        }
        if (pMediaType == 0) {
            m_dwFlags &= ~Sample_TypeChanged;
            return 0;
        }
        m_pMediaType = CreateMediaType(pMediaType);
        if (m_pMediaType == 0) {
            m_dwFlags &= ~Sample_TypeChanged;
            return HR(0x8007000E);
        }
        m_dwFlags |= Sample_TypeChanged;
        return 0;
    }
};
static void run()
{
    CMediaSample s; AM_MEDIA_TYPE mt = { 42 };
    CHECK(s.SetMediaType(&mt) == 0); CHECK(s.m_pMediaType != 0 && s.m_pMediaType != &mt && s.m_pMediaType->id == 42);
    CHECK(s.m_dwFlags == 0x09);
    CHECK(s.SetMediaType(&mt) == 0); CHECK(g_deletes == 1);
    CHECK(s.SetMediaType(0) == 0); CHECK(s.m_pMediaType == 0); CHECK(s.m_dwFlags == 0x01); CHECK(g_deletes == 2);
    s.SetMediaType(&mt); g_failCreate = 1;
    CHECK(s.SetMediaType(&mt) == HR(0x8007000E)); CHECK(s.m_pMediaType == 0); CHECK(s.m_dwFlags == 0x01); CHECK(g_deletes == 3);
}

int main()
{
    run();
    if (g_fail) { std::printf("00ca93f0_TEST FAIL\n"); return 1; }
    std::printf("00ca93f0_TEST PASS\n");
    return 0;
}