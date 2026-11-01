// Behaviour test for CDispParams::CDispParams 0x00CAE9B0 (standalone copy of the model and logic).
#include <cstdio>
#include <cstring>
#include <new>
typedef int HRESULT;
typedef unsigned short WORD;
typedef unsigned short OLECHAR;
typedef OLECHAR* BSTR;
static int g_addref = 0;
struct IUnknown { virtual unsigned int AddRef() { return ++g_addref; } };
enum { VT_I2 = 2, VT_I4 = 3, VT_R4 = 4, VT_R8 = 5, VT_CY = 6, VT_DATE = 7, VT_BSTR = 8, VT_DISPATCH = 9,
       VT_ERROR = 10, VT_BOOL = 11, VT_UNKNOWN = 13, VT_UI1 = 17 };
struct VARIANT {
    unsigned short vt, r1, r2, r3;
    union { int lVal; unsigned char bVal; short iVal; float fltVal; double dblVal; short boolVal; int scode;
            long long cyVal; double date; BSTR bstrVal; IUnknown* punkVal; IUnknown* pdispVal; };
};
struct CDispParams {
    VARIANT* rgvarg; int* rgdispidNamedArgs; unsigned int cArgs; unsigned int cNamedArgs;
    CDispParams(unsigned int nArgs, VARIANT* pArgs, HRESULT* phr);
};
static bool g_failAlloc = false;
template <class T> static T* NewArr(unsigned int n) { return g_failAlloc ? 0 : new (std::nothrow) T[n]; }
CDispParams::CDispParams(unsigned int nArgs, VARIANT* pArgs, HRESULT* phr)
{
    cNamedArgs = 0; rgdispidNamedArgs = 0; cArgs = nArgs;
    if (cArgs) {
        rgvarg = NewArr<VARIANT>(cArgs);
        if (0 == rgvarg) { cArgs = 0; if (phr) *phr = (HRESULT)0x8007000E; return; }
        for (unsigned int i = 0; i < cArgs; i++) {
            VARIANT* pDest = &rgvarg[i]; VARIANT* pSrc = &pArgs[i];
            pDest->vt = pSrc->vt;
            switch (pDest->vt) {
            case VT_I4: pDest->lVal = pSrc->lVal; break;
            case VT_UI1: pDest->bVal = pSrc->bVal; break;
            case VT_I2: pDest->iVal = pSrc->iVal; break;
            case VT_R4: pDest->fltVal = pSrc->fltVal; break;
            case VT_R8: pDest->dblVal = pSrc->dblVal; break;
            case VT_BOOL: pDest->boolVal = pSrc->boolVal; break;
            case VT_ERROR: pDest->scode = pSrc->scode; break;
            case VT_CY: pDest->cyVal = pSrc->cyVal; break;
            case VT_DATE: pDest->date = pSrc->date; break;
            case VT_BSTR:
                if (pSrc->bstrVal == 0) { pDest->bstrVal = 0; }
                else {
                    WORD len = *(WORD*)(pSrc->bstrVal - 1);
                    OLECHAR* pch = NewArr<OLECHAR>(len + 1);
                    if (pch) { *(WORD*)pch = len; pDest->bstrVal = pch + 1; memcpy(pDest->bstrVal, pSrc->bstrVal, len * sizeof(OLECHAR)); }
                    else { cArgs = i; if (phr) *phr = (HRESULT)0x8007000E; }
                }
                pDest->bstrVal = pSrc->bstrVal;   // SDK quirk: the copy is overwritten by the source pointer
                break;
            case VT_UNKNOWN: pDest->punkVal = pSrc->punkVal; pDest->punkVal->AddRef(); break;
            case VT_DISPATCH: pDest->pdispVal = pSrc->pdispVal; pDest->pdispVal->AddRef(); break;
            default: break;
            }
        }
    } else {
        rgvarg = 0;
    }
}
int main()
{
    int ok = 1; HRESULT hr = 0; IUnknown unk;
    OLECHAR bs[4] = { 2, 'h', 'i', 0 };
    VARIANT a[6]; memset(a, 0, sizeof(a));
    a[0].vt = VT_I4; a[0].lVal = -5; a[1].vt = VT_R8; a[1].dblVal = 2.5; a[2].vt = VT_BSTR; a[2].bstrVal = bs + 1;
    a[3].vt = VT_UNKNOWN; a[3].punkVal = &unk; a[4].vt = VT_UI1; a[4].bVal = 7; a[5].vt = VT_CY; a[5].cyVal = 123456789012LL;
    CDispParams p(6, a, &hr);
    ok &= hr == 0 && p.cArgs == 6 && p.cNamedArgs == 0 && p.rgdispidNamedArgs == 0 && p.rgvarg != 0;
    ok &= p.rgvarg[0].lVal == -5 && p.rgvarg[1].dblVal == 2.5 && p.rgvarg[2].bstrVal == bs + 1 && p.rgvarg[3].punkVal == &unk;
    ok &= g_addref == 1 && p.rgvarg[4].bVal == 7 && p.rgvarg[5].cyVal == 123456789012LL && p.rgvarg[2].vt == VT_BSTR;
    CDispParams e(0, 0, &hr); ok &= e.rgvarg == 0 && e.cArgs == 0;
    g_failAlloc = true; hr = 0; CDispParams f(3, a, &hr); ok &= hr == (HRESULT)0x8007000E && f.cArgs == 0;
    CDispParams g(3, a, 0); ok &= g.cArgs == 0;
    g_failAlloc = false;
    printf(ok ? "00cae9b0_TEST PASS\n" : "00cae9b0_TEST FAIL\n");
    return ok ? 0 : 1;
}