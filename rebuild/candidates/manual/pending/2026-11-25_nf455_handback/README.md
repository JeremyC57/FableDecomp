# NF455 hand-back (2026-11-25): PDB same-name respells (ruling r454b)

NF455 took the 456 landed spellings whose `?name@scope@@` appears in a PDB set with another signature. It handed back 35 ACCEPT rows in three batches, and 8 more that pass every gate but are held. `NOTES.md` covers its method and resume point; `evidence/` holds the systematic counts.

## Landed: 18 constructors respelt onto their PDB names (+18 proven)
- **b2_ctor_thingref, 10 rows:** `(CThing*)` → `(CThing&)` (Ego_r + FableWin). No landed caller found from retail call sites plus objects. Rows: 0x00740250, 0x007418D0, 0x007419B0, 0x00741A90, 0x00741EE0, 0x007438A0, 0x008D7CD0, 0x008D7D60, 0x008D7DE0, 0x008D9220.
- **b3_ctor_thingref_alloc, 16 rows (8 units):** the same respell, each with its landed `CTCX_Alloc` factory. The factory now calls `new CTCX(*owner)`.
  - The 8 factory-to-constructor bindings were added to the table; before this the table had no line for them.
  - The factories keep their stand-in self names. Both PDBs spell them `?Alloc@X@@SIPAVCTCBase@@AAVCThing@@@Z`; that is a later respell.
- **Gates:** grade RELOCATION_MATCH both sides, tests PASS, objproof ALL_EQ with one rename per row, b3 link 0 duplicates.
  - Constructor tie: `ret 4`, and the row stores its own vftable.
  - The tracker diff is exactly 18 moves from `agree_unverified` to `agree_pdb`.

## Reverted at landing (coordinator, advisor condition)
- **b1, 6 destructors QAE → UAE** (0x007420E0, 0x007E7ED0, 0x0084B840, 0x00BBA140, 0x00BCDE40, 0x00BDFD80).
  - Each class's scalar deleting destructor calls a free `<Class>_dtor` stand-in. The binding table cannot see those calls.
  - So the six targets would read proven while a landed caller disagrees.
  - They land later, each as one unit with its deleting destructor.
- **Three b2 constructors** (0x004DB15E, 0x004DCAB9, 0x007BBC65).
  - Their landed Alloc factories (0x004DCB3C, 0x004DEB47, 0x004D53BB) call them through a namespace-less stand-in.
  - Same treatment: they land later as factory units.

## Rulings (advisor decision, owner delegated)
- **r455a: the 16 destructors whose model emits its own vftable. Declined as respells.**
  - Adding `virtual` emits `??_G` / `??_E` at the wrong slot (the nf297 stop case).
  - Route: a per-class unit port, piloting one class first.
  - Not registered as guesses, because a PDB name contradicts them.
- **r455b: 0x00A85FF0 to protected (`MAE`).** Granted only as one unit with 0x00A86F60 (`??_G` MAE) and every row that emits that vftable.
- **r455c: the 5 FableWin-only constructors (b2f).** Land them on the FableWin spelling as `one-source:fablewin-pdb` best guesses, after the same caller census. The Alloc signature corroborates the name but is not a second source.
- **Held, not landed:**
  - b2h: 0x00740340 / 0x007710D0 are duplicate definitions (NF281 D2), and 0x0073FDB0 is under nf298.
  - Three destructors need ports: their models are not polymorphic.
  - 0x00A3B970 is rejected: retail's base has a virtual destructor, so the class can only be `UAE`.

## Systematic findings (owner items)
- **`unsigned short` vs `wchar_t`:** 15 lines differ only by `G` → `_W`, including CreateSaveGame, the CWideString operators and GFStringToFloat / Int.
  - That is a /Zc:wchar_t or typedef question, not per-row naming, so no respells were made.
- **`CPersistContext::Transfer<T>`:** in all 58 lines, retail's `ret N` matches the landed one-argument form. The PDB spelling is a different overload, so these are not respells. They may be registered as best guesses, with that note.
- **Pointer vs reference:** 166 lines, mostly creature-mode and TC constructors. This is the resume point:
  - first the 23 remaining `(CThing&)` constructors;
  - then 55 `(CThingCreatureBase&)`;
  - then 50 access-only lines.
- **The binding table misses landed callers** (28 of 28 sampled). See `docs/journal/FINDINGS_LOG.md`. Every respell now needs a caller census from retail call sites plus objects.
- **Identity:** 0x008ED950 binds `??0CModeDazed` at 0x00740160, which is the landed CModeStunned constructor. Queued in NAME_FIXES_QUEUE.
