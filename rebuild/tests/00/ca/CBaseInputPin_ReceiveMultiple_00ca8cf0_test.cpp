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
// CBaseInputPin::ReceiveMultiple: Receive each sample in turn, stopping at the first result other than S_OK.
struct IMediaSample { int id; };
class CBaseInputPin {
public:
    int stopAt; int received;
    CBaseInputPin() : stopAt(-1), received(0) {}
    virtual HRESULT Receive(IMediaSample* p) { received++; return p->id == stopAt ? 1 : 0; }
    HRESULT ReceiveMultiple(IMediaSample** pSamples, long nSamples, long* nSamplesProcessed)
    {
        if (pSamples == 0) return HR(0x80004003);
        HRESULT hr = 0;
        *nSamplesProcessed = 0;
        while (nSamples-- > 0) {
            hr = Receive(pSamples[*nSamplesProcessed]);
            if (hr != 0) {
                break;
            }
            (*nSamplesProcessed)++;
        }
        return hr;
    }
};
static void run()
{
    IMediaSample s[4]; IMediaSample* ps[4];
    for (int i = 0; i < 4; i++) { s[i].id = i; ps[i] = &s[i]; }
    long done = -1;
    { CBaseInputPin p; CHECK(p.ReceiveMultiple(ps, 4, &done) == 0); CHECK(done == 4); CHECK(p.received == 4); }
    { CBaseInputPin p; p.stopAt = 2; CHECK(p.ReceiveMultiple(ps, 4, &done) == 1); CHECK(done == 2); CHECK(p.received == 3); }
    { CBaseInputPin p; CHECK(p.ReceiveMultiple(ps, 0, &done) == 0); CHECK(done == 0); }
    { CBaseInputPin p; CHECK(p.ReceiveMultiple(0, 4, &done) == HR(0x80004003)); }
}

int main()
{
    run();
    if (g_fail) { std::printf("00ca8cf0_TEST FAIL\n"); return 1; }
    std::printf("00ca8cf0_TEST PASS\n");
    return 0;
}