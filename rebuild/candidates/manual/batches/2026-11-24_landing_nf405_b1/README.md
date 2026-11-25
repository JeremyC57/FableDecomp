# Landing nf405 b1: 0x004E5C71 is the implicit ~CTCCreatureGroup

Retail used the compiler-generated destructor, compiled with `#pragma optimize("s", on)`. NF405 measured each spelling against the retail bytes:

| Spelling | Result |
|---|---|
| user-declared empty dtor | stores ??_7 first: DIFFER 25v19 |
| `__declspec(novtable)` | excluded: it also drops the ctor 0x004E5C2F's vtable store, which retail keeps |
| non-virtual base dtor | excluded: vftable slot 0 is a ??_G |
| empty base dtor | excluded: ~CTCBase 0x00686830 stores its vptr |
| implicit dtor | RELOCATION_MATCH |

Name sources:
- FableWin.pdb has `??1CTCCreatureGroup@@UAE@XZ` (public symbol, module debug\tc_register.obj).
- Ego_d.pdb also places it in tc_register.obj.
- tc_creature_group.obj holds the class's other members and no destructor.

Header: add-only, default-off opt-in `FABLE_RETAIL_CTCCREATUREGROUP_IMPLICIT_DTOR` in engine/CTCCreatureGroup.h. equiv_check over all rows shows only `__LINE__` typedef-name differences, and VC7.1 builds of those rows are identical.

Table:
- The stand-in `?inner@FableDestVal_004e5c71_t -> 004e350f` becomes the `??1?$_Rb_tree<..CGroupMemberData..>` line (witness 004e5c71).
- `?outer@... -> 00686830` is deleted, since `??1CTCBase` is already bound there.

Extra COMDATs: five are tier 1 against the landed 0x004E350F object. The new ~map's only home is the unreferenced jmp at 0x004E409B (nf398 Q3 precedent).

Results:
- NF405 gates: vland, pk (0 NEQ / 0 flips), /FORCE duplicates 10 -> 10, census of 62 rows with no other definer.
- Coordinator: 1/1 landed, 0 LITERAL_DIFF, self respelt from `@FableDestVal_004e5c71@4` with column 4 aligned, baselines OK.
- Closed T3 lines: nf398 and nf303 0x004E5C71.
