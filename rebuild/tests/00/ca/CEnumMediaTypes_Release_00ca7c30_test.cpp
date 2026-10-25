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
// CEnumMediaTypes::Release: interlocked decrement, delete on the last reference, return the new count.
static int g_deleted = 0;
static LONG InterlockedDecrement(LONG* p) { return --*p; }
class CEnumMediaTypes {
public:
    int m_Position; void* m_pPin; LONG m_Version; LONG m_cRef;
    CEnumMediaTypes() : m_Position(0), m_pPin(0), m_Version(0), m_cRef(1) {}
    virtual ~CEnumMediaTypes() { g_deleted++; }
    ULONG Release()
    {
        ULONG cRef = InterlockedDecrement(&m_cRef);
        if (cRef == 0) {
            delete this;
        }
        return cRef;
    }
};
static void run()
{
    CEnumMediaTypes* p = new CEnumMediaTypes;
    p->m_cRef = 3;
    CHECK(p->Release() == 2);
    CHECK(p->Release() == 1);
    CHECK(g_deleted == 0);
    CHECK(p->Release() == 0);
    CHECK(g_deleted == 1);
}

int main()
{
    run();
    if (g_fail) { std::printf("00ca7c30_TEST FAIL\n"); return 1; }
    std::printf("00ca7c30_TEST PASS\n");
    return 0;
}