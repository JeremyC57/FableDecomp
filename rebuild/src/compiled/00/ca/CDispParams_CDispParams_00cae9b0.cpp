// CDispParams::CDispParams(UINT nArgs, VARIANT* pArgs, HRESULT* phr) -- DirectShow strmbase ctlutil.cpp constructor (DX9 SDK text)
// Vendor-proof: strmbase-dxsdk-sum2004(BaseClasses/ctlutil.cpp) ??0CDispParams@@QAE@IPAUtagVARIANT@@PAJ@Z -- 504 bytes verified against Fable.exe 0x00CAE9B0
// Leaf: ??0CDispParams@@QAE@IPAUtagVARIANT@@PAJ@Z
// Identity: ret 0Ch with ecx=this; stores cNamedArgs(+0xC)=0, rgdispidNamedArgs(+4)=0, cArgs(+8)=nArgs; new(nArgs<<4) via
//   operator new 0xBFEA1A; E_OUTOFMEMORY 0x8007000E; vt-2 jump table of 16 (VT_I2..VT_UI1); BSTR length-word copy; AddRef
//   (vtable +4) for VT_UNKNOWN/VT_DISPATCH. Called from CDeferredCommand ctor 0xCAF230 (m_DispParams at +0x34).
//   Catalog label Variant_CopyVariantArray is wrong.
typedef long HRESULT;
typedef unsigned long ULONG;
typedef long LONG;
typedef unsigned int UINT;
typedef unsigned short WORD;
typedef unsigned char BYTE;
typedef short SHORT;
typedef float FLOAT;
typedef double DOUBLE;
typedef double DATE;
typedef void* PVOID;
typedef unsigned short WCHAR;
typedef WCHAR OLECHAR;
typedef OLECHAR* BSTR;
typedef long SCODE;
typedef short VARIANT_BOOL;
typedef unsigned short VARTYPE;
typedef long DISPID;
typedef __int64 LONGLONG;
typedef struct _GUID { unsigned long Data1; unsigned short Data2; unsigned short Data3; unsigned char Data4[8]; } GUID;
typedef GUID IID;
struct __declspec(novtable) IUnknown {
    virtual HRESULT __stdcall QueryInterface(const IID& riid, void** ppv) = 0;   // 0
    virtual ULONG __stdcall AddRef() = 0;                                        // 1
    virtual ULONG __stdcall Release() = 0;                                       // 2
};
struct __declspec(novtable) IDispatch : IUnknown {};
typedef union tagCY { struct { unsigned long Lo; long Hi; }; LONGLONG int64; } CY;   // wtypes.h
enum VARENUM { VT_EMPTY = 0, VT_NULL = 1, VT_I2 = 2, VT_I4 = 3, VT_R4 = 4, VT_R8 = 5, VT_CY = 6, VT_DATE = 7,
               VT_BSTR = 8, VT_DISPATCH = 9, VT_ERROR = 10, VT_BOOL = 11, VT_VARIANT = 12, VT_UNKNOWN = 13,
               VT_DECIMAL = 14, VT_I1 = 16, VT_UI1 = 17 };
struct tagVARIANT {                               // oaidl.h, 16 bytes (typedef tagVARIANT VARIANT)
    VARTYPE vt;                                   // +0x00
    WORD wReserved1, wReserved2, wReserved3;
    union {                                       // +0x08
        LONG lVal;
        BYTE bVal;
        SHORT iVal;
        FLOAT fltVal;
        DOUBLE dblVal;
        VARIANT_BOOL boolVal;
        SCODE scode;
        CY cyVal;
        DATE date;
        BSTR bstrVal;
        IUnknown* punkVal;
        IDispatch* pdispVal;
    };
};
typedef tagVARIANT VARIANT;
struct tagDISPPARAMS {                            // oaidl.h (typedef tagDISPPARAMS DISPPARAMS)
    VARIANT* rgvarg;                              // +0x00
    DISPID* rgdispidNamedArgs;                    // +0x04
    UINT cArgs;                                   // +0x08
    UINT cNamedArgs;                              // +0x0C
};
typedef tagDISPPARAMS DISPPARAMS;
extern "C" void* __cdecl memcpy(void* dst, const void* src, unsigned int n);
#pragma intrinsic(memcpy)
#define CopyMemory(Destination, Source, Length) memcpy((Destination), (Source), (Length))   // winbase.h
#define E_OUTOFMEMORY ((HRESULT)0x8007000EL)
#define ASSERT(x)                                 // wxdebug.h, release build
void* __cdecl operator new(unsigned int size);    // 0x00BFEA1A (no operator new[] in scope: VC7.1 calls scalar new)

class CDispParams : public DISPPARAMS {           // ctlutil.h
public:
    CDispParams(UINT nArgs, VARIANT* pArgs, HRESULT* phr = 0);
    ~CDispParams();
};

CDispParams::CDispParams(UINT nArgs, VARIANT* pArgs, HRESULT *phr)
{
   cNamedArgs = 0;
   rgdispidNamedArgs = 0;
   cArgs = nArgs;

    if (cArgs) {
        rgvarg = new VARIANT[cArgs];
        if (0 == rgvarg) {
            cArgs = 0;
            if (phr) {
                *phr = E_OUTOFMEMORY;
            }
            return;
        }

        for (UINT i = 0; i < cArgs; i++) {

            //  Why aren't we using VariantCopy?

            VARIANT * pDest = &rgvarg[i];
            VARIANT * pSrc = &pArgs[i];

            pDest->vt = pSrc->vt;
            switch(pDest->vt) {

            case VT_I4:
                pDest->lVal = pSrc->lVal;
                break;

            case VT_UI1:
                pDest->bVal = pSrc->bVal;
                break;

            case VT_I2:
                pDest->iVal = pSrc->iVal;
                break;

            case VT_R4:
                pDest->fltVal = pSrc->fltVal;
                break;

            case VT_R8:
                pDest->dblVal = pSrc->dblVal;
                break;

            case VT_BOOL:
                pDest->boolVal = pSrc->boolVal;
                break;

            case VT_ERROR:
                pDest->scode = pSrc->scode;
                break;

            case VT_CY:
                pDest->cyVal = pSrc->cyVal;
                break;

            case VT_DATE:
                pDest->date = pSrc->date;
                break;

            case VT_BSTR:
                if ((PVOID)pSrc->bstrVal == 0) {
                    pDest->bstrVal = 0;
                } else {

                    // a BSTR is a WORD followed by a UNICODE string.
                    // the pointer points just after the WORD

                    WORD len = * (WORD*) (pSrc->bstrVal - (sizeof(WORD) / sizeof(OLECHAR)));
                    OLECHAR* pch = new OLECHAR[len + (sizeof(WORD)/sizeof(OLECHAR))];
                    if (pch) {
                        WORD *pui = (WORD*)pch;
                        *pui = len;
                        pDest->bstrVal = pch + (sizeof(WORD)/sizeof(OLECHAR));
                        CopyMemory(pDest->bstrVal, pSrc->bstrVal, len*sizeof(OLECHAR));
                    } else {
                        cArgs = i;
                        if (phr) {
                            *phr = E_OUTOFMEMORY;
                        }
                    }
                }
                pDest->bstrVal = pSrc->bstrVal;
                break;

            case VT_UNKNOWN:
                pDest->punkVal = pSrc->punkVal;
                pDest->punkVal->AddRef();
                break;

            case VT_DISPATCH:
                pDest->pdispVal = pSrc->pdispVal;
                pDest->pdispVal->AddRef();
                break;

            default:
                // a type we haven't got round to adding yet!
                ASSERT(0);
                break;
            }
        }

    } else {
        rgvarg = 0;
    }

}