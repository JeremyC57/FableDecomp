// T::Hook() -- empty member hook (a do-nothing virtual override)
// Retail: Fable.exe (TLC, VC7.1) @ 0x004036A0, 1 byte. Callees: none.
//
// A single `ret`. This is the body the linker gives every member that does nothing: default virtual
// hooks such as OnPreDeviceReset, empty Update overrides and the like all share it. Which of them this
// particular address serves is decided by the vtables that point at it.
//
// Matching: `this` in ecx is unused; any empty member compiles to a lone `ret`.
struct T { void Hook(); };
void T::Hook() {}
