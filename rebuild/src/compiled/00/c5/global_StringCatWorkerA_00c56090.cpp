// StringCatWorkerA(char* pszDest, unsigned int cchDest, const char* pszSrc) -- strsafe.h worker: measure pszDest within cchDest (inline StringLengthWorkerA) and append pszSrc into the remaining space through StringCopyWorkerA
// Leaf: _StringCatWorkerA@12
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C56090, length 84 bytes (+ int3 padding); `ret 0Ch` (extern "C" __stdcall).
// Callees: StringCopyWorkerA 0x00C55750. Callers: the OpenAL wrapper's string builders 0x00C56587 (tail jump) / 0x00C56671 /
// 0x00C566A3 / 0x00C5673A / 0x00C56ACF / 0x00C56B01 / 0x00C56B95 (inlined StringCchCatA calls with constant sizes, so the
// STRSAFE_MAX_CCH test folded away there and the worker is called directly).
// Shape: the Platform SDK strsafe.h bodies verbatim; StringLengthWorkerA is inlined (its `pcch` local lives in the dead
// cchDest argument slot, re-read on the failure paths), StringCopyWorkerA is not. The catalog label
// "StringCopy_ValidateLength" is descriptive; the strsafe name follows its callee's decorated name `_StringCopyWorkerA@12`.
#define STRSAFE_E_INVALID_PARAMETER   ((long)0x80070057L)

extern "C" long __stdcall StringCopyWorkerA(char* pszDest, unsigned int cchDest, const char* pszSrc);   // 0x00C55750

extern "C" __inline long __stdcall StringLengthWorkerA(const char* psz, unsigned int cchMax, unsigned int* pcch)
{
    long hr = 0;
    unsigned int cchMaxPrev = cchMax;

    while (cchMax && (*psz != '\0'))
    {
        psz++;
        cchMax--;
    }

    if (cchMax == 0)
    {
        // the string is longer than cchMax
        hr = STRSAFE_E_INVALID_PARAMETER;
    }

    if (hr >= 0 && pcch)
    {
        *pcch = cchMaxPrev - cchMax;
    }

    return hr;
}

extern "C" long __stdcall StringCatWorkerA(char* pszDest, unsigned int cchDest, const char* pszSrc)
{
    long hr;
    unsigned int cchDestCurrent;

    hr = StringLengthWorkerA(pszDest, cchDest, &cchDestCurrent);

    if (hr >= 0)
    {
        hr = StringCopyWorkerA(pszDest + cchDestCurrent,
                               cchDest - cchDestCurrent,
                               pszSrc);
    }

    return hr;
}