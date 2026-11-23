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
