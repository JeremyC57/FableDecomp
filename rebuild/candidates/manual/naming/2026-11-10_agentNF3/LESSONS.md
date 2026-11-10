# Lessons (agentNF3, name-fix wave)

- Under Wine, VC7.1 `cl.exe` hangs (it does not exit) after a fatal C1xxx error, such as a header `#error` or a missing include path. A grade that never returns usually means a missing `FABLE_RETAIL_*` prerequisite. Run every grade under `timeout`.
- Before adding an opt-in, grep the header for the method. `engine/CHitParameters.h` already declared `IsMeleeStrike() const`, so an overlay opt-in for it was a duplicate (C2535).
- In VC7.1, an overloaded virtual declared later takes the earlier vtable slot (`AddItemToInventory(CThing&, ...)` sits at +0x140 although it is declared second). Do not trust slot comments for overloads.
- An emitted `??_G` needs the vftable in the TU. Defining the out-of-line virtual destructor in the same TU (as the retail TU did) emits the `??_G`, which then calls `??1` out of line. No invented constructor is needed.
- VC7.1 calls a virtual member directly when it is called unqualified on `this` inside a constructor or destructor, or on an automatic object. So turning `void Shutdown()` into `virtual void StopAllSamples()` keeps the direct call.
- Pointer and reference parameters have the same ABI. A respell of `T*` to `T&` changes only the call site (`f(p)` becomes `f(*p)`), and a derived-to-base reference conversion at offset 0 emits nothing.
- A private member such as `CDataInputStream::ReadWithSrcChunkOverflow` (`AAE`) can only be bound from an inline caller inside that class. Move the inline `Read` into the base model rather than making the member protected (`IAE`) or public (`QAE`).
- `engine/CVectorMap.h` → `CArray.h` → `stlport/fable_containers.h` brings in the real STLport `std::list`/`std::vector`, so a local `namespace std { class list }` model clashes (C2955). Use the shared container and its `*_DECL` opt-ins instead.
- A `std::map` const find with two lookups matches retail's shared 8-byte frame only when each iterator lives in its own block scope (`{ it = find(0); end = end(); ... } { it2 = find(1); ... }`). Without the scopes VC7.1 gives each iterator its own slot.
- `chk.py`'s `// Flags:` regex must not use `\s*`, which runs on into the next line, so an empty `Flags:` line picks up `//` as a flag. Use `[ \t]*`.
- A mechanical respell batch (rewrite, grade, keep only MATCH/RELOCATION_MATCH with the target binding OK, otherwise revert) is safe and fast. It fixed 21/53 WriteSLONG and 12/20 PutThingInCarrySlot binders in about one minute each.
- Making `~CBaseIntelligentPointer` virtual in a row that models the vptr as an explicit `void*` member shifts the layout. The intelligent-pointer family needs the shared engine/CIntelligentPointer.h, not a one-word respell.
