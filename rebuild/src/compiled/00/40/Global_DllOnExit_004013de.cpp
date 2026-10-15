// __dllonexit import thunk -- jump through the import-table slot for msvcr71!__dllonexit
// Retail: Fable.exe (TLC, VC7.1) @ 0x004013DE, 6 bytes. Callees: MSVCR71.DLL::__dllonexit (import slot).
//
// Six-byte import thunk: `jmp dword ptr [__imp____dllonexit]`. `__onexit` (0x00401296) calls it to
// append an exit-time callback to this module's private onexit table. The thunk has no logic of its own;
// the start-up object calls `__dllonexit` as an ordinary function and the linker routes it to the DLL.
//
// The model below is a generic "call through an import slot" function. The names in it (free /
// operator_delete) are stand-ins for the slot and the thunk; the real target is `__dllonexit`.
//
// Matching: a function whose whole body is a call through a function-pointer global compiles to a tail
// `jmp dword ptr [slot]`; the slot address is a relocation, so any import slot matches.
extern "C" void __cdecl free(void*);
typedef void (__cdecl *FreeFn)(void*);
// Stand-in for the import slot that the jump goes through.
extern FreeFn __imp_free;
FreeFn __imp_free = &free;
// Stand-in name for the thunk itself (the slot it reaches is __dllonexit).
void __cdecl operator_delete(void* p)
{
    __imp_free(p);
}
