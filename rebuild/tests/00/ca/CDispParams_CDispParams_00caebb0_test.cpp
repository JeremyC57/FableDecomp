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
// CDispParams::~CDispParams: per argument free BSTR blocks (allocated with a one-OLECHAR length prefix), Release
// IUnknown / IDispatch arguments, ignore the rest; finally free the VARIANT array.
typedef unsigned int UINT; typedef WCHAR OLECHAR; typedef OLECHAR* BSTR; typedef unsigned short WORD; typedef long DISPID;
static void* g_freed[8]; static int g_nFreed = 0;
static void test_delete(void* p) { if (p) { g_freed[g_nFreed++] = p; ::operator delete(p); } }
struct IUnknown { int releases; IUnknown() : releases(0) {} virtual ULONG Release() { return ++releases; } };
struct IDispatch : IUnknown {};
enum { VT_EMPTY = 0, VT_I4 = 3, VT_BSTR = 8, VT_DISPATCH = 9, VT_UNKNOWN = 13 };
struct VARIANT { unsigned short vt; WORD r1, r2, r3; union { LONG lVal; BSTR bstrVal; IUnknown* punkVal; IDispatch* pdispVal; }; };
struct DISPPARAMS { VARIANT* rgvarg; DISPID* rgdispidNamedArgs; UINT cArgs; UINT cNamedArgs; };
class CDispParams : public DISPPARAMS {
public:
    ~CDispParams()
    {
        for (UINT i = 0; i < cArgs; i++) {
            switch(rgvarg[i].vt) {
            case VT_BSTR:
                if ((void*)rgvarg[i].bstrVal != 0) {
                    OLECHAR * pch = rgvarg[i].bstrVal - (sizeof(WORD)/sizeof(OLECHAR));
                    test_delete(pch);
                }
                break;
            case VT_UNKNOWN:
                rgvarg[i].punkVal->Release();
                break;
            case VT_DISPATCH:
                rgvarg[i].pdispVal->Release();
                break;
            }
        }
        test_delete(rgvarg);
    }
};
static void run()
{
    IUnknown unk; IDispatch disp;
    OLECHAR* block = (OLECHAR*)::operator new(8 * sizeof(OLECHAR));
    VARIANT* v = (VARIANT*)::operator new(5 * sizeof(VARIANT));
    v[0].vt = VT_BSTR; v[0].bstrVal = block + 1;
    v[1].vt = VT_UNKNOWN; v[1].punkVal = &unk;
    v[2].vt = VT_DISPATCH; v[2].pdispVal = &disp;
    v[3].vt = VT_BSTR; v[3].bstrVal = 0;
    v[4].vt = VT_I4; v[4].lVal = 5;
    {
        CDispParams* dp = (CDispParams*)::operator new(sizeof(CDispParams));
        dp->rgvarg = v; dp->rgdispidNamedArgs = 0; dp->cArgs = 5; dp->cNamedArgs = 0;
        dp->~CDispParams();
        ::operator delete(dp);
    }
    CHECK(g_nFreed == 2); CHECK(g_freed[0] == (void*)block); CHECK(g_freed[1] == (void*)v);
    CHECK(unk.releases == 1); CHECK(disp.releases == 1);
}

int main()
{
    run();
    if (g_fail) { std::printf("00caebb0_TEST FAIL\n"); return 1; }
    std::printf("00caebb0_TEST PASS\n");
    return 0;
}