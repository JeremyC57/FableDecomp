#pragma optimize("s",on)
// __onexit(_onexit_t function) -- register an exit-time callback with the C runtime
// Library: msvcrt.lib(build\intel\dll_obj\atonexit.obj) __onexit
// Retail: Fable.exe (TLC, VC7.1) @ 0x00401296, 38 bytes. Callees: msvcr71!_onexit (import), __dllonexit (import).
//
// This is the CRT's own onexit registrar as linked into the game, not Lionhead code. `atexit` and the
// static-destructor registration emitted for every file-scope object funnel through here. The CRT keeps
// a per-module onexit table; its sentinel value of -1 means "this image shares the runtime DLL's table",
// in which case the call goes straight to the DLL's `_onexit`. Otherwise the callback is appended to this
// module's own table through `__dllonexit`, which is what a statically initialised DLL/EXE pair uses.
//
// The result is the function pointer on success and 0 on failure; the caller (`atexit`) turns that into
// 0 / -1.
//
// Matching: returns the result, so the `_onexit` path is a tail jump through the import table
// (`jmp dword ptr [__imp__onexit]`), hence the dllimport declaration; `optimize("s")` makes the argument
// be pushed straight from the stack (`push [esp+0x0c]`) instead of being hoisted into ecx first.

#include <stdlib.h>

// The runtime DLL's registrar, reached through the import table.
extern "C" __declspec(dllimport) _onexit_t __cdecl CrtOnExit(_onexit_t function);
// Appends `function` to the module-private onexit table bounded by *first / *last.
extern "C" _onexit_t __cdecl __dllonexit(
    _onexit_t function, _onexit_t** first, _onexit_t** last);
// Module-private onexit table: start pointer and end pointer. The end pointer is -1 until the table is created.
extern "C" _onexit_t* DAT_0143e990;
extern "C" _onexit_t* DAT_0143e9a0;

extern "C" _onexit_t __cdecl __onexit(_onexit_t function)
{
    // No private table: hand the callback to the runtime DLL.
    if (DAT_0143e9a0 == reinterpret_cast<_onexit_t*>(-1))
        return CrtOnExit(function);
    // Otherwise record it in this module's own table.
    return __dllonexit(function, &DAT_0143e9a0, &DAT_0143e990);
}
