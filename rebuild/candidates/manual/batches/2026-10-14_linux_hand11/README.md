# Batch `2026-10-14_linux_hand11` — hand-authored CAT_BanditContactEnd .. CActiveFile functions: 40 bodies (36 landed)

Delegated hand-authoring pass (compiler in the loop, Wine lane) over the twelfth chunk of
class-labelled open functions of 17–96 bytes: scripted-control action constructors and
activations (`CScriptThing` members built from the by-value `CThing::GetScriptThing()`),
`CParentDefClassBase` copy constructors and assignments from the size-optimised def module,
`CAT_ConvoySpy::Init`, intelligent-pointer records, an uninitialised-copy loop, list inserts,
`/Od` list destructors, a `CreateSemaphoreA` import call and the `CActiveFile` bodies.

Lessons recorded by the pass: the 0x0047xxxx def module is size-optimised (`#pragma
optimize("s", on)`) and retail `CParentDefClassBase` is 0x3c bytes; a `__declspec(novtable)`
intermediate action base with an inline constructor puts the zero stores before the derived
vtable store; a 12-byte `C3DVector` copied through three registers is a memberwise user copy
constructor on dword members; a byte-state compare with a long result needs an inline enum
getter; a template `CIntelligentPointer` vtable stored at +0 of a non-polymorphic record means
the pointer is the first member; a 64-bit by-value argument stores its halves from two
registers; the destination of an uninitialised copy is the byte offset form; `where->prev` must
be a named local before the four link stores. Three bodies differ by register roles, a tag
temporary homed in a live argument slot, or a `push edi` before an early-out.

All landed bodies graded `MATCH` / `RELOCATION_MATCH` + `PASS` on 2026-10-14.
