# NF451 hand-back (2026-11-25): drift regrade, r450a / r450b header views, two destructor-only views

`NOTES.md` is NF451's own notes (method, gates, resume point). `STATUS.tsv` lists every row; `RESIDUE.tsv` lists the held ones.

## Task 1: the five rows NF448 saw DIFFER
- **No regression.** 0x0072A680 and 0x01214190 grade RELOCATION_MATCH. NF448's census was wrong about them:
  - 0x0072A680 has no `// Leaf:` line;
  - 0x01214190 is an EH funclet row graded on its parent's funclet block.
- **Still DIFFER:** the three CLandscapeBackgroundPatch rows 0x00BE7D70, 0x00BE6F70 and 0x00BE8920. They have graded DIFFER since nf127's check on 2026-11-20. They are July behaviour-only rows that need a real re-decomp.
- **Coverage question:** the landed metrics count these rows as genuine coverage. In all, 43 landed rows grade DIFFER in the parity file. This is recorded in `docs/journal/FINDINGS_LOG.md` and waits on an owner decision.

## Landed: 13 rows (`batches/2026-11-24_landing_nf451_b1_hdr` .. `b4_hdr`)

| Batch | Rows | Header opt-in | Table change |
|---|---|---|---|
| b1_hdr | 10 (unit 0x00443190) | `NPlayerGui_CStatBase.h` `FABLE_RETAIL_NPLAYERGUI_CSTATBASE_NO_CONTAINERS` | 4 stand-in deletes at 0x00442280 |
| b2_hdr | 0x00AF7C80 | `CDecal.h` `FABLE_RETAIL_CDECAL_NO_CONTAINERS` | 2 stand-in deletes (0x00AF7C10, 0x00AF8AD0) |
| b3_hdr | 0x004582BA | `CSpecialAbilitiesUnholyPowerDef.h` `..._DTOR_DECL_ONLY` | stand-in to `??1CSpecialAbilitiesUnholyPowerDef@@UAE@XZ` |
| b4_hdr | 0x00459AA2 (+ test) | `CBrainDef.h` `FABLE_RETAIL_CBRAINDEF_DTOR_DECL_ONLY` | stand-in to `??1CBrainDef@@UAE@XZ` |

- **Why the views:** each view keeps the row's TU free of `stlport/fable_containers.h`. That header adds `/DEFAULTLIB:"libcp"` to the object's `.drectve`, which retail's objects never had. This is why r450a and r450b were declined.
- **Best guesses (`one-source:fablewin-pdb`):** three FableWin-only destructors were registered: 0x00442280, 0x004582D6 and 0x00459ABE. descriptive_check reports 190 lines, 0 FAIL.
- **T3 lines closed:** nf294 0x00459abe and nf303 0x00442280. Only the proven spelling is still bound at each of them.

## Rulings (advisor decision, owner delegated)
- **r451a: containers-free views. Granted with conditions.**
  - The view must keep every member at its real offset. Both views pass this: the two CStatBase vectors are the tail (0x30 + 2 × 12 = 72), and the CDecal CArrays sit at +0x5C / +0x68 after the last kept member.
  - equiv is not waived. NF451's CStatBase block sat above the full view, which moved its unpinned `FABLE_STATIC_ASSERT` line and changed 8 includer rows. The coordinator rewrote it before landing:
    - the guard line now also tests the macro;
    - the view sits below the full view, so the full view keeps its line numbers. The agent's version is in `evidence/`.
- **r451b: destructor-only views. Granted, narrowly.**
  - A row may use one only when its sole use of the class is naming a member function: no member access, no `sizeof`, no construction, no delete.
  - The view repeats the full view's base and the PDB destructor (`UAE` = public virtual).
  - It is not a precedent for memberless views in rows that touch `this`.
  - 0x0045782F may use the same recipe.
- **Coordinator additions:** an `#error` on every combination with the full view's macros, in all four headers.
  - CDecal.h: `NO_CONTAINERS` together with `FABLE_RETAIL_CDECAL`.
  - UnholyPowerDef: `DTOR_DECL_ONLY` together with ALLOC, RTTI_DECLS or COPY_DECLS.
  - CBrainDef.h: `DTOR_DECL_ONLY` together with `FABLE_RETAIL_CAIBRAIN_INITGROUPS_PASS26`. Before, PASS26 won silently.
- **b4's test** stubs `??1CBrainDef@@UAE@XZ` through a test-side `/alternatename`. 542 landed tests already use that technique. q442a concerns `.drectve` tokens in row objects, so it does not apply.
- **Lock takeover:** NF451 overwrote ten NF450 locks. NF450 had finished, and r450b was in NF451's brief.

## Incident
NF451 ran the repo's `apply_table.py` without `--dry` from a sandbox. That rewrote 4 lines of `rebuild/oracles/reloc_bindings.tsv` for a few minutes.
- NF451 restored the file, and the coordinator confirmed it is byte-equal to HEAD (sha256 4619503830fbb14d).
- The tracker rerun was identical to the earlier one.
- NF452 was running during that window, so check its base copy of the table at its hand-back.
- A GOTCHAS line was added; every brief now says `FABLE_ROOT=<sandbox>`.

## Coordinator checks at landing
- **Lander:** 13 / 13 landed, 0 held, 0 recheck failed.
- **Baselines unchanged:**
  - bindings_multi: 1 (NullCoord);
  - funclets: 1,698 checked, 0 broken;
  - s units: 157 checked, 0 failed.
- **equiv_check** of the four headers: 57,160 rows checked; the 13 that differ are exactly the batch rows, 0 outside (`evidence/EQUIV_CHECK.txt`).

## Held (RESIDUE.tsv)
- **0x00443CF0:** an identity problem. The model reads +0x154 on a 0xC0-byte class.
- **0x00739E20:** CTCCreatureModeManager.h opt-in; 6 callers.
- **0x00AF7CA0:** CDecal::Delete self identity.
- **0x0045782F:** the r451b recipe applies.
