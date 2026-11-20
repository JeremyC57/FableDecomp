# NF147 lessons (one line each)
- A mode header (NCreatureMode_*.h) compiles in a row that still models CThing locally: it only forward-declares CThing, but it needs FABLE_RETAIL_CMODESTANDBASE + FABLE_RETAIL_CMODEBASE_VIRTUALS (without them: C2118 at its sizeof assert and C2440 on the CModeBase conversion).
- A row with its own CCountedPointer<T> template ports onto the shared one in one form when the mode token stays exactly as HEAD spelled it (global `struct` = U): the called (T*) ctor keeps its spelling and only AddMode moves.
- `ptr->Method(call())` with the header's inline operator-> loads the pointer before the argument call; HEAD's direct member read `ptr.Data->` keeps retail's load-after order.
- When a ported binder would add a PDB spelling to a callee whose only referencer it is, respell that callee's self row in the same hand-back (0x00868220): the address stays on one spelling.
- A "CScriptedModeInfo"-style stand-in member is often a known class: check what the callee address's other binders call it (0x0073E8F0 = CAnimFilterOfGroup) before keeping it.
- /d2inlS bisects are cheap and decisive when the function calls the out-of-line copy nowhere: 0x00B21E90 went S83 DIFFER / S84 MATCH in 6 grades.
- mkpay takes flags from the catalog, not from the row's // Flags: line: patch "flags" into the payload for a row whose CompilerFlags will change at landing.
