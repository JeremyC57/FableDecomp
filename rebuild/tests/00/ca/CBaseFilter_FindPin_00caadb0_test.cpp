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
// CBaseFilter::FindPin: under the filter lock return (AddRef'd) the pin whose name equals Id, else VFW_E_NOT_FOUND.
#define VFW_E_NOT_FOUND HR(0x80040216)
struct CCritSec { int locks, unlocks; CCritSec() : locks(0), unlocks(0) {} void Lock() { locks++; } void Unlock() { unlocks++; } };
class CAutoLock { CCritSec* m_pLock; public: CAutoLock(CCritSec* p) : m_pLock(p) { m_pLock->Lock(); } ~CAutoLock() { m_pLock->Unlock(); } };
static int lstrcmpW(const WCHAR* a, const WCHAR* b) { while (*a && *a == *b) { a++; b++; } return (int)*a - (int)*b; }
struct IPin { int refs; IPin() : refs(0) {} ULONG AddRef() { return ++refs; } };
struct CBasePin : IPin { const WCHAR* m_pName; const WCHAR* Name() { return m_pName; } };
class CBaseFilter {
public:
    CCritSec* m_pLock; CBasePin* pins[4]; int n;
    int GetPinCount() { return n; }
    CBasePin* GetPin(int i) { return pins[i]; }
    HRESULT FindPin(const WCHAR* Id, IPin** ppPin)
    {
        if (ppPin == 0) return HR(0x80004003);
        CAutoLock lck(m_pLock);
        int iCount = GetPinCount();
        for (int i = 0; i < iCount; i++) {
            CBasePin* pPin = GetPin(i);
            if (0 == lstrcmpW(pPin->Name(), Id)) {
                *ppPin = pPin;
                pPin->AddRef();
                return 0;
            }
        }
        *ppPin = 0;
        return VFW_E_NOT_FOUND;
    }
};
static void run()
{
    static const WCHAR nIn[] = { 'I', 'n', 0 }, nOut[] = { 'O', 'u', 't', 0 }, nX[] = { 'X', 0 };
    CCritSec cs; CBasePin a, b; a.m_pName = nIn; b.m_pName = nOut;
    CBaseFilter f; f.m_pLock = &cs; f.pins[0] = &a; f.pins[1] = &b; f.n = 2;
    IPin* p = 0;
    CHECK(f.FindPin(nOut, &p) == 0); CHECK(p == &b); CHECK(b.refs == 1); CHECK(a.refs == 0);
    CHECK(f.FindPin(nX, &p) == VFW_E_NOT_FOUND); CHECK(p == 0);
    CHECK(f.FindPin(nIn, 0) == HR(0x80004003));
    CHECK(cs.locks == 2 && cs.unlocks == 2);
}

int main()
{
    run();
    if (g_fail) { std::printf("00caadb0_TEST FAIL\n"); return 1; }
    std::printf("00caadb0_TEST PASS\n");
    return 0;
}