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
// CBasePin::TryMediaTypes: walk an enumerator, attempt every type that matches the partial type, free each
// enumerated type; S_OK on the first success, else the first "interesting" failure or VFW_E_NO_ACCEPTABLE_TYPES.
#define VFW_E_NO_ACCEPTABLE_TYPES HR(0x80040207)
#define VFW_E_TYPE_NOT_ACCEPTED HR(0x8004022A)
#define E_FAIL HR(0x80004005)
#define E_INVALIDARG HR(0x80070057)
struct CMediaType {
    int id;
    BOOL MatchesPartial(const CMediaType* p) const { return p->id == 0 || p->id == id; }
};
typedef CMediaType AM_MEDIA_TYPE;
static int g_deletes = 0;
static void DeleteMediaType(AM_MEDIA_TYPE* p) { g_deletes++; delete p; }
struct IEnumMediaTypes {
    int ids[8]; int n, pos; HRESULT hrReset;
    IEnumMediaTypes() : n(0), pos(0), hrReset(0) {}
    HRESULT Reset() { pos = 0; return hrReset; }
    HRESULT Next(ULONG, AM_MEDIA_TYPE** pp, ULONG* pc)
    {
        if (pos >= n) { *pc = 0; return 1; }
        CMediaType* p = new CMediaType; p->id = ids[pos++]; *pp = p; *pc = 1; return 0;
    }
};
struct IPin {};
class CBasePin {
public:
    HRESULT results[16]; int attempts[16]; int nAttempts;
    CBasePin() : nAttempts(0) { for (int i = 0; i < 16; i++) results[i] = HR(0x80004005); }
    HRESULT AttemptConnection(IPin*, const CMediaType* pmt) { attempts[nAttempts++] = pmt->id; return results[pmt->id]; }
    HRESULT TryMediaTypes(IPin* pReceivePin, const CMediaType* pmt, IEnumMediaTypes* pEnum)
    {
        HRESULT hr = pEnum->Reset();
        if (hr < 0) {
            return hr;
        }
        CMediaType* pMediaType = 0;
        ULONG ulMediaCount = 0;
        HRESULT hrFailure = 0;
        for (;;) {
            hr = pEnum->Next(1, (AM_MEDIA_TYPE**)&pMediaType, &ulMediaCount);
            if (hr != 0) {
                if (0 == hrFailure) {
                    hrFailure = VFW_E_NO_ACCEPTABLE_TYPES;
                }
                return hrFailure;
            }
            if ((pmt == 0) || pMediaType->MatchesPartial(pmt)) {
                hr = AttemptConnection(pReceivePin, pMediaType);
                if ((hr < 0) && (hrFailure >= 0) && (hr != E_FAIL) && (hr != E_INVALIDARG) && (hr != VFW_E_TYPE_NOT_ACCEPTED)) {
                    hrFailure = hr;
                }
            } else {
                hr = VFW_E_NO_ACCEPTABLE_TYPES;
            }
            DeleteMediaType(pMediaType);
            if (0 == hr) {
                return hr;
            }
        }
    }
};
static void run()
{
    IPin peer;
    { CBasePin p; IEnumMediaTypes e; e.n = 3; e.ids[0] = 1; e.ids[1] = 2; e.ids[2] = 3; p.results[2] = 0; g_deletes = 0;
      CHECK(p.TryMediaTypes(&peer, 0, &e) == 0); CHECK(p.nAttempts == 2); CHECK(g_deletes == 2); }
    { CBasePin p; IEnumMediaTypes e; e.n = 3; e.ids[0] = 1; e.ids[1] = 2; e.ids[2] = 3; p.results[3] = 0;
      CMediaType partial = { 3 }; g_deletes = 0;
      CHECK(p.TryMediaTypes(&peer, &partial, &e) == 0); CHECK(p.nAttempts == 1); CHECK(p.attempts[0] == 3); CHECK(g_deletes == 3); }
    { CBasePin p; IEnumMediaTypes e; e.n = 2; e.ids[0] = 1; e.ids[1] = 2; p.results[1] = HR(0x8007000E); p.results[2] = HR(0x80040200);
      CHECK(p.TryMediaTypes(&peer, 0, &e) == HR(0x8007000E)); }
    { CBasePin p; IEnumMediaTypes e; e.n = 2; e.ids[0] = 1; e.ids[1] = 2;
      CHECK(p.TryMediaTypes(&peer, 0, &e) == VFW_E_NO_ACCEPTABLE_TYPES); }
    { CBasePin p; IEnumMediaTypes e;
      CHECK(p.TryMediaTypes(&peer, 0, &e) == VFW_E_NO_ACCEPTABLE_TYPES); CHECK(p.nAttempts == 0); }
    { CBasePin p; IEnumMediaTypes e; e.hrReset = HR(0x80040203);
      CHECK(p.TryMediaTypes(&peer, 0, &e) == HR(0x80040203)); }
}

int main()
{
    run();
    if (g_fail) { std::printf("00ca7e90_TEST FAIL\n"); return 1; }
    std::printf("00ca7e90_TEST PASS\n");
    return 0;
}