#pragma optimize("s",on)
// atexit(_PVFV func) -- CRT: register a function to run at process exit, returning 0 on success
// Library: msvcrt.lib(build\intel\dll_obj\atonexit.obj) _atexit
// Retail: Fable.exe (TLC, VC7.1) @ 0x004012BC, 18 bytes. Callees: __onexit 0x00401296.
//
// The standard `atexit` as shipped in the runtime: it is a thin wrapper over `_onexit`, which returns the
// registered pointer or 0 on failure. `atexit` is specified to return 0 for success and non-zero for
// failure, so the pointer result is folded to 0 / -1 here. Every file-scope object with a destructor in
// the game registers through this path.
//
// Matching: `optimize("s")` keeps the body as a call plus a `neg/sbb` style fold rather than a branch.
typedef void (__cdecl *_PVFV)(void);
// The registrar; returns `func` on success, 0 on failure.
extern "C" _PVFV __cdecl _onexit(_PVFV func);        // 0x00401296 (atonexit.obj __onexit)

extern "C" int __cdecl atexit_impl(_PVFV func)
{
    // Success is a non-null echo of the pointer; anything else is a failure (-1).
    return (_onexit(func) == 0) ? -1 : 0;
}
