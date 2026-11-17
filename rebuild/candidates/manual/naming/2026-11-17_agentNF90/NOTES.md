# NF90 notes (round 7, entries q11178..q11189)

Slice: the 12 nf88 follow-up entries plus 34 given rows. I claimed 8 more rows through `$S/w22/nflocks` (owner NF90), for 42 rows in
`rows.txt`. The final row states are in `STATUS.tsv`: 25 ported, 17 not-a-binder, 0 blocked. All 12 entries are `done` in `APPLIED.tsv`.
Every ported row graded RELOCATION_MATCH or MATCH in my private grading directory. `recheck_final.log` is one vland `--recheck-landed`
run over all 25 changed rows.

## Descriptive (made-up) names

- **Scaffolding helpers.** These anonymous-namespace functions exist only to make a TU emit a COMDAT. Each is marked
  `// Fidelity: scaffolding, not retail`. They have no retail address, and their names are descriptive:
  - `ReferenceModeBaseOnDeactivate` (row 0x00739E60)
  - `DestroyHeroExperienceDef` (row 0x004EBB1C)
  - `AssignHeroExperienceDef` (row 0x004F914A)
- **STLport instantiation names.** These follow the library's own spelling, so they are descriptive by library. The PDBs either hold
  the Dinkumware debug spelling or no spelling at all.
  - The tree instantiations: `_M_erase`, `clear`, `~_Rb_tree`, `operator=`, `_M_copy`, `_M_create_node`, `insert_unique`, `_M_insert`.
  - `map::clear` and the map copy constructor.
  - The pair `??_G`.
  - The `find_if` instances.
  - The NAME_FIXES spellings for 0x004CCCE0, 0x004CD5D0, 0x004DD21C, 0x004E81C1 and 0x004F9207.
- **Everything else** is a PDB spelling backed by two sources (Ego_r.pdb, FableWin.pdb and/or Ego_d.pdb), cited in each row and
  in RENAMES.

## Flags for the coordinator

1. **Opt-in inside a quarantined, generated header (now repo text).** `FABLE_RETAIL_CINVENTORYCATEGORYDEF_PARENTDEF` in
   `engine/_quarantine/CInventoryCategoryDef.h` (row 0x005BE85B) landed with NF89 (0cc6bc1018), byte-identical to my overlay, so I deleted
   my overlay. No CInventoryCategoryDef.h exists outside quarantine.
2. **New scaffolding variant (q11189, row 0x00739E60).**
   - The CModeBase::OnDeactivate body is now one inline definition in `engine/CTCCreatureModeManager.h`
     (`FABLE_RETAIL_CMODEBASE_ONDEACTIVATE_INLINE`).
   - Row 0x00739E60 emits it as a pick-any COMDAT through a qualified call under `#pragma inline_depth(0)`, a variant of the
     nf86/nf88 helper for a virtual member.
   - Link proof is in `link/`. The control (a strong definition plus the new 0x008E74F0 object) gives LNK2005 + LNK1169. The two new
     objects together link with exit 0, and both carry OnDeactivate at selection 2.
3. **Possible second one-definition case.** The prefix of 0x008E74F0 equals CModeDragonBase::OnDeactivate 0x008E72B0, which is also
   inline-like and landed as a strong row. Not acted on. Also check the OnActivate pair 0x008E7290 / 0x008E7390.
4. **Lighting global (row 0x00B33010, not-a-binder probe).** The global it reads is 0x01436EA0, not 0x01436E9C. Its current name is a
   stand-in.
5. **CThing::TCsToBeRemoved.**
   - The list at 0x013B8A24 is CThing::TCsToBeRemoved. Evidence:
     - the Ego_d.pdb PUB32 type: a list of pair<CIntelligentPointer<CThing>, CTCBase*>;
     - its 0x14-byte sentinel, allocated by the initializer 0x0121A440;
     - its use by AddTC 0x004C9D60 and RemoveTC 0x004C9840.
   - The catalog name `CThing::$E_TCsToBeRemoved` on 0x0121A670 is therefore wrong: that initializer builds a tree at 0x013B8A8C.
     Its FableWin initializer-order argument needs another look.
   - See NAME_FIXES 0x013B8A24.
6. **Implicit members of CHeroExperienceDef (rows 0x004EBB1C / 0x004F914A).**
   - Both are compiler-generated in the shared header (`FABLE_RETAIL_CHEROEXPERIENCEDEF_TRANSFER`; no header change).
   - Each row emits its COMDAT through a scaffolding helper.
   - The destructor row relies on the def-base K050 behaviour (no vptr store of its own), as row 0x00F26DF0 does.
7. **Inline-budget flags.** These per-row flags replace header controls:
   - 0x004F9934 `/d2inlS22`
   - 0x004C9D60 `/d2inlS60` (HasTC expansion; the default, 20, 30 and 45 all give DIFFER 461v504)
8. **The q11188 port changed several callee spellings** (RENAMES section 2, NAME_FIXES):
   - InitTC is private (`AAE`).
   - `std::sort` / `_M_insert_overflow` / `bitset::set` / `list::remove` / `list::push_back` now use their STLport names.
   - FableWin.pdb shows CVectorMap `Insert(key, value)` + `Sort()` beside AddTC and no `Update` for that map. The row uses
     INSERT_KEYVALUE + INSERT_VALUE_TYPE + SORT_INLINE accordingly.

## Rebase onto the NF89 landing

- The coordinator landed NF89 mid-run. `hdr/engine/CCategory.h` and `hdr/engine/NInventory_CTCInventoryBase.h` were rebased onto the
  repo copies by a three-way merge.
- My duplicate CCATEGORY_ADDITEM / CCATEGORY_GETPDEF blocks were dropped, so each block now exists once, in the repo text.
- The `orig/` copies were refreshed. `pin_asserts.py` pinned 0, and tokid gives 16/16 IDENTICAL.
- `recheck_final.log` is the full recheck after the rebase. The pre-rebase run is `logs/recheck_prerebase.log`.
- The CThing.h opt-in is two member macros: `FABLE_RETAIL_CTHING_INITTC` and `FABLE_RETAIL_CTHING_TCSTOBEREMOVED`.

## Rule slips

- **PASS recorded too early.** STATUS showed PASS before vland had run on some early rows. They were verified afterwards (batch1 /
  batch2 logs, then recheck_final.log).
- **Local models left in a ported row.** Row 0x005C1921 was first recorded `ported` as an unchanged copy of the landed text, which
  still had local NInventory::CItem / CIsSameItemType / std models. The recheck warned SHARED_HEADER_AVAILABLE CItem. It was then ported
  onto the shared headers (RELOCATION_MATCH, 2 grades in all).
- **Entry order.** I worked q11189 before q11188, against the queue order the brief set.
- **Rows blocked on a probe only.** 0x004EBB1C and 0x004F914A were first marked `blocked: port` after a single probe. They were later
  ported (2 and 1 grades).
- **Leaf slip.** Row 0x006DC9B0 had a `@3@` vs `@4@` slip in its Leaf line, which cost one wasted grade.
- **Non-zero Bash exits.** Two Bash calls exited non-zero: a symsel run on a missing object (exit 1), and an `ls` of a missing
  `overrides.tsv` (exit 2).
- **Not a slip, for the record.** Row 0x004F9934 took 12 compiles, inside the 15 port cap. Its first header opt-ins were withdrawn
  for the preference-2 flag.
