# 2026-11-11 landing cf -- family FAM03: 742 unnamed virtual slots (`<Class>::vfunc_N`) renamed to their retail virtuals

Source: `naming/2026-11-11_agentFAM03/` (STATUS, NOTES, LESSONS, RENAMES, NAME_FIXES, LEFT, hdr README, fix14/STATUS).

**Verification**
- vland --recheck-landed ran in ten parts: all 742 rows grade MATCH.
- vland reports the tests as FAIL only because these tests print the catalog PassPattern `PASS_<addr>`. Running every test exe found that PassPattern in 742 of 742.
- Bindings were re-derived (no change). bindings_multi_check is at its NullCoord baseline.

**How each name is proven**
- Slot: the TLC RTTI vtable slot, pinned by the TLC-named slots around it. The Xbox 360 Test PDB virtual order bridges the gap; it is used only as an order.
- Spelling: the owning class's virtual in the Ego_r / FableWin name sets.
- `ret N` agrees with the signature where the body has one.
- No made-up names.

**What was renamed**
- 551 constant bodies (HasFrameUpdate, GetType, IsComponentSerialisable, ...).
- 112 empty bodies.
- 73 GetAnimFilter rows.
- 6 member getters and 1 setter.
- 40 rows also change class: the catalog had named the inheriting class.

**Headers**
- 93 headers carry opt-ins (91 from the main pass, 2 from fix14). Most are `FABLE_RETAIL_<HEADER>_VSLOTS`, which declares the missing virtuals.
- Every opt-in is `#ifdef` additions only. hdrcheck2 reports default=same, and every header is a line-superset of the repo copy.
- 14 rows first spelled a local class where a shared header exists. The agent respelled them onto the headers (fix14).
- The coordinator added `FABLE_RETAIL_CMODEDRAGONBASE_VSLOTS` to engine/CModeDragonBase.h for 0x00741D50. The opt-in puts the class in NCreatureMode, declares GetAnimFilter and types Filter. The default preprocessed text is unchanged apart from static-assert line tags.

**Left over and queued**
- 690 rows are left (LEFT.tsv).
- 445 of them have a proven name but no respelled source yet. They are queued in NAME_FIXES_QUEUE as land1011cf, together with:
  - 0x006868A0 CTCBase::FrameUpdate, whose callers still use stand-in spellings;
  - 0x00644627, a deleting destructor that is mis-bound as ClearPrimitivesIfNecessary.
- Labels: ghidra_out/labels_landing_2026-11-11cf.tsv.
- Budget note: 130 rows used 10-18 grades. The extra grades were mostly failed compiles while trying header opt-in combinations.
