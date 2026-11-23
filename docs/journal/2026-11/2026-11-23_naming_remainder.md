# Naming remainder and faster routes (Account A, 2026-11-23)

Owner question: "Check how much naming is left, then carefully check if there is a faster way of naming the remainder."
Numbers from `tools/decomp_pipeline/naming_status.py` (commit 3d5b2d34c) and `tools/decomp_pipeline/naming_remainder.py` (run from the
repo root; it re-derives the tracker's statuses and adds the checks below).

## How much is left

| Bucket | Addresses | Notes |
|---|---:|---|
| done: agree_pdb + agree_std | 31,786 (49.6%) | one spelling, a PDB or std / compiler name |
| open, measured: standin / split_unknown / split_fixable / split_pdb_multi / catalog_standin | 3,907 / 3,699 / 2,368 / 55 / 16 = 10,045 | the tracker's open list |
| agree_unverified | 6,947 | one spelling, not a PDB name, no stand-in marker: not judged |
| unmeasured | 15,298 | landed rows with no table binder and no full mangled Leaf |

The unmeasured bucket is not mostly done. The compile gate (`rebuild/compile-gate/retail-parity.tsv`, object_symbol column, last
regenerated 2026-11-22) covers 6,275 of the 15,298. Of those, only 140 define a PDB name and 383 a std name. 1,311 define a stand-in, and
4,441 define a mangled non-PDB name. Of those 4,441, 3,599 are free functions or ctor / dtor forms, 573 match a PDB name and class with
another signature, and 269 have a name and class absent from both PDBs. So the real remainder is roughly 20-25k addresses, not 10k.

## What this session's agents yield (why more T1 batches are not the lever)

- Throughput: 29 landings, about 840 rows, in about 6 hours with 3 agents, which is roughly 47 rows per agent-hour.
- In-place yield is falling.
  - NF311 found 6 rows in 164 verdicts and NF308 found 10 in 118.
  - NF312 found 14 in 85 and NF313 22 in 69.
  - NF303 sent 684 of 802 entries to ports by census.
- The tracker's T1 tier (1,956) is mostly ports in disguise: T3_PORT_QUEUE.tsv already holds 1,839 lines covering 1,830 addresses.

## Finding the name vs making the source emit it

These are separate problems; a proposal is sized by the addresses it closes and by which of the two it removes.

1. **Reclassify, no code (count only).**
   - 748 open addresses spell only STLport `std::` forms. The VC10 Dinkumware donor PDBs cannot match them by construction (ROADMAP item);
     they are agree_std.
   - A compile pass that records each landed row's own defined symbol would measure the 15,298 unmeasured rows (ROADMAP NF302 Q3). It is
     cheap: a vland recheck runs at 0.5-1.0 s per row, test link and run included (7 landings measured). The ~9,000 rows the gate does not
     cover take a few hours.
2. **Scripted generate-and-verify for regular shapes (removes the "emit" work, mechanically).**
   - The oracle is already mechanical:
     - HEAD and new compile;
     - objsec ALL_EQ under the expected rename;
     - a link check;
     - vland.
   - At about 2-3 s per candidate (HEAD + new + diff + link), a script tries about 1,200 or more an hour, against 47 rows per agent-hour.
   - **Largest regular shape found:** flattened `Class_Method` free functions. Example: `bool __fastcall CAFile_IsIOFinished(const CAFile*)`
     defines `?CAFile_IsIOFinished@@YI_NPBUCAFile@@@Z` for the PDB `?IsIOFinished@CAFile@@UBE_NXZ`. In the gate-covered sixth of the tree:
     - 3,478 such rows, none with a table binder;
     - 1,246 name a class and a member with exactly one PDB symbol (the target is known from the name plus the PDB);
     - 1,227 name a class absent from the PDB sets;
     - 990 name no such member (many are compiler-generated `vector_deleting_destructor` forms, which are ports);
     - 15 have several overloads.
   - **Untested hypothesis:** converting such a row to a member of its own local model, with the body kept as `const C* self = this;`,
     emits the same bytes when the function has no stack arguments (fastcall's ecx = thiscall's this). It needs a 20-30 row pilot that
     measures the yield before scaling.
3. **Family ports ranked by addresses (one header port closes many addresses).**
   - The top 10 T3 classes cover 49% of T3 addresses: CCountedPointer 966, vector 427, _Rb_tree 365, the placeholder class `T` 324,
     PKCTRList 141, then STL algorithms.
   - Within CCountedPointer, 696 are `~CCountedPointer<CUnprovenPointee_xxxx>` rows whose pointee is unknown. That is a naming (identity)
     problem first, since the destructor calls DeleteData through a pointer.
   - **Untested idea:** derive T from typed callers (a known destructor that releases a member at a known offset in its Ego_r layout). Test
     it first on destructor instances whose pointee is already known.
4. **Not faster: BSim as a naming source.**
   - The setup: `ghidra_out/bsim_port_audit*.tsv` gives donor names for 32,884 addresses.
   - Precision, measured against the agree_pdb addresses (where the right name is known), std-shaped names excluded:
     - 46.5% at similarity >= 0.95 and significance >= 10 (641 open standin / split_unknown named);
     - 74.4% at significance >= 20 (145 named).
   - Small bodies fold: 0x00401377 is `__setargv`, while BSim says AddChildPrimitive. BSim stays a hint, never a second source.

## Recommendation

1. Do 1 (reclassify + measure) first, so every later number is real.
2. Then pilot 2 on the 1,246 unique-target flattened rows. Run it as a script with the coordinator reviewing batches, not as more agent
   batches.
3. Give 3 dedicated family agents with the header lane: CCountedPointer, the STLport instance rows and the `T` placeholder class.
4. Keep the T1 batch agents only until the current three report.

## Pilot results (route 2, same day)

- **Measurement pass.** `tools/decomp_pipeline/linux_lane/lane_tools/self_symbols.py` records the compiled self symbol of the 26,666 landed
  rows without a full Leaf. It runs in chunks of 600 at about 5-6 minutes a chunk (longer when a compile hits vland's 180 s limit), with
  output in the scratchpad.
- **Scripted fix.** `tools/decomp_pipeline/linux_lane/lane_tools/flatten_pilot.py`, the flattened `Class_Method` -> member conversion.
  - v1 (shared-header route) failed. The headers do not declare these members in the branch a row compiles (e.g. CVillagePeopleDef.h is
    wrapped in `FABLE_RETAIL_CREATE_VILLAGER_PASS27`), so the header route needs generated add-only opt-ins.
  - v2 / v3 (the row's own local model): the byte oracle accepted 28 of 30, then 89 of 103 generated candidates (86%).
  - Review found 10 identity conflicts: the PDB name is already claimed by another address in the catalog or by a landed self. They are
    held as T3 identity research, and the script now rejects that case itself.
  - **79 rows landed** (`batches/2026-11-23_landing_flatten_pilot1/`, recheck WINS 79) in about an hour of coordinator time, against about
    47 rows per agent-hour for the T1 batches.
- **Where the rest of the 987 candidates went:**
  - 498 need a header opt-in (the class has a shared header and no local model);
  - 271 are named by other landed files (the binders must move in the same unit);
  - 75 have other definition shapes;
  - 40 are virtual with no vptr slot in the local model.
- **Next, in order of yield:**
  1. Generate add-only opt-in declarations for the 498, with equiv_check and a planted control per header.
  2. Move the 271 together with their binders.
  3. Rerun the candidate scan once the measurement pass finishes (it adds the rows the gate never covered).

## Advisor review of the next step (same day)

The owner had the advisor decide whether the header opt-in route should come next. Verdict: yes, as a bounded pilot of 20-30
non-virtual rows on 3-5 headers. It uses the landed `FLAT_*` opt-in form, one equiv_check and one planted control per header, and stops if
acceptance falls well below the local route's 86% or the 180 s compile timeouts persist. The advisor ordered these steps before it:

1. **Rerun the local-model route on the newly measured rows.** No new yield. All 5 accepted rows were pilot 1's held identity rows coming
   back, because the old guard only saw measured symbols and the catalog. The 11 rejected rows match last time's.
2. **Close the identity-guard gap.** `flatten_pilot.py` now also rejects a candidate when another landed source:
   - carries the PDB symbol in a `// Leaf:` line;
   - defines `Class::Method(...) {` (test stubs excluded);
   - is a file named `Class_Method_<other address>`;
   - or when `reloc_bindings.tsv` binds the PDB symbol to another address.

   Calls that spell the PDB name are agreement, not claims. Checked on pilot 1: 0 of the 79 landed rows flagged, 7 of the 10 held rows
   flagged. The other 3 are not conflicts. The table already binds 0x00662090's and 0x006D6570's PDB symbols to those same addresses, and
   0x00B25D00's callers spell the PDB name. 0x006D6570 still has a second binder spelling `@Sub_GetExperiencePickUpMultiplier@4` (0x00899C90),
   so it lands as a unit with that caller.
3. **Count the 271 "named by other files" rows before routing them.** 237 of them (15 names) are not caller units: they are shared-name
   clusters. Examples: `CTCCarryable_OnKill` is defined by 173 landed rows in two pointer forms, `CGuiVarTransferBool_TransferToGui` by 18 and
   `CTCInventoryItem_OnDie` by 18. That is one made-up name on many addresses, so at most one row per name can be the PDB member. They are
   queued as `flatten2 ... shared-name cluster` in T3_PORT_QUEUE.tsv. The 34 true caller units have 90 caller files: 17 with a local model,
   47 header-only and 26 with neither. So the caller route depends mostly on the header pilot.

## Measurement pass, part 1 (9,000 rows, `rebuild/oracles/self_symbols.tsv`)

- **Grades:** 6,996 RELOCATION_MATCH, 1,666 MATCH, 312 NOT_GRADED (chunk 4's timeout, re-measured by `--retry` at the end), 21
  LIBRARY_MATCH with no symbol, 2 DIFFER.
- **Trust only MATCH / RELOCATION_MATCH symbols.** For a row with no `// Leaf:`, the recheck falls back to the largest function, so a
  DIFFER row's symbol may not be the row's own function.
- **Two DIFFER rows to look at.** Both sources are unchanged since the tree import, and the graded function is the right one in both:
  - 0x0056C840 (`_Dest_val` for `pair<CCharString, CSymbolInfo>`) grades DIFFER(30v28), though the 2026-11-22 gate had it as
    RELOCATION_MATCH;
  - 0x00647308 (`GetElementCount`) grades DIFFER(18v17) and is not in the gate.

## Header route: the switch route, and identity by RTTI (same day)

- **The 498 header-route candidates are almost all virtual.** 491 are virtual (U/M/E), and 420 have 8 bytes or less with a body that other
  catalog addresses share.
- **292 need no header edit.** The member is already declared in the header behind an existing class-model switch, so the row only defines
  the switch, as thousands of landed rows already do:
  - `FABLE_RETAIL_NSCRIPT` 59, used by 6,288 landed rows;
  - `FABLE_RETAIL_CCREATUREACTIONBASE` 37;
  - `FABLE_RETAIL_CIENGINE` 15;
  - `FABLE_RETAIL_CAISTATEGROUPBASE_MEMBERS` 9.

  205 are absent from their header, mostly `GetSizeofClass` overrides. The advisor reconciled its earlier "non-virtual only" condition:
  that applied to adding virtual declarations to shared headers, not to defining an existing switch.
- **Identity for tiny shared-body virtuals.** The bytes cannot tell these members apart (NF306 Q2), so a second source is needed.
  `ghidra_out/labels_rtti_port.tsv`, the retail RTTI vftable placement slot by slot, is that source:
  - it names the PDB symbol at the same address for 490 of the 498 candidates, and for all 70 virtual rows of flatten pilot 1;
  - no landed row or candidate address carries two RTTI names, so nothing is folded;
  - `flatten_pilot.py` now requires it for every virtual target.

  Ego_r class sizes do not confirm the `GetSizeofClass` constants: Ego_r runs 4-60 bytes larger, being a different build. The 9
  non-virtual pilot-1 rows rest on the flattened label plus a unique PDB member.

## Measurement pass, part 2 (16,200 rows) and the switch landing

- **Measured to date:** 12,966 RELOCATION_MATCH, 2,888 MATCH, 312 NOT_GRADED (retry pending), 21 LIBRARY_MATCH and 10 DIFFER.
- **None of the 10 DIFFER rows comes from today's work.** All 10 sources are unchanged since the tree import (7633233c4).
  - 0x0082E3D0, 0x0082E420 and 0x008ED590 already grade DIFFER in the committed gate.
  - 0x0056C840 was RELOCATION_MATCH in the gate but grades DIFFER(30v28) on this lane.
  - 0x00647308 and five `IsThingAlive` copies grade DIFFER on this lane and are not in the gate. The `IsThingAlive` copies are
    0x007FB120, 0x008D04B0, 0x0093A890, 0x009445C0 and 0x00944CE0, all DIFFER(40v39) with one shape, so they likely share one root cause.
  - They are listed for a grade-repair pass, not naming work.
- **Flatten switch 1 landed 51 rows** (`batches/2026-11-23_landing_flatten_switch1/`). Together with pilot 1, the script has landed 130
  rows today.
