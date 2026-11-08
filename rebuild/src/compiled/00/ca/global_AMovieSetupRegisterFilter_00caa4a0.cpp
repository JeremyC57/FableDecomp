// _global::AMovieSetupRegisterFilter(const AMOVIESETUP_FILTER* const psetupdata, IFilterMapper* pIFM, BOOL bRegister) -- DirectShow base-class (strmbase dllsetup.cpp) filter self-registration: unregister, then if asked register the filter, every pin and every pin media type through IFilterMapper
// Vendor-proof: strmbase-dxsdk-sum2004(BaseClasses/amfilter.cpp) _AMovieSetupRegisterFilter@12 -- 477 bytes verified against Fable.exe 0x00CAA4A0
// Leaf: _AMovieSetupRegisterFilter@12
// Flags: /EHsc
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00CAA4A0, 477 bytes, STDAPI (`extern "C" HRESULT __stdcall`, `ret 0Ch`).
// Identity: the catalog's "FileArchive_ValidateAndLoad" is a propagated stand-in. The body is the DirectShow SDK
//   baseclasses dllsetup.cpp `AMovieSetupRegisterFilter` text (the non-"2" variant that takes IFilterMapper):
//   S_FALSE on a null setup record; IFilterMapper::UnregisterFilter (slot 7, +0x1C) with *clsID by value; when
//   bRegister: RegisterFilter (slot 3, +0x0C) with clsID/strName/dwMerit, then for every AMOVIESETUP_PIN (0x24 bytes)
//   RegisterPin (slot 5, +0x14) and for every AMOVIESETUP_MEDIATYPE (8 bytes) RegisterPinType (slot 6, +0x18), each
//   loop broken by `if (FAILED(hr)) break;`; finally `0x80070002 == hr ? NOERROR : hr` (compiled branchless:
//   `sete cl ; dec ecx ; and eax,ecx`). Callers: CBaseFilter::Register 0x00CAAEEA and ::Unregister 0x00CAAF5D
//   (the catalog's "CTextureRenderer vfunc_3/4" -- strmbase CBaseFilter, Fable's DirectShow video renderer).
// Layout (DirectShow SDK): AMOVIESETUP_FILTER {clsID +0, strName +4, dwMerit +8, nPins +0xC, lpPin +0x10};
//   AMOVIESETUP_PIN {strName +0, bRendered +4, bOutput +8, bZero +0xC, bMany +0x10, clsConnectsToFilter +0x14,
//   strConnectsToPin +0x18, nMediaTypes +0x1C, lpMediaType +0x20}; AMOVIESETUP_MEDIATYPE {clsMajorType +0, clsMinorType +4}.
// Shape: the DbgLog of the SDK text is compiled out of a retail build. `/EHsc` as on every landed strmbase row.
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

struct __declspec(novtable) IUnknown {
    virtual HRESULT __stdcall QueryInterface(const GUID& riid, void** ppv) = 0;
    virtual unsigned long __stdcall AddRef() = 0;
    virtual unsigned long __stdcall Release() = 0;
};
struct __declspec(novtable) IFilterMapper : public IUnknown {
    virtual HRESULT __stdcall RegisterFilter(CLSID clsid, LPCWSTR Name, DWORD dwMerit) = 0;
    virtual HRESULT __stdcall RegisterFilterInstance(CLSID clsid, LPCWSTR Name, CLSID* MRId) = 0;
    virtual HRESULT __stdcall RegisterPin(CLSID Filter, LPCWSTR Name, BOOL bRendered, BOOL bOutput, BOOL bZero,
                                          BOOL bMany, CLSID ConnectsToFilter, LPCWSTR ConnectsToPin) = 0;
    virtual HRESULT __stdcall RegisterPinType(CLSID clsFilter, LPCWSTR strName, CLSID clsMajorType, CLSID clsSubType) = 0;
    virtual HRESULT __stdcall UnregisterFilter(CLSID Filter) = 0;
    virtual HRESULT __stdcall UnregisterFilterInstance(CLSID MRId) = 0;
    virtual HRESULT __stdcall UnregisterPin(CLSID Filter, LPCWSTR Name) = 0;
    virtual HRESULT __stdcall EnumMatchingFilters(void** ppEnum, DWORD dwMerit, BOOL bInputNeeded, CLSID clsInMaj,
                                                  CLSID clsInSub, BOOL bRender, BOOL bOututNeeded, CLSID clsOutMaj,
                                                  CLSID clsOutSub) = 0;
};

typedef struct _AMOVIESETUP_MEDIATYPE {
    const CLSID* clsMajorType;
    const CLSID* clsMinorType;
} AMOVIESETUP_MEDIATYPE;

typedef struct _AMOVIESETUP_PIN {
    LPCWSTR strName;
    BOOL bRendered;
    BOOL bOutput;
    BOOL bZero;
    BOOL bMany;
    const CLSID* clsConnectsToFilter;
    const WCHAR* strConnectsToPin;
    UINT nMediaTypes;
    const AMOVIESETUP_MEDIATYPE* lpMediaType;
} AMOVIESETUP_PIN;

typedef struct _AMOVIESETUP_FILTER {
    const CLSID* clsID;
    const WCHAR* strName;
    DWORD dwMerit;
    UINT nPins;
    const AMOVIESETUP_PIN* lpPin;
} AMOVIESETUP_FILTER;

#define STDAPI extern "C" HRESULT __stdcall

STDAPI
AMovieSetupRegisterFilter( const AMOVIESETUP_FILTER * const psetupdata
                         , IFilterMapper *                  pIFM
                         , BOOL                             bRegister  )
{
  // check we've got data
  //
  if( 0 == psetupdata ) return S_FALSE;

  // unregister filter
  // (as pins are subkeys of filter's CLSID key
  // they do not need to be removed separately).
  //
  HRESULT hr = pIFM->UnregisterFilter( *(psetupdata->clsID) );

  if( bRegister )
  {
    // register filter
    //
    hr = pIFM->RegisterFilter( *(psetupdata->clsID)
                             , psetupdata->strName
                             , psetupdata->dwMerit );
    if( SUCCEEDED(hr) )
    {
      // all its pins
      //
      for( UINT m1=0; m1 < psetupdata->nPins; m1++ )
      {
        hr = pIFM->RegisterPin( *(psetupdata->clsID)
                              , psetupdata->lpPin[m1].strName
                              , psetupdata->lpPin[m1].bRendered
                              , psetupdata->lpPin[m1].bOutput
                              , psetupdata->lpPin[m1].bZero
                              , psetupdata->lpPin[m1].bMany
                              , *(psetupdata->lpPin[m1].clsConnectsToFilter)
                              , psetupdata->lpPin[m1].strConnectsToPin );

        if( SUCCEEDED(hr) )
        {
          // and each pin's media types
          //
          for( UINT m2=0; m2 < psetupdata->lpPin[m1].nMediaTypes; m2++ )
          {
            hr = pIFM->RegisterPinType( *(psetupdata->clsID)
                                      , psetupdata->lpPin[m1].strName
                                      , *(psetupdata->lpPin[m1].lpMediaType[m2].clsMajorType)
                                      , *(psetupdata->lpPin[m1].lpMediaType[m2].clsMinorType) );
            if( FAILED(hr) ) break;
          }
          if( FAILED(hr) ) break;
        }
        if( FAILED(hr) ) break;
      }
    }
  }

  // handle one acceptable "error" - that
  // of filter not being registered!
  // (couldn't find a suitable #define'd
  // name for the error!)
  //
  if( 0x80070002 == hr)
    return NOERROR;
  else
    return hr;
}