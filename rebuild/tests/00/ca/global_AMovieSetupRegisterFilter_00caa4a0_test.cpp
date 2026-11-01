// Test for 0x00CAA4A0 AMovieSetupRegisterFilter: a recording IFilterMapper checks the call sequence and the 0x80070002 mapping.
#include <stdio.h>
#include <string.h>
typedef long HRESULT;
typedef int BOOL;
typedef unsigned long DWORD;
typedef unsigned int UINT;
typedef unsigned short WCHAR;
typedef const WCHAR* LPCWSTR;
struct GUID { unsigned long Data1; unsigned short Data2; unsigned short Data3; unsigned char Data4[8]; };
typedef GUID CLSID;
#define S_FALSE ((HRESULT)1L)
#define NOERROR 0
#define SUCCEEDED(hr) (((HRESULT)(hr)) >= 0)
#define FAILED(hr) (((HRESULT)(hr)) < 0)
struct IUnknown {
    virtual HRESULT QueryInterface(const GUID& riid, void** ppv) = 0;
    virtual unsigned long AddRef() = 0;
    virtual unsigned long Release() = 0;
};
struct IFilterMapper : public IUnknown {
    virtual HRESULT RegisterFilter(CLSID clsid, LPCWSTR Name, DWORD dwMerit) = 0;
    virtual HRESULT RegisterFilterInstance(CLSID clsid, LPCWSTR Name, CLSID* MRId) = 0;
    virtual HRESULT RegisterPin(CLSID Filter, LPCWSTR Name, BOOL bRendered, BOOL bOutput, BOOL bZero,
                                BOOL bMany, CLSID ConnectsToFilter, LPCWSTR ConnectsToPin) = 0;
    virtual HRESULT RegisterPinType(CLSID clsFilter, LPCWSTR strName, CLSID clsMajorType, CLSID clsSubType) = 0;
    virtual HRESULT UnregisterFilter(CLSID Filter) = 0;
};
typedef struct _AMOVIESETUP_MEDIATYPE { const CLSID* clsMajorType; const CLSID* clsMinorType; } AMOVIESETUP_MEDIATYPE;
typedef struct _AMOVIESETUP_PIN {
    LPCWSTR strName; BOOL bRendered; BOOL bOutput; BOOL bZero; BOOL bMany; const CLSID* clsConnectsToFilter;
    const WCHAR* strConnectsToPin; UINT nMediaTypes; const AMOVIESETUP_MEDIATYPE* lpMediaType;
} AMOVIESETUP_PIN;
typedef struct _AMOVIESETUP_FILTER {
    const CLSID* clsID; const WCHAR* strName; DWORD dwMerit; UINT nPins; const AMOVIESETUP_PIN* lpPin;
} AMOVIESETUP_FILTER;

static char g_log[256]; static int g_n;
static HRESULT g_pinTypeResult = 0;
struct Mapper : public IFilterMapper {
    HRESULT QueryInterface(const GUID&, void**) { return 0; }
    unsigned long AddRef() { return 1; }
    unsigned long Release() { return 1; }
    HRESULT RegisterFilter(CLSID c, LPCWSTR, DWORD m) { g_log[g_n++] = 'F'; return (c.Data1 == 0x11 && m == 0x200000) ? 0 : -1; }
    HRESULT RegisterFilterInstance(CLSID, LPCWSTR, CLSID*) { g_log[g_n++] = 'I'; return 0; }
    HRESULT RegisterPin(CLSID, LPCWSTR, BOOL r, BOOL, BOOL, BOOL, CLSID ctf, LPCWSTR) { g_log[g_n++] = 'P'; return (r == 1 && ctf.Data1 == 0x22) ? 0 : -1; }
    HRESULT RegisterPinType(CLSID, LPCWSTR, CLSID maj, CLSID sub) { g_log[g_n++] = 'T'; return (maj.Data1 == 0x33 && sub.Data1 == 0x44) ? g_pinTypeResult : -1; }
    HRESULT UnregisterFilter(CLSID c) { g_log[g_n++] = 'U'; return c.Data1 == 0x11 ? (HRESULT)0x80070002 : -1; }
};

HRESULT AMovieSetupRegisterFilter(const AMOVIESETUP_FILTER* const psetupdata, IFilterMapper* pIFM, BOOL bRegister)
{
  if( 0 == psetupdata ) return S_FALSE;
  HRESULT hr = pIFM->UnregisterFilter( *(psetupdata->clsID) );
  if( bRegister )
  {
    hr = pIFM->RegisterFilter( *(psetupdata->clsID), psetupdata->strName, psetupdata->dwMerit );
    if( SUCCEEDED(hr) )
    {
      for( UINT m1=0; m1 < psetupdata->nPins; m1++ )
      {
        hr = pIFM->RegisterPin( *(psetupdata->clsID), psetupdata->lpPin[m1].strName, psetupdata->lpPin[m1].bRendered,
                                psetupdata->lpPin[m1].bOutput, psetupdata->lpPin[m1].bZero, psetupdata->lpPin[m1].bMany,
                                *(psetupdata->lpPin[m1].clsConnectsToFilter), psetupdata->lpPin[m1].strConnectsToPin );
        if( SUCCEEDED(hr) )
        {
          for( UINT m2=0; m2 < psetupdata->lpPin[m1].nMediaTypes; m2++ )
          {
            hr = pIFM->RegisterPinType( *(psetupdata->clsID), psetupdata->lpPin[m1].strName,
                                        *(psetupdata->lpPin[m1].lpMediaType[m2].clsMajorType),
                                        *(psetupdata->lpPin[m1].lpMediaType[m2].clsMinorType) );
            if( FAILED(hr) ) break;
          }
          if( FAILED(hr) ) break;
        }
        if( FAILED(hr) ) break;
      }
    }
  }
  if( 0x80070002 == hr) return NOERROR; else return hr;
}

int main()
{
    CLSID filt = {0x11}, conn = {0x22}, maj = {0x33}, sub = {0x44};
    AMOVIESETUP_MEDIATYPE mt[2] = { {&maj, &sub}, {&maj, &sub} };
    AMOVIESETUP_PIN pins[2] = { {0, 1, 0, 0, 0, &conn, 0, 2, mt}, {0, 1, 0, 0, 0, &conn, 0, 1, mt} };
    AMOVIESETUP_FILTER f = { &filt, 0, 0x200000, 2, pins };
    Mapper m; int ok = 1;
    if (AMovieSetupRegisterFilter(0, &m, 1) != S_FALSE) ok = 0;
    g_n = 0; if (AMovieSetupRegisterFilter(&f, &m, 0) != 0 || g_n != 1 || g_log[0] != 'U') ok = 0;    // 0x80070002 -> NOERROR
    g_n = 0; if (AMovieSetupRegisterFilter(&f, &m, 1) != 0 || g_n != 7 || memcmp(g_log, "UFPTTPT", 7) != 0) ok = 0;
    g_pinTypeResult = -5; g_n = 0;
    if (AMovieSetupRegisterFilter(&f, &m, 1) != -5 || g_n != 4 || memcmp(g_log, "UFPT", 4) != 0) ok = 0;  // break out of both loops
    printf("00caa4a0_TEST %s\n", ok ? "PASS" : "FAIL");
    return ok ? 0 : 1;
}