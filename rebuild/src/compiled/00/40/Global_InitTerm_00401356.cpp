// _initterm import thunk -- jump through the import-table slot for msvcr71!_initterm
// Retail: Fable.exe (TLC, VC7.1) @ 0x00401356, 6 bytes. Callees: MSVCR71.DLL::_initterm (import slot).
//
// Six-byte import thunk: `jmp dword ptr [__imp__initterm]`. The CRT start-up code calls this to run the
// static-initialiser tables (`.CRT$XCA`..`.CRT$XCZ`) that hold every file-scope constructor in the game.
// The thunk itself has no logic; it exists because the start-up object was compiled to call `_initterm`
// as an ordinary function and the linker resolved it to the DLL import.
//
// The model below is a generic "call through an import slot" function. The names in it (free /
// operator_delete) are stand-ins: the only thing that matters is the shape, a single indirect jump through
// a dllimport slot. The real target of this slot is `_initterm`, not `free`.
//
// Matching: a function whose whole body is a call through a function-pointer global compiles to a tail
// `jmp dword ptr [slot]`; the slot address is a relocation, so any import slot matches.
extern "C" void __cdecl free(void*);
typedef void (__cdecl *FreeFn)(void*);
// Stand-in for the import slot that the jump goes through.
extern FreeFn __imp_free_01440190;
FreeFn __imp_free_01440190 = &free;
// Stand-in name for the thunk itself (the slot it reaches is _initterm).
void __cdecl operator_delete(void* p)
{
    __imp_free_01440190(p);
}
