# 0x006F2DE0 `anonymous namespace'::CGetBestAttackerForProng::operator()

**Grade:** RELOCATION_MATCH (375/375). No flag, no pragma.
- **Strict vland:** passes (`vland.log`): LANDVERIFY_STRICT_HEADERS=1, overlay `hdr/`, fresh LANDVERIFY_WORK.
  - Its two warnings are RELOC_NAME, where the labels DB has old names for 0x0129BA3C and 0x006E7FD0. Both go in NAME_FIXES.tsv.
- **Behaviour tests:** there are two.
  - **`test/006f2de0_realobj.cpp`** links the REAL row object, including its own 0x006E7F00 and 0x006E8850 bodies.
    - Its anonymous-namespace names (`?A0x<hash>`) depend on the compile path. So it names the hash `a05a0de1` of mut.py's fixed path,
      `/tmp/claude-0/-home-user-FableDecomp/923afea2-3bc0-53db-a7ad-a8bbc78e03e2/scratchpad/w26/p25K/tmp/mut/m.cpp`, and runs only there. It is the mutant oracle.
    - To rerun elsewhere, compile once and read the hash with `strings m.obj | grep -o '?A0x[0-9a-f]*'`, then set `ANON` at the
      test's top.
    - **Mutants:** 21/21 KILLED (`mutants/006f2de0.out`): 19 in the source (the operator, the helper, IsOuterRing, GetRingRadius)
      and 2 in the header getters.
    - **Short prong directions** put the prong line at the 1e-4 threshold of 0x006E7F00, so the preferred distance and the ring
      radius show in the score.
  - **`test/006f2de0.cpp`** (the payload's test) copies the row's statements and same-TU text over stand-ins, with the same fixtures and
    checks. This follows the 0x0055F710 precedent: vland's compile path is not fixed, and the row calls the anonymous
    0x006E9F60.
- **Name:** Ego_d.pdb / FableWin.pdb
  `??RCGetBestAttackerForProng@?A0xd8b789b9@@QAEXU?$pair@PAVCThingCreatureBase@@PAVCAttackingCombatantInfo@@@std@@@Z`.
  - The class is Ego_d TPI 0x503E1 (`egod_lines/CGetBestAttackerForProng.tpi.txt`).
  - Ring / Wheel / Prong / Defender / WorldMap are references, BestCompareVal is +0x14 and PBestAttacker +0x18.
  - The locals are Ego_d's.

## What changed against the draft (pending/2026-11-11_linux_handQ8/differ/006f2de0.cpp, DIFFER(370v375) on today's headers)
- **Local models replaced.** The draft's local models are gone. The row uses the shared CThingCreatureBase / CTCPhysicsBase /
  CCombatTypeDef / CDefendingCombatantInfo / CAttackingCombatantInfo headers.
- **Functor class.** It is defined in the row (the class is TU-local in retail) with Ego_d's members.
  - The constructor's -1.0f / 0 come from the caller's inline stores at 0x006EC180 / 0x006EC188.
- **The helper is expanded.** The preferred distance is GetPreferredCombatDistanceForAttacker. Retail expands it at
  0x006F2DE8..0x006F2E2D through VC7.1's own /O2 auto-inlining; no flag or device is involved.
  - The text is Inferred: 0x006E89B0's helper with Ego_d's `float distance`, Ego_d's parameter order (defender, attacker) and the PDB's
    `__fastcall` (YI).
  - Retail also keeps an unreferenced out-of-line copy at **0x006E7EB0** (57 B). The object's copy is byte-equal to it (SAME,
    relocations only).
  - How the variants measure:
    - cdecl, or the landed (attacker, defender) order, leaves the row matching but the copy DIFFERS (63 B / 55 B);
    - `double` leaves the copy SAME but the row DIFFERS: DIFFER(379v375). See `evidence/comdat_checks.txt` and `evidence/helper/`.
- **Two same-TU bodies are visible**, each as a `// Fidelity:` copy of its landed row:
  - **GetAttackerProngSelectionCompareValue 0x006E7F00.** prong_line and my_line pass their addresses to it. With it declared only,
    c2 loads every product / difference before it stores to them: DIFFER(375v375), 16 lines (the Z,Y,X then store order at
    0x006F2E66 and 0x006F2EAA).
  - **CCombatWheel::GetRingRadius 0x006E8850.** With it declared only, Ring is reloaded from [esi] after the call, where retail keeps it
    in ebp: DIFFER(376v375).
  - Both are anonymous / same-TU functions at lower addresses in combat_manager.obj, so their bodies were seen before the row.
  - **COMDAT checks** (`evidence/comdat_checks.txt`): both copies are byte-equal to retail (SAME, relocations only), and fidcheck finds
    both copies verbatim.
  - **GetRingRadius text:** the copy is the landed 0x006E8850 file's own body. The landed 0x006E89B0 also carries a strong copy, with
    different spelling (`dist` / `c1` / `pringu`) and no Fidelity line. Both give the same COMDAT.

## Ablation (`evidence/ablations/results.txt`, preflighted by ablq.py)
- **Base:** these runs used the source before the four neutral defines were dropped and before the helper's fastcall / parameter-order
  fix. The final source grades RELOCATION_MATCH with the same undefined symbols.
- **Helper variants:** measured on the final source (`evidence/comdat_checks.txt`).
| change | grade |
|---|---|
| final source | RELOCATION_MATCH |
| 0x006E7F00 declared only | DIFFER(375v375) |
| GetRingRadius declared only | DIFFER(376v375) |
| helper `double distance` (the landed 0x006E89B0 Inferred type) | DIFFER(379v375) |
| `Ring.Index` / `Ring.RotationAngle` instead of the getters (no FABLE_RETAIL_CCOMBATRING_GETTERS) | DIFFER(369v375) |
| prong_line declared first and assigned (Ego_d's local order) | DIFFER(399v375): the assignment copies through a temporary; Ego_d's line 345 is 47 B, the copy-init size (an assignment adds 18 B) |
| pointer members instead of Ego_d's references | RELOCATION_MATCH (kept references: Ego_d's member types) |
| no inner `{ }` block (Ego_d's S_BLOCK32 for my_line / attacker_pos / compare_val) | RELOCATION_MATCH (kept, as Ego_d scopes them) |
| no CCLASS / CDEFSTRINGTABLE / CWIDESTRING / DEF_CLASS_BASE | RELOCATION_MATCH each and all four together, same undefined symbols (dropped) |
| no CTHING / CCOMBATTYPEDEF | RELOCATION_MATCH (kept: they select the retail layouts the row reads, as in 0x006E89B0) |
| any other define | SRC_FAIL |

## Header change (`hdr/`, `hdr.diff`; add-only; pin_asserts.py pinned 0)
- **engine/CDefendingCombatantInfo.h** gets FABLE_RETAIL_CCOMBATRING_GETTERS: CCombatRing::GetIndex / GetRotationAngle, with inferred
  one-member bodies. Ego_d / Ego_r / FableWin all name both.
- **newly_flagged_rows.tsv:** header line only.

## For the coordinator
- **Helper ruling applied: `float`.** Retail has an out-of-line copy at 0x006E7EB0, and this row's copy matches it byte for byte. So the
  row lands with float, the PDB type.
  - NAME_FIXES.tsv proposes the new label for 0x006E7EB0.
  - That address now has a strong definition in this row and no row of its own. Its COMDAT is retail's.
- **0x006E89B0 re-port.** It still needs `double`. Measured with the same fastcall + Ego_d parameter order:
  - float: DIFFER(344v344), 2 lines (pop esi / fmul st(1) at 0x006E8A7A);
  - double: RELOCATION_MATCH, with its helper copy then SAME as 0x006E7EB0.
  - Its current cdecl, (attacker, defender) text emits a 63-B copy that is not retail's.
  - Queued as a re-port: its residue is the float scheduling shift.
- **Parameter names of 0x006E7F00.** In the landed row they look swapped: this caller passes the defender position in ecx (first) and
  the attacker position in edx. That is not a symbol fix.
