// StringCopyWorkerA(char* pszDest, unsigned int cchDest, const char* pszSrc) -- strsafe.h worker: copy pszSrc into a cchDest-character buffer, truncating (STRSAFE_E_INSUFFICIENT_BUFFER) and always NUL-terminating; a zero-size buffer is STRSAFE_E_INVALID_PARAMETER
// Leaf: _StringCopyWorkerA@12
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C55750, length 77 bytes; `ret 0Ch` (extern "C" __stdcall).
// Callees: none. Caller: StringCatWorkerA 0x00C560DA (the OpenAL wrapper TU builds its device and EAX strings with
// StringCchCatA / StringCchCopyA, whose workers are emitted out of line). The catalog name is the decorated C name.
// Shape: the Platform SDK strsafe.h body verbatim (STRSAFEAPI = `__inline HRESULT __stdcall`; here a plain extern "C"
// definition so the out-of-line copy is emitted): the copy loop counts cchDest down in edi, the truncation path backs
// pszDest up one character before the shared NUL store.
#define STRSAFE_E_INSUFFICIENT_BUFFER ((long)0x8007007AL)
#define STRSAFE_E_INVALID_PARAMETER   ((long)0x80070057L)

extern "C" long __stdcall StringCopyWorkerA(char* pszDest, unsigned int cchDest, const char* pszSrc)
{
    long hr = 0;

    if (cchDest == 0)
    {
        // can not null terminate a zero-byte dest buffer
        hr = STRSAFE_E_INVALID_PARAMETER;
    }
    else
    {
        while (cchDest && (*pszSrc != '\0'))
        {
            *pszDest++ = *pszSrc++;
            cchDest--;
        }

        if (cchDest == 0)
        {
            // we are going to truncate pszDest
            pszDest--;
            hr = STRSAFE_E_INSUFFICIENT_BUFFER;
        }

        *pszDest = '\0';
    }

    return hr;
}