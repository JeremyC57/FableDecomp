# NF426 hand-back (2026-11-25): CDefPointer<T>::Release / ~CDefPointer out-of-line copies

**Landed:** 110 rows in `batches/2026-11-24_landing_nf426_b1..b3` (38 + 38 + 34), all MATCH, 0 held.
- **Before:** each row was an unreferenced out-of-line COMDAT copy of a header inline, carrying a stand-in name (`?Reset@AutoTinyCountedHandle@@QAEXXZ` or `CIVCountedPointer_Release_<addr>`).
- **After:** it is the explicit instantiation of `CDefPointer<const T>::Release()` or `~CDefPointer()` from `engine/CDefPointer.h`, under its full PDB spelling.
- The catalog names, modules and file names follow (110 `git mv`), and 110 self-symbol lines were added.
- **Held, R-NF426-1 below:** 2 rows of the agent's 112 ACCEPT. They are 0x00788B55 / 0x00788CBA, the tc_trap pair of the 24-byte size-mode shape.
- **Open:** 434 family rows, in the T3 queue as one family line per reason. The per-address list is in `T3.txt`; every address with its TU, counts and candidate T is in `FAMILY_TABLE.tsv`.

## How T and the member were identified
Full method in `NOTES.md`.

**T per TU, accepted on two sources:**
1. The FableWin.pdb module of the copy's TU is found from the nearest PDB-named neighbours.
2. A count gate: the retail copies in the TU must equal the module's `?Release@` + `??1` names, and those must be of one T.
3. Ego_r.pdb's object of the same name must instantiate `GetSubDef<T>` or `GetDef<T>`.

**Which copy is Release:** the two copies are byte-identical, so the lower copy is taken as `Release` and the higher as `~CDefPointer`.
- The rule: an inline's out-of-line copy precedes every COMDAT that expands it.
- VC7.1 /O1 and /O2 test compiles put Release first in 21 of 21. FableWin's /Od order is the reverse, which the toys reproduce.
- On retail, no SetDynamicCast or operator= body sits below the lower copy in any of the 56 TUs.

## R-NF426-1 (advisor decision, owner delegated, 2026-11-25): each row is `direct` or `rule`
Both names are PDB names, so the naming tracker counts every row done. A swapped pair would still byte-match and would be invisible. Each row is therefore graded by what retail itself shows (`DIRECT_RULE.tsv`).

- **direct: 26 rows in 13 TUs.**
  - In these TUs, retail has a SetDynamicCast (or operator=) body of the SAME T between the two copies (`evidence/assignscan.tsv`).
  - FableWin's module lists exactly that T's SetDynamicCast / operator= (checked by the coordinator).
  - Under the rule, that body must follow Release and may not precede it, so the lower copy is Release by retail's own layout.
  - The TUs: combat_action_balverine, player_manager, tc_bed, tc_camera_collision, tc_clock, tc_graphic_appearance_replaceable_mesh, tc_hero_special_movement, tc_physics_base, tc_skeletal_morph, tc_smoke_generator, tc_spot_light, tc_summoned_creature, tc_will_response.
- **rule: 84 rows.** Here the order rests on the rule alone: the toys plus the 0-of-60 contradiction scan. All 84 are the 27-byte /O2 shape, the same shape as every direct row and the toys.
  - tc_hero_experience counts as rule: its between body is `operator=` of a different T, CHeroStatsDef.
  - tc_opinion_source counts as rule: its module has two SetDynamicCast Ts.
- **held: 2 rows,** the 24-byte size-mode shape (tc_trap). It has no direct TU. The only size-mode toy (`inst_s.cpp`) uses explicit instantiation, which fixes the order by source order, so it is no evidence. They land when a natural size-mode toy, or a retail body between the copies, settles the order.

If a later finding contradicts the rule, re-check the 84 `rule` rows first.

## Exposed debt (NF419 r2)
36 unchanged landed rows emit 40 differing unreferenced copies of the newly bound names.
- Two are link-order hazards: the TUs of 0x0071EF40 and 0x0083CEC0, GOTCHAS nf399_b2.
- They are queued as one T3 family line, with the per-address list in `T3.txt` section 1.
