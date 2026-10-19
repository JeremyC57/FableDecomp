# Script-thing identity and spatial initialization

008CFFE0 independently passes RELOCATION_MATCH and exact-source host C++98/O2, no-elision and canonical VC7.1 tests. Actual CGameScriptThing, CTCPhysicsBase, quarantined CThing and string headers remain included; documented typed native views account for retail container widths and preserve full host pointers. The actual intelligent-pointer hierarchy invokes its genuine constructor/Get/Set/destructor; no raw vtable injection is used.

The routine binds the input through a temporary intelligent pointer, reloads the live thing for name/UID/data, conditionally restores a category1 home position and copies physics position/angle when available. Native ordered comparison means equality and NaN enter the home restoration branch. Root checked all18 relocated operands, tolerance bits38D1B717, intelligent-pointer RTTI/deleting destructor, base constructor and native physics slot63. Const GetPhysicsTC and the temporary's real Get-then-Set lifetime produce the final match.

Seven fixture cases cover category filtering, zero/positive/tiny/equal/NaN home magnitude, missing physics, exact Get counts,64-bit UID/string copy, preserved unrelated state and balanced references. Dependencies are stubs rather than whole-game execution. All10 immutable author grades remain preserved; no cap extension or reset. Final source/test hashes are frozen in batch evidence.
All independently byte-verified (MATCH or RELOCATION_MATCH) + PASS, with verbatim-source host and canonical VC7.1 fixtures.
