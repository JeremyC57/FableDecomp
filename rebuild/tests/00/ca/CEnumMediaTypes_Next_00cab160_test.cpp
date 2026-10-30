// Behaviour test for CEnumMediaTypes::Next (0x00CAB160): E_POINTER for a null array, VFW_E_ENUM_OUT_OF_SYNC when the pin's
// media-type version moved, E_INVALIDARG for more than one type without a fetched-count pointer; otherwise each type the
// pin returns is handed over in a task-allocated AM_MEDIA_TYPE (the format block moved, not copied, so the local
// CMediaType does not free it), stopping at the first failure; S_OK when all were fetched, else S_FALSE.
#include <cstdio>
#include <cstdlib>
#include <cstring>
typedef int HRESULT;
typedef unsigned int ULONG;
typedef int LONG;
typedef int BOOL;
typedef unsigned char BYTE;
#define NULL 0
#define TRUE 1
#define NOERROR 0
#define S_OK 0
#define S_FALSE 1
#define E_POINTER ((HRESULT)0x80004003L)
#define E_INVALIDARG ((HRESULT)0x80070057L)
#define VFW_E_ENUM_OUT_OF_SYNC ((HRESULT)0x80040203L)
#define CheckPointer(p, ret) { if ((p) == NULL) return (ret); }
struct GUID { unsigned int Data1; unsigned short Data2; unsigned short Data3; unsigned char Data4[8]; };
struct IUnknown;
struct AM_MEDIA_TYPE {
    GUID majortype; GUID subtype; BOOL bFixedSizeSamples; BOOL bTemporalCompression; ULONG lSampleSize;
    GUID formattype; IUnknown* pUnk; ULONG cbFormat; BYTE* pbFormat;
};
static int g_formatFrees = 0, g_allocs = 0, g_failAllocAt = -1;
static void* CoTaskMemAlloc(ULONG cb) { if (g_allocs++ == g_failAllocAt) return 0; return std::malloc(cb); }
class CMediaType : public AM_MEDIA_TYPE {
public:
    CMediaType() { std::memset((AM_MEDIA_TYPE*)this, 0, sizeof(AM_MEDIA_TYPE)); }
    ~CMediaType() { if (pbFormat) { ++g_formatFrees; std::free(pbFormat); } }
};
struct CBasePin {
    LONG ver; int nTypes;
    LONG GetMediaTypeVersion() { return ver; }
    HRESULT GetMediaType(int iPosition, CMediaType* pmt)
    {
        if (iPosition >= nTypes) return 0x00040103;   // VFW_S_NO_MORE_ITEMS
        pmt->lSampleSize = 100 + iPosition;
        pmt->cbFormat = 4; pmt->pbFormat = (BYTE*)std::malloc(4); pmt->pbFormat[0] = (BYTE)iPosition;
        return S_OK;
    }
};
class CEnumMediaTypes {
public:
    int m_Position; CBasePin* m_pPin; LONG m_Version;
    BOOL AreWeOutOfSync() { return (m_pPin->GetMediaTypeVersion() == m_Version ? 0 : 1); }
    HRESULT Next(ULONG cMediaTypes, AM_MEDIA_TYPE** ppMediaTypes, ULONG* pcFetched)
    {
        CheckPointer(ppMediaTypes,E_POINTER);
        if (AreWeOutOfSync() == TRUE) {
            return VFW_E_ENUM_OUT_OF_SYNC;
        }
        if (pcFetched!=NULL) {
            *pcFetched = 0;
        }
        else if (cMediaTypes>1) {
            return E_INVALIDARG;
        }
        ULONG cFetched = 0;
        while (cMediaTypes) {
            CMediaType cmt;
            HRESULT hr = m_pPin->GetMediaType(m_Position++, &cmt);
            if (S_OK != hr) {
                break;
            }
            *ppMediaTypes = (AM_MEDIA_TYPE *)CoTaskMemAlloc(sizeof(AM_MEDIA_TYPE));
            if (*ppMediaTypes == NULL) {
                break;
            }
            **ppMediaTypes = cmt;
            cmt.pbFormat = NULL;
            cmt.cbFormat = NULL;
            cmt.pUnk     = NULL;
            ppMediaTypes++;
            cFetched++;
            cMediaTypes--;
        }
        if (pcFetched!=NULL) {
            *pcFetched = cFetched;
        }
        return ( cMediaTypes==0 ? NOERROR : S_FALSE );
    }
};
static int fail(const char* why) { std::printf("00cab160_TEST FAIL: %s\n", why); return 1; }
int main()
{
    CBasePin pin; pin.ver = 1; pin.nTypes = 3;
    CEnumMediaTypes e; e.m_pPin = &pin; e.m_Version = 1; e.m_Position = 0;
    AM_MEDIA_TYPE* out[4]; ULONG fetched = 99;
    if (e.Next(1, 0, &fetched) != E_POINTER || fetched != 99) return fail("null array");
    pin.ver = 2;
    if (e.Next(1, out, &fetched) != VFW_E_ENUM_OUT_OF_SYNC) return fail("out of sync");
    pin.ver = 1;
    if (e.Next(2, out, 0) != E_INVALIDARG || e.m_Position != 0) return fail("count without pcFetched");
    // two of three: S_OK, the format blocks handed over, not freed
    if (e.Next(2, out, &fetched) != S_OK || fetched != 2 || e.m_Position != 2) return fail("fetch two");
    if (out[0]->lSampleSize != 100 || out[1]->lSampleSize != 101 || out[1]->pbFormat[0] != 1 || out[1]->cbFormat != 4 || g_formatFrees) return fail("hand-over");
    // two more of the one left: S_FALSE, one fetched, the failed position consumed
    if (e.Next(2, out + 2, &fetched) != S_FALSE || fetched != 1 || out[2]->lSampleSize != 102 || e.m_Position != 4) return fail("past the end");
    // a single type with no pcFetched is allowed
    e.m_Position = 0;
    if (e.Next(1, out + 3, 0) != S_OK || out[3]->lSampleSize != 100) return fail("single without pcFetched");
    // allocation failure: the local CMediaType frees its own format block, S_FALSE
    g_failAllocAt = g_allocs; e.m_Position = 0;
    AM_MEDIA_TYPE* spare = 0;
    if (e.Next(1, &spare, &fetched) != S_FALSE || fetched != 0 || g_formatFrees != 1) return fail("allocation failure");
    for (int i = 0; i < 4; ++i) { std::free(out[i]->pbFormat); std::free(out[i]); }
    std::printf("00cab160_TEST PASS\n");
    return 0;
}