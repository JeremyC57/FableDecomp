# 0x005CA68E CTCInventoryWeapons::SetupActiveWeaponsOnCarrier

**Grade:** RELOCATION_MATCH (1535/1535). No flag, no header change.
- **Strict vland:** passes. LANDVERIFY_STRICT_HEADERS=1, empty overlay `hdr/`, fresh LANDVERIFY_WORK (`vland.log`).
  - Its two warnings are RELOC_NAME. Both go in NAME_FIXES.tsv: 0x005CBDBD is the landed made-up wrapper `@FableIsDead_5cbdbd@4`, really
    `??$IS_THING_DEAD@PAVCThing@@@@YI_NABQAVCThing@@@Z` (Ego_r.pdb 0x0069F345), and 0x005CBDD2 is the operator!=<CThing> label.
- **Behaviour test:** passes on the real object, over 16 scenarios.
  - It drives the object's own GetTC / PeekTC expansions (real component vector-maps and the object's LowerBound), its
    IS_THING_ALIVE / IS_THING_DEAD COMDATs and GetSubDef<CCarryableDef>.
  - It checks the exact call sequence and argument names, and the release of the def handles and the carried-things vector.
- **Mutants:** 20/20 KILLED (`mutants/005ca68e.out`).
  - One mutant is byte-different but behaviour-neutral, so it SURVIVES by design: the draft's reuse of ptc_carried_weapon in the sheathed
    branch (`.dead.out`).
- **Header changes:** none. `hdr/` and `hdr.diff` are empty, and newly_flagged_rows.tsv has the header line only.

## What fixed it (the draft, pending/2026-11-10_linux_hand1006, was DIFFER(1538v1535): `this` vs the TCI_WEAPON constant 0x48 in ebx)
- **Diagnosis first:** a dummy-local sweep (literal dummies 1..34 at entry, 1..8 mid-body; `evidence/ablations/sw1.out`) never moved the
  residue.
- **The fix:** Ego_d's lexical structure. Ego_d gives the sheathed-thing else branch its own `ptc_sheathed_weapon`, in scope +0x900, which
  opens at line 748 with the combined `PActiveMeleeWeapon != psheathed_thing && (!PeekTC || IsMeleeWeapon)` test.
  - That alone gives RELOCATION_MATCH (`sw2.out`: "sheathed").
  - Reusing the in-hand weapon local there, as the draft did, is the 3-byte residue.
- **Locals and init:** all locals are renamed to Ego_d's spellings, and `ptc_carried_weapon = 0` follows Ego_d line 674 (a 4-byte
  `and [ebp-x],0`). Both are byte-neutral.

## Ablation (each device removed singly from the FINAL source, regraded after the define drop; `evidence/ablations/final_source/`)
| device / change | grade |
|---|---|
| final source | RELOCATION_MATCH |
| `#pragma optimize("s", on)` removed (every landed CTCInventoryWeapons TU-mate has it) | DIFFER(1676v1535) |
| `#pragma auto_inline(off)` round the IS_THING_ALIVE<CThing*> / IS_THING_DEAD<CThing*> bodies removed | DIFFER(1542v1535) |
| IS_THING_ALIVE<CThing*> body declaration-only / IS_THING_DEAD<CThing*> body declaration-only / both | DIFFER(1553v1535) / DIFFER(1551v1535) / DIFFER(1553v1535) |
| GetSubDef<CCarryableDef> body declaration-only | DIFFER(1546v1535) |
| the draft's auto_inline(off) round the GetSubDef body | not needed: RELOCATION_MATCH without it (dropped) |
| no CTHING_GETTC_EXPANDED | DIFFER(1325v1535) |
| no STLP_BITSET_TEST_DECL | same bytes, but the object binds a different bitset test symbol (kept) |
| the shared engine/ThingAlive.h (FABLE_RETAIL_IS_THING_ALIVE + COMPARE_NULL_INT) instead of the local templates (`evidence/shared_alive3.cpp`) | DIFFER(1544v1535) |
| generic template bodies instead of the explicit specialisations (`evidence/v5.cpp`) | DIFFER(1542v1535) |
| Ego_d structure: the separate ptc_sheathed_weapon reverted to the draft's reuse | DIFFER(1538v1535) |

- **Precedent:** the local templates, the auto_inline(off) specialisations and the visible GetSubDef body follow the landed TU-mate
  SetThingAsActiveWeapon 0x005CB2B0.
- **Defines:** ten byte- and symbol-neutral defines from the draft are dropped (`define_sweep.txt`). The final object's undefined-symbol set
  equals the full-define build's.

## Flag rung for the auto_inline(off) region (today's 0x00A1A1A0 ruling)
- **What the pragma does:** without it, the three IS_THING_* calls (0x005CA7CB, 0x005CA947, 0x005CA9F2) are expanded instead of called.
- **Sweep of the no-pragma source** (`final_source/flag_sweep_nopragma.txt`):
  - /d2inlT1, 5, 10, 15, 20, 30, 40, 50 and 60 all give DIFFER(1542v1535): T does not move these explicit-`inline` specialisations in
    this /Os TU.
  - /d2inlS1 gives DIFFER(1538v1535), S5 DIFFER(1529v1535), and S10 / S20 DIFFER(1542v1535).
- **Result:** no flag matches, so the scoped pragma stays (ladder step 3).

## Same-TU bodies: needs a ruling (`final_source/comdat_checks.txt`)
The source carries three landed retail bodies without a `// Fidelity:` line, following the landed TU-mate 0x005CB2B0. In the row object
each COMDAT is byte-equal to retail:

| body | retail | result |
|---|---|---|
| GetSubDef<CCarryableDef> | 0x005CC10E | 162 B, SAME |
| IS_THING_ALIVE<CThing*> | 0x0040DCE0 | 21 B, SAME |
| IS_THING_DEAD<CThing*> | 0x005CBDBD | 21 B, SAME |

- **Text vs the landed rows:**
  - GetSubDef: the landed 0x005CC10E row writes an explicit `template <> ...GetSubDef<CCarryableDef>` specialisation. This row uses the
    generic template body (the 0x005CB2B0 text), with the same statements.
  - IS_THING_ALIVE: the text equals the landed 0x0040DCE0 template body, but written as an explicit `inline` specialisation.
  - IS_THING_DEAD: the landed 0x005CBDBD text is a made-up wrapper (NAME_FIXES).
- **The question for main:** add `// Fidelity:` lines with the landed text verbatim (which would change the spelling: an explicit
  specialisation instead of the generic template, and IS_THING_DEAD has no genuine landed text), or keep the 0x005CB2B0 precedent?

## Coordinator notes
- **Inline-budget line:** line 2 names every device, each with its grade.
- **Pragma:** `#pragma optimize("s", on)` is a TU property shared by all landed TU-mates, not a per-row budget. It is listed only because
  removing it changes the bytes.
- **Fidelity line:** IS_THING_ALIVE<CThing*>'s body is the same text as the landed 0x0040DCE0 template. IS_THING_DEAD has no genuine landed
  text to copy, since the landed 0x005CBDBD is a made-up wrapper (see NAME_FIXES). So no `// Fidelity:` line is used; this is the
  0x005CB2B0 precedent.
