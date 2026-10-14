# Batch `2026-10-14_linux_eh21` — hand-authored small EH-framed bodies: 16 bodies (16 landed)

Twenty-first hand pass (Wine lane, `/EHsc`): the `CGameFilters` destructor (two inline
container members whose node destructors take the states after the class's own, a registry
told to drop `this`), three audio-stream bodies under a one-word scoped critical section
(restart, start-rendering with a COM `__stdcall` interface reached at +0xc of the source, and
the two-section `__stdcall` open gate), the `PKCTRRedblackTree.cpp(18)` not-morphable warning
(chained `__fastcall` log builders), the inventory item-value / binding queries and the
flagged reset walk, the clothing-suit part visitor (two 8-byte stack visitors with inline base
destructors), the audio-worker destructor (wait, close, drain through a cursor, delete the
section), the bird-fly-to-food action request (a record viewed from +4 through an accessor),
the combat instance factory call (vector copied to a local, world counter post-incremented),
the pixel-shader close with its tick deadline, the resource-map owner destructor (drain through
virtual `Next`, three registries) and a three-vector frame setter.

Lessons kept: a non-polymorphic head in front of a COM interface must own a vtable or VC7.1
moves the polymorphic base to +0; reading the count through an inline `GetCount()` accessor
swaps the loop's ebx/edi roles; `m_pSource->` re-read at each use (no local) keeps the
null-checked conversion separate from the unchecked member call; the literal `1.0f` is a pool
load under the DDK. Parked: 0x00C34B80 (the remove-last search loop keeps indexed addressing
and reloads the array base in retail; the DDK strength-reduces it).

All graded `RELOCATION_MATCH` + `PASS` on 2026-10-14; funclets in `2026-10-14_linux_funclets25`.
