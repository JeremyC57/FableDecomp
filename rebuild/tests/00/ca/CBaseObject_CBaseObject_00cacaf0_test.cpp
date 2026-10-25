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
// CBaseObject::~CBaseObject: the last live object unloads OleAut32.dll if it was loaded.
struct HINSTANCE__ { int unused; };
typedef HINSTANCE__* HINSTANCE;
static HINSTANCE hlibOLEAut32 = 0; static int g_frees = 0;
static LONG InterlockedDecrement(LONG* p) { return --*p; }
static BOOL FreeLibrary(HINSTANCE) { g_frees++; return 1; }
class CBaseObject {
public:
    static LONG m_cObjects;
    CBaseObject() { m_cObjects++; }
    ~CBaseObject()
    {
        if (InterlockedDecrement(&m_cObjects) == 0) {
            if (hlibOLEAut32) {
                FreeLibrary(hlibOLEAut32);
                hlibOLEAut32 = 0;
            }
        };
    }
};
LONG CBaseObject::m_cObjects = 0;
static void run()
{
    static HINSTANCE__ lib;
    CBaseObject* a = new CBaseObject; CBaseObject* b = new CBaseObject;
    hlibOLEAut32 = &lib;
    delete a; CHECK(CBaseObject::m_cObjects == 1); CHECK(g_frees == 0); CHECK(hlibOLEAut32 == &lib);
    delete b; CHECK(CBaseObject::m_cObjects == 0); CHECK(g_frees == 1); CHECK(hlibOLEAut32 == 0);
    CBaseObject* c = new CBaseObject; delete c; CHECK(g_frees == 1);
}

int main()
{
    run();
    if (g_fail) { std::printf("00cacaf0_TEST FAIL\n"); return 1; }
    std::printf("00cacaf0_TEST PASS\n");
    return 0;
}