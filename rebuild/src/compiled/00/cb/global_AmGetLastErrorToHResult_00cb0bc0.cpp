// AmGetLastErrorToHResult() -- DirectShow wxutil.cpp: HRESULT_FROM_WIN32(GetLastError()), or E_FAIL when no error is set
// Leaf: _AmGetLastErrorToHResult@0
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00CB0BC0, 29 bytes. STDAPI = extern "C" __stdcall with no arguments.
// Library: DirectShow base classes (strmbase wxutil.cpp). The manifest label "Win32ErrorToHRESULT" is descriptive.
// Callee: kernel32 GetLastError (IAT 0x0143FD58).
// Shape: HRESULT_FROM_WIN32 is the SDK `(HRESULT)(x) <= 0 ? (HRESULT)(x) : ((x) & 0xFFFF) | (FACILITY_WIN32 << 16) | 0x80000000`
// form: the zero test goes to E_FAIL, the `jle` returns the value unchanged.
extern "C" __declspec(dllimport) unsigned long __stdcall GetLastError();                 // IAT 0x0143FD58

extern "C" long __stdcall AmGetLastErrorToHResult()
{
    unsigned long dwLastError = GetLastError();
    if (dwLastError != 0)
        return (long)dwLastError <= 0 ? (long)dwLastError
                                      : (long)((dwLastError & 0x0000FFFF) | (7 << 16) | 0x80000000);
    else
        return 0x80004005L;                         // E_FAIL
}