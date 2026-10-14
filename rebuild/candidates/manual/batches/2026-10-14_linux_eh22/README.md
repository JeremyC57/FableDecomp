# Batch `2026-10-14_linux_eh22` — hand-authored small EH-framed bodies: 13 bodies (13 landed)

Twenty-second hand pass (Wine lane, `/EHsc`): a COM-style initialisation under a one-word
critical-section holder (two 8-byte records filled through a `__stdcall` slot), the script-host
try-activate body, the `CEngineWaterRenderer` destructor (OpenAL function pointers resolved on
demand through `GetProcAddress`, the module freed, a mixer handle member closed), a `__fastcall`
chunk-extent reader over a stack `CChunkedFileChunk` (derived destructor hoisted base vtable
store), a navigation-layer loader with a 0x50-byte stack request (two PDB-named members, 1.0f
scales), the two inventory unbind walks (entity array plus observer drain), a region tracker
(lookup key on the stack, node allocated into the dead argument slot), the twin predicate-chain
constructors (0x12B4010 / 0x12B4024 nodes), the `CTCDPhysicalObstruction` destructor (four
0x12B1AB0-family containers), the `C3DMeshVolume` constructor (`__ehvec_ctor` array of two
0x180-byte parts) and the hero-stats index-buffer factory (class `operator new`/`operator delete`,
error recorded once in a global and reported through a `__stdcall` hook).

Lessons kept: a non-polymorphic head in front of a polymorphic base must carry a vtable or VC7.1
moves the polymorphic base to +0; a `bool` constructor argument from a call result puts the
`this` move before the push; a POD vector copy in an init list is a plain dword block move (a
user copy constructor reorders the loads); an early `return 0` and a later "null the pointer and
return it" are two epilogues; `operator new` without `throw()` is null-checked by VC7.1 but not
by g++ (declare it `throw()` in behaviour tests). Parked: 0x00C34B80 (indexed search loop the
DDK strength-reduces), 0x00C31F60 (fail-path epilogue layout), 0x00C127C0 (ecx/eax for a tested
member load).

All graded `RELOCATION_MATCH` + `PASS` on 2026-10-14; funclets in `2026-10-14_linux_funclets26`
and `_funclets27`.
