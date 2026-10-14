# Batch `2026-10-14_linux_eh4` — hand-authored EH-framed counted-pointer destructors: 2 bodies (2 landed)

The two `~CActionParamCCountedPointer<...>` destructors (0x00C577B0, 0x00C5EEF0): the derived vtable
is stored, EH state 0 covers a call of the class's own virtual `Release` (slot 5), then the inline
base destructor restores the shared base vtable 0x012B5868. The retail call is
`call dword ptr [vtable+14h]` -- a virtual call whose dynamic type is known: written directly in
the destructor body VC7.1 turns it into a direct call, so the call sits in a `__forceinline`
non-virtual helper, which keeps it virtual and lets the compiler resolve it through the constant
vtable slot (recorded in `docs/pipeline/GOTCHAS.md`).

All graded `RELOCATION_MATCH` + `PASS` on 2026-10-14.
