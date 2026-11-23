# NF314 lessons (for the next naming batch agent)

- **The table under-lists binders, so audit deletes from the exe.** reloc_bindings keeps one witness per (symbol, target), and the
  tracker's spellings column comes from it.
  - Before writing any delete, take every landed exe caller of the target: calls (xsites.py) plus absolute references (absrows.py).
  - Grep each caller's current source for the old declaration form, and place each hit in its enclosing class (tools/delaudit3.py +
    tools/hitctx.py).
  - Here, together with a widened name-only pass, it turned 8 planned deletes into rewitnesses and found two more rows to move (0x004FF8B0, 0x00958000).
- **A binder-only move can break a link HEAD resolved.**
  - If the self row DEFINES the old spelling (read its HEAD object's defined symbols, not just its Leaf), moving one binder leaves it
    unresolved: 0x00740340 against the PAV CModeStandBase ctor.
  - That is a self + every-binder unit (NF304 Q6), or held.
- **Inherited virtual dtors change the spelling of a QAE-looking declaration.**
  - `~T();` in a local model whose base declares `virtual ~Base()` binds UAE. Read the base before counting it as a QAE binder: 5 of 8
    such hits were really UAE.
- **Vtable-slot spellings have binders that no call scan sees.** A row whose TU emits a vftable binds every slot's symbol.
  - For a slot line, scan every landed model of the class for the member's access and virtual-ness (tools/clsmember.py).
  - Then compile candidates to find a real witness.
- **virtual on a base can add a vptr store.**
  - Making a non-polymorphic local base's dtor virtual makes the derived dtor that the row DEFINES store its vptr and emit ??_7.
  - That is a vtable appearing, so stop.
  - It is safe only when the TU defines no ctor / dtor of a class that gains a vtable.
- **Check the emitted ??_7 set before adding a virtual.** If the TU already emits a derived class's vftable (an inline ctor), a new virtual
  in its base changes that table.
- **A re-scope can rename implicit members the TU emits.** Moving a local base into its PDB namespace renamed its pick-any implicit dtor
  onto a non-PDB QAE spelling (the PDB dtor is UAE): NF307 D1.
- **Keep the target screen current.** The target of NF309's ready entry 169 became an NF312 witness overnight. Re-run mkstayoff.py and
  so_check.py at the start, not only the census.
