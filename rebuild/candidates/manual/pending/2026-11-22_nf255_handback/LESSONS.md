# NF255 lessons (one line each)
- Settle a class vtable's slot count and owners from the exe before any header write: rvt3.py (class v CCombatActionBase v CCreatureActionBase words) gave 81 slots and the three own overrides per class in one call, where NF253's 70-slot draft map could not see slots 70-80.
- For a class whose Clone calls the base copy ctor 0x0062F3B0 out of line, only the MELEE branch of CCombatActionBase.h (MELEE_COPYCTOR) declares it; pick the prelude by what EVERY row of the class needs, not by the caller's branch (0x00951FB0 uses the third branch).
- One prelude text per class in every row (ctor, Clone, slot self rows, ??_G): the vtmap outputs of all emitting objects are then byte-equal, which is the cheapest proof that the ??_7 COMDATs are one.
- A deleting-destructor row of a class with an out-of-line ctor cannot use the `new T(args)` harness (no vtable store in the TU); placement-copy-construct instead: the implicit copy ctor stores the vtable and VC7.1 emits ??_7 + ??_G.
- vtmap's NOLEAF hides most slots; compare the new table slot for slot with a landed emitter's HEAD table on the slots that share a retail target (0x00955620: 77 / 77 and 76 / 76 equal) to prove no new spelling.
- reloc_bindings.tsv carries no vtable-slot data relocations, so a slot symbol that is "NEW" there is not a split; check prefix-only Leaf lines (`?OnBlowBlocked@CCombatActionBase`) before calling a vtmap DIFF a split.
- A `// Flags: default` line in a landed row reaches mp.py-style payload builders as a literal flag; the catalog CompilerFlags (none here) is what the build uses (GOTCHAS 499): clear it in the payload.
- End a Bash call with `; true` after a `kill -0` test: a finished job makes the test the call's non-zero exit.
