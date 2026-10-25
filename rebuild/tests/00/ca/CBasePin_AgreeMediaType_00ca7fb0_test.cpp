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
// CBasePin::AgreeMediaType: a fully specified type is attempted directly; otherwise both pins' enumerators are
// tried (order chosen by m_bTryMyTypesFirst), returning S_OK on success or the last interesting failure.
#define VFW_E_NO_ACCEPTABLE_TYPES HR(0x80040207)
#define VFW_E_TYPE_NOT_ACCEPTED HR(0x8004022A)
#define E_FAIL HR(0x80004005)
#define E_INVALIDARG HR(0x80070057)
struct CMediaType { BOOL partial; BOOL IsPartiallySpecified() const { return partial; } };
struct IEnumMediaTypes { int owner; int releases; IEnumMediaTypes(int o) : owner(o), releases(0) {} ULONG Release() { return ++releases; } };
static IEnumMediaTypes g_peerEnum(1), g_myEnum(2);
struct IPin {
    HRESULT hrEnum;
    IPin() : hrEnum(0) {}
    HRESULT EnumMediaTypes(IEnumMediaTypes** pp) { *pp = &g_peerEnum; return hrEnum; }
};
class CBasePin {
public:
    unsigned char m_bTryMyTypesFirst;
    HRESULT hrMyEnum, hrTryPeer, hrTryMine, hrAttempt;
    int order[4]; int nTries, attempts;
    CBasePin() : m_bTryMyTypesFirst(0), hrMyEnum(0), hrTryPeer(HR(0x80004005)), hrTryMine(HR(0x80004005)), hrAttempt(0), nTries(0), attempts(0) {}
    HRESULT EnumMediaTypes(IEnumMediaTypes** pp) { *pp = &g_myEnum; return hrMyEnum; }
    HRESULT AttemptConnection(IPin*, const CMediaType*) { attempts++; return hrAttempt; }
    HRESULT TryMediaTypes(IPin*, const CMediaType*, IEnumMediaTypes* e) { order[nTries++] = e->owner; return e->owner == 1 ? hrTryPeer : hrTryMine; }
    HRESULT AgreeMediaType(IPin* pReceivePin, const CMediaType* pmt)
    {
        IEnumMediaTypes* pEnumMediaTypes = 0;
        if ((pmt != 0) && (!pmt->IsPartiallySpecified())) {
            return AttemptConnection(pReceivePin, pmt);
        }
        HRESULT hrFailure = VFW_E_NO_ACCEPTABLE_TYPES;
        for (int i = 0; i < 2; i++) {
            HRESULT hr;
            if (i == (int)m_bTryMyTypesFirst) {
                hr = pReceivePin->EnumMediaTypes(&pEnumMediaTypes);
            } else {
                hr = EnumMediaTypes(&pEnumMediaTypes);
            }
            if (hr >= 0) {
                hr = TryMediaTypes(pReceivePin, pmt, pEnumMediaTypes);
                pEnumMediaTypes->Release();
                if (hr >= 0) {
                    return 0;
                } else {
                    if ((hr != E_FAIL) && (hr != E_INVALIDARG) && (hr != VFW_E_TYPE_NOT_ACCEPTED)) {
                        hrFailure = hr;
                    }
                }
            }
        }
        return hrFailure;
    }
};
static void run()
{
    IPin peer;
    { CBasePin p; CMediaType full = { 0 }; p.hrAttempt = HR(0x12345);
      CHECK(p.AgreeMediaType(&peer, &full) == HR(0x12345)); CHECK(p.attempts == 1); CHECK(p.nTries == 0); }
    { CBasePin p; CMediaType part = { 1 };
      CHECK(p.AgreeMediaType(&peer, &part) == VFW_E_NO_ACCEPTABLE_TYPES); CHECK(p.nTries == 2);
      CHECK(p.order[0] == 1); CHECK(p.order[1] == 2); }
    { CBasePin p; p.m_bTryMyTypesFirst = 1; p.hrTryMine = 1;
      CHECK(p.AgreeMediaType(&peer, 0) == 0); CHECK(p.nTries == 1); CHECK(p.order[0] == 2); }
    { CBasePin p; p.hrTryPeer = HR(0x8007000E);
      CHECK(p.AgreeMediaType(&peer, 0) == HR(0x8007000E)); CHECK(p.nTries == 2); }
    { CBasePin p; IPin bad; bad.hrEnum = HR(0x80004001); p.hrMyEnum = HR(0x80004001);
      CHECK(p.AgreeMediaType(&bad, 0) == VFW_E_NO_ACCEPTABLE_TYPES); CHECK(p.nTries == 0); }
    CHECK(g_peerEnum.releases == 2); CHECK(g_myEnum.releases == 3);
}

int main()
{
    run();
    if (g_fail) { std::printf("00ca7fb0_TEST FAIL\n"); return 1; }
    std::printf("00ca7fb0_TEST PASS\n");
    return 0;
}