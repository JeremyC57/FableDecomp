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
// CEnumMediaTypes::CEnumMediaTypes: AddRef the pin; a new enumerator snapshots the pin's type version, a clone copies
// the source enumerator's position and version; the reference count starts at 1.
struct CBasePin { int refs; LONG ver; CBasePin() : refs(0), ver(0) {} ULONG AddRef() { return ++refs; } LONG GetMediaTypeVersion() { return ver; } };
class CEnumMediaTypes {
public:
    int m_Position; CBasePin* m_pPin; LONG m_Version; LONG m_cRef;
    CEnumMediaTypes(CBasePin* pPin, CEnumMediaTypes* pEnumMediaTypes) :
        m_Position(0),
        m_pPin(pPin),
        m_cRef(1)
    {
        m_pPin->AddRef();
        if (pEnumMediaTypes == 0) {
            m_Version = m_pPin->GetMediaTypeVersion();
            return;
        }
        m_Position = pEnumMediaTypes->m_Position;
        m_Version = pEnumMediaTypes->m_Version;
    }
};
static void run()
{
    CBasePin pin; pin.ver = 7;
    CEnumMediaTypes a(&pin, 0);
    CHECK(a.m_Position == 0); CHECK(a.m_Version == 7); CHECK(a.m_cRef == 1); CHECK(a.m_pPin == &pin); CHECK(pin.refs == 1);
    a.m_Position = 2; a.m_cRef = 5; pin.ver = 8;
    CEnumMediaTypes b(&pin, &a);
    CHECK(b.m_Position == 2); CHECK(b.m_Version == 7); CHECK(b.m_cRef == 1); CHECK(pin.refs == 2);
}

int main()
{
    run();
    if (g_fail) { std::printf("00cabf70_TEST FAIL\n"); return 1; }
    std::printf("00cabf70_TEST PASS\n");
    return 0;
}