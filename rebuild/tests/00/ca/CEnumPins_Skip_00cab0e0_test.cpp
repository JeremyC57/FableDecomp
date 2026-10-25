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
// CEnumPins::Skip: out-of-sync check against the filter's pin version, S_FALSE when skipping past the end.
#define VFW_E_ENUM_OUT_OF_SYNC HR(0x80040203)
struct CBaseFilter { LONG ver; LONG GetPinVersion() { return ver; } };
class CEnumPins {
public:
    int m_Position; int m_PinCount; CBaseFilter* m_pFilter; LONG m_Version;
    BOOL AreWeOutOfSync() { return (m_pFilter->GetPinVersion() == m_Version ? 0 : 1); }
    HRESULT Skip(ULONG cPins)
    {
        if (AreWeOutOfSync() == 1) {
            return VFW_E_ENUM_OUT_OF_SYNC;
        }
        ULONG PinsLeft = m_PinCount - m_Position;
        if (cPins > PinsLeft) {
            return 1;
        }
        m_Position += cPins;
        return 0;
    }
};
static void run()
{
    CBaseFilter f; f.ver = 3;
    CEnumPins e; e.m_pFilter = &f; e.m_Version = 3; e.m_Position = 1; e.m_PinCount = 4;
    CHECK(e.Skip(2) == 0); CHECK(e.m_Position == 3);
    CHECK(e.Skip(2) == 1); CHECK(e.m_Position == 3);
    CHECK(e.Skip(1) == 0); CHECK(e.m_Position == 4);
    CHECK(e.Skip(0) == 0);
    f.ver = 4;
    CHECK(e.Skip(0) == VFW_E_ENUM_OUT_OF_SYNC); CHECK(e.m_Position == 4);
}

int main()
{
    run();
    if (g_fail) { std::printf("00cab0e0_TEST FAIL\n"); return 1; }
    std::printf("00cab0e0_TEST PASS\n");
    return 0;
}