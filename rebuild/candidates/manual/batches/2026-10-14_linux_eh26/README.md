# Batch `2026-10-14_linux_eh26` — hand-authored small EH-framed bodies: 12 bodies (11 landed)

Twenty-sixth hand pass (Wine lane, `/EHsc`): the audio-bank resume walk (a `__stdcall` over the
bank's state part with the list reached through a reference cast), the edit-control track-node
removal (inlined `Find` over an MFC-style position list, the reference released through a table
of entry points), the `CWorld` resource-allocation constructor (members read back through
`m_pWorld` in the init list, the 0x12B7CE8 container with out-of-line root and container
destructors), the `PKCFrallocStatic` slot access with the "Fralloc overflow" warning, the
`SExpSpendUndo` constructor (polymorphic base whose vtable store is dead, all scalars assigned in
the body), the `C3DMeshObject` constructor (the descriptor's origin copied with Y and Z swapped,
the scale homed in a dead argument slot, a null-checked `new` of the landed 0x00C2D2F0 volume),
the `~CWound` destructor (slot-5 virtual called through a non-virtual helper so it stays a
`call [vtable+14h]`, five handle/section holders with inlined destructors), the mesh volume loader
(unsigned-to-float `fild` plus 2^32 fix-up, the record's four words copied from the source), the
AI state-group dispatch (frame request with a 0x20-byte part, 12-byte functor, 8-byte result
record), the script float-variable update (`fucompp` compares, `delete` through a non-virtual
destructor, frame binding state and a null-checked `new`) and the `~CVolume` destructor (three
bases, two interface releases, three section holders and three one-word holders).

Lessons kept: a `CNode` container's root and container destructors may be out of line even
when the constructor is inline; `__declspec(novtable)` intermediate classes let only the
most-derived constructor store its vtables; a virtual call on `this` in a constructor or
destructor goes through an inline non-virtual helper (known vtable → `call [vtable+N]`,
`novtable` → dynamic dispatch). Parked: 0x00C101D0 (the empty-string temporary's data loaded
into edi by retail, ecx by the DDK), 0x00CAC9D0 (zero stores sunk past the semaphore pushes or
hoisted above InitializeCriticalSection), 0x00CAC4A0 (hoisted final `xor eax,eax`).

All landed graded `RELOCATION_MATCH` + `PASS` on 2026-10-14; funclets in
`2026-10-14_linux_funclets31`.
