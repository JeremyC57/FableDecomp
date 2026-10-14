# Batch `2026-10-14_linux_hand13` — hand-authored CAnimFilterOf2Groups .. CBankFile functions: 40 bodies (35 landed)

Delegated hand-authoring pass (compiler in the loop, Wine lane) over the fourteenth chunk of
class-labelled open functions of 17–96 bytes: `CArray` deallocations and destructors for
several element types (the inlined `Exchange(slot, 0)` detach through a `T*&` reference keeps
the retail store order), `CRetailVector` push-backs and resizes, a VIA/Centaur CPU-name lookup,
`CDefWithRecords::operator=`, a message-list owner constructor, an audio scratch-stream opener,
`CBankFile` wide-string getters and `CMyAtmosMap` block accessors. Labels were corrected from
the bytes; retail `CAnimSet` carries two extra `std::list` heads.

Lessons recorded by the pass: a store through a `T*&` reference is opaque to the compiler's
alias analysis; a 16-byte POD member assignment is two `lea`s plus four dword moves while a user
memberwise `operator=` drops the `lea`s; `obj->GetVirtual()->Func(arg)` needs a named local for
the callee object to push the argument after the virtual call; a vector copy constructor must
read the source size before the zero stores; placement `new` emits the null test; float
parameters forwarded to a `__cdecl` function pointer move as dwords. Five bodies differ by the
register of a byte count, a sunk `last` store, or an empty copy loop the compiler folds.

All landed bodies graded `MATCH` / `RELOCATION_MATCH` + `PASS` on 2026-10-14.
