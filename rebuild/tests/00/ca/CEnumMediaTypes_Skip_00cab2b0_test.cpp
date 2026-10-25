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
// CEnumMediaTypes::Skip: zero is S_OK; out-of-sync check against the pin's type version; advance, then S_OK only
// if the pin still has a media type at the new position - 1.
#define VFW_E_ENUM_OUT_OF_SYNC HR(0x80040203)
static int g_live = 0;
struct CMediaType { CMediaType() { g_live++; } ~CMediaType() { g_live--; } };
struct CBasePin {
    LONG ver; int nTypes;
    LONG GetMediaTypeVersion() { return ver; }
    HRESULT GetMediaType(int iPosition, CMediaType*) { return iPosition < nTypes ? 0 : HR(0x00040103); }
};
class CEnumMediaTypes {
public:
    int m_Position; CBasePin* m_pPin; LONG m_Version;
    BOOL AreWeOutOfSync() { return (m_pPin->GetMediaTypeVersion() == m_Version ? 0 : 1); }
    HRESULT Skip(ULONG cMediaTypes)
    {
        if (cMediaTypes == 0) {
            return 0;
        }
        if (AreWeOutOfSync() == 1) {
            return VFW_E_ENUM_OUT_OF_SYNC;
        }
        m_Position += cMediaTypes;
        CMediaType cmt;
        return 0 == m_pPin->GetMediaType(m_Position - 1, &cmt) ? 0 : 1;
    }
};
static void run()
{
    CBasePin pin; pin.ver = 1; pin.nTypes = 3;
    CEnumMediaTypes e; e.m_pPin = &pin; e.m_Version = 1; e.m_Position = 0;
    CHECK(e.Skip(2) == 0); CHECK(e.m_Position == 2);
    CHECK(e.Skip(1) == 0); CHECK(e.m_Position == 3);
    CHECK(e.Skip(1) == 1); CHECK(e.m_Position == 4);
    CHECK(g_live == 0);
    pin.ver = 2;
    CHECK(e.Skip(0) == 0);
    CHECK(e.Skip(1) == VFW_E_ENUM_OUT_OF_SYNC); CHECK(e.m_Position == 4);
}

int main()
{
    run();
    if (g_fail) { std::printf("00cab2b0_TEST FAIL\n"); return 1; }
    std::printf("00cab2b0_TEST PASS\n");
    return 0;
}