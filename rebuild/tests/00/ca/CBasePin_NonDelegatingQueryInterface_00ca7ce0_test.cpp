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
// CBasePin::NonDelegatingQueryInterface: IPin / IQualityControl handed out AddRef'd, else CUnknown's answer.
struct IUnknown { virtual HRESULT QueryInterface(const IID&, void**) = 0; virtual ULONG AddRef() = 0; virtual ULONG Release() = 0; };
struct INonDelegatingUnknown { virtual HRESULT NonDelegatingQueryInterface(const IID&, void**) = 0; virtual ULONG NonDelegatingAddRef() = 0; virtual ULONG NonDelegatingRelease() = 0; };
static const IID IID_IUnknown = { 0x00000000, 0x0000, 0x0000, { 0xC0, 0, 0, 0, 0, 0, 0, 0x46 } };
static const IID IID_IPin = { 0x56A86891, 0x0AD4, 0x11CE, { 0xB0, 0x3A, 0x00, 0x20, 0xAF, 0x0B, 0xA7, 0x70 } };
static const IID IID_IQualityControl = { 0x56A868A5, 0x0AD4, 0x11CE, { 0xB0, 0x3A, 0x00, 0x20, 0xAF, 0x0B, 0xA7, 0x70 } };
static const IID IID_Other = { 0x12345678, 0x1111, 0x2222, { 1, 2, 3, 4, 5, 6, 7, 8 } };
static HRESULT GetInterface(IUnknown* pUnk, void** ppv)
{
    if (ppv == 0) return HR(0x80004003);
    *ppv = pUnk;
    pUnk->AddRef();
    return 0;
}
class CUnknown : public INonDelegatingUnknown {
public:
    LONG m_cRef;
    CUnknown() : m_cRef(0) {}
    HRESULT NonDelegatingQueryInterface(const IID& riid, void** ppv)
    {
        if (ppv == 0) return HR(0x80004003);
        if (riid == IID_IUnknown) { GetInterface((IUnknown*)(INonDelegatingUnknown*)this, ppv); return 0; }
        *ppv = 0;
        return HR(0x80004002);
    }
    ULONG NonDelegatingAddRef() { return ++m_cRef; }
    ULONG NonDelegatingRelease() { return --m_cRef; }
};
struct IPin : IUnknown {};
struct IQualityControl : IUnknown {};
class CBasePin : public CUnknown, public IPin, public IQualityControl {
public:
    HRESULT QueryInterface(const IID& riid, void** ppv) { return NonDelegatingQueryInterface(riid, ppv); }
    ULONG AddRef() { return NonDelegatingAddRef(); }
    ULONG Release() { return NonDelegatingRelease(); }
    HRESULT NonDelegatingQueryInterface(const IID& riid, void** ppv)
    {
        if (riid == IID_IPin) {
            return GetInterface((IPin*)this, ppv);
        } else if (riid == IID_IQualityControl) {
            return GetInterface((IQualityControl*)this, ppv);
        } else {
            return CUnknown::NonDelegatingQueryInterface(riid, ppv);
        }
    }
};
static void run()
{
    CBasePin pin;
    void* pv = 0;
    CHECK(pin.NonDelegatingQueryInterface(IID_IPin, &pv) == 0);
    CHECK(pv == (void*)(IPin*)&pin);
    CHECK(pin.m_cRef == 1);
    CHECK(pin.NonDelegatingQueryInterface(IID_IQualityControl, &pv) == 0);
    CHECK(pv == (void*)(IQualityControl*)&pin);
    CHECK(pin.m_cRef == 2);
    CHECK(pin.NonDelegatingQueryInterface(IID_IUnknown, &pv) == 0);
    CHECK(pv == (void*)(INonDelegatingUnknown*)&pin);
    CHECK(pin.m_cRef == 3);
    pv = &pin;
    CHECK(pin.NonDelegatingQueryInterface(IID_Other, &pv) == HR(0x80004002));
    CHECK(pv == 0);
    CHECK(pin.NonDelegatingQueryInterface(IID_IPin, 0) == HR(0x80004003));
    CHECK(pin.m_cRef == 3);
}

int main()
{
    run();
    if (g_fail) { std::printf("00ca7ce0_TEST FAIL\n"); return 1; }
    std::printf("00ca7ce0_TEST PASS\n");
    return 0;
}