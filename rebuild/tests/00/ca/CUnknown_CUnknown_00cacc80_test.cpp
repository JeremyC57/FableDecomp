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
// CUnknown::CUnknown(const WCHAR*, IUnknown*): counts the live object, starts at reference count 0 and owns itself
// (its non-delegating IUnknown) unless an outer unknown aggregates it.
struct IUnknown { virtual ULONG AddRef() = 0; };
struct INonDelegatingUnknown { virtual ULONG NonDelegatingAddRef() = 0; };
static LONG InterlockedIncrement(LONG* p) { return ++*p; }
class CBaseObject {
public:
    static LONG m_cObjects;
    CBaseObject(const WCHAR*) { InterlockedIncrement(&m_cObjects); }
};
LONG CBaseObject::m_cObjects = 0;
class CUnknown : public INonDelegatingUnknown, public CBaseObject {
public:
    IUnknown* const m_pUnknown;
    LONG m_cRef;
    CUnknown(const WCHAR* pName, IUnknown* pUnk)
    : CBaseObject(pName)
    , m_pUnknown( pUnk != 0 ? pUnk : reinterpret_cast<IUnknown*>( static_cast<INonDelegatingUnknown*>(this) ) )
    , m_cRef(0)
    {
    }
    ULONG NonDelegatingAddRef() { return ++m_cRef; }
};
struct COuter : IUnknown { ULONG AddRef() { return 1; } };
static void run()
{
    CUnknown a(0, 0);
    CHECK(CBaseObject::m_cObjects == 1); CHECK(a.m_cRef == 0);
    CHECK((void*)a.m_pUnknown == (void*)static_cast<INonDelegatingUnknown*>(&a));
    COuter outer;
    CUnknown b(0, &outer);
    CHECK(CBaseObject::m_cObjects == 2); CHECK(b.m_pUnknown == &outer); CHECK(b.m_cRef == 0);
}

int main()
{
    run();
    if (g_fail) { std::printf("00cacc80_TEST FAIL\n"); return 1; }
    std::printf("00cacc80_TEST PASS\n");
    return 0;
}