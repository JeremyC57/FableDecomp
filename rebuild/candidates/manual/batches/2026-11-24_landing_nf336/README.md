# Landing nf336 (Account A, 2026-11-24): 24 CV_HauntedHouseScript-object rows onto the shared headers, + TRetVoid::Return

Why: Account B's Gate 0 for the object-order build mode (`pending/2026-11-24_acctB_b24/progress/GATE0.md`) found that 24 landed rows of
the retail object 0x00E93D50..0x00E96E10 carried their own class models, so the object could not compose into one TU. Agent NF336
re-based them; the hand-back is `pending/2026-11-24_nf336_handback/` (read NOTES.md and COMPOSE.md first), and its tools are in
`naming_batch/agent_tools/nf336/`.
- **Rows:** 23 local-model rows moved onto `engine/CV_HauntedHouseScript.h` / `engine/CSpawnedFuncBase.h` and the other shared headers, with
  their real PDB names. 0x00E95380 moved from the stand-in `?M@C00e95380@@QAEXXZ` onto `?Return@TRetVoid@@QAEXXZ`.
  - HEAD and new were graded at one work path: grade and test unchanged for all 24.
  - The section diffs show only these changes:
    - vftable slot symbols now name retail's functions, and every emitted `??_7` equals retail slot for slot;
    - callees are respelled onto PDB names;
    - libcp appears on 0x00E95F80 (NF318 Q1);
    - pick-any copies either equal retail or already differed at HEAD.
- **Headers:** both are add-only opt-ins, and each inserted hunk is wrapped in its own `#ifdef FABLE_RETAIL_*`.
  - `CV_HauntedHouseScript.h` (+58): `FABLE_RETAIL_CV_HAUNTEDHOUSE_DECLS`, `FABLE_RETAIL_CHHUNDEAD_MEMBERS` (Ego_r TPI 0x10B4C) and
    `FABLE_RETAIL_CM_HEROINSIDEHOUSE_MEMBERS` (0x10B5B). The nested `??_7` probes equal retail at 0x012ECCA0 / 0x012ECCBC / 0x012ECD20 / 0x012ECD3C.
  - `CSpawnedFuncBase.h` (+8): `FABLE_RETAIL_CSPAWNEDFUNCBASE_DTOR_DECL_ONLY`.
  - equiv_check: 0 of 972 includers differ. The planted control flagged all 972.
- **Table:** two deletes, 0 misses, and no new multi-target symbol.
  - the `?IsActiveThreadTerminating@CV_HauntedHouseScript` stand-in: the census found no other user;
  - a stale `??_7CVtable_012ecb94` witness.
- **Catalog:** three entries renamed.
  - 0x00E95380: `_global::Nop_ret0_00e95380` becomes `TRetVoid::Return`, and its files move from `E2_E2_00e95380*` to `TRetVoid_Return_00e95380*`.
  - 0x00E95D80 / 0x00E95E80: the Module becomes `NScript::CV_HauntedHouseScript::CHHUndead`.
- **Coordinator checks:**
  - all `orig/` rows and headers equal the repo;
  - recheck on the landed tree, covering the 24 rows plus the direct includers of `CV_HauntedHouseScript.h` (26 rows):
    `logs/recheck.log`.

## Rulings (coordinator)
- **Q1, 0x00E96C30: skipped-port.** The port keeps the grade but changes the COMDAT set (STLport `vector<CScriptThing>` helpers) and adds libcp.
  It goes to the STLport census as T3 `nf336`, with the attempt in `evidence/attempt_00e96c30.cpp`. It is now the last local model in this
  object, so B's Gate 0 still fails on it.
- **Q2, `~CLevelScriptBase<T>`:** 0x00E93D50 / 0x00E93E60 / 0x00E96DD0 keep the declared form (COMDAT set = HEAD). The implicit form adds an
  unreferenced thunk (NF320 Q2); the sources for it are parked in `evidence/q2cand/`. 0x00E96E10 keeps the implicit form it had at HEAD. Two
  forms in one object is a fact for Gate 0's per-row model choices (COMPOSE.md), not a landing blocker.
- **Q3, accepted:** the decl-only `~CSpawnedFuncBase` opt-in. It is per TU and leaves the header's default (implicit) as it was. It is backed by
  retail: with it the `??_GCSpawnedFuncBase` copy is EQ retail 0x00CDD4A0, and `??1CSpawnedFuncBase@@UAE@XZ` is in Ego_r and FableWin.
- **Q4, accepted:** 0x00E95F80's `??1CActiveEntityScriptBase` copy changes from 35 to 51 bytes. It was a pick-any copy that already differed
  at HEAD (nf203 rule), and retail has no separate owner to compare it with.
- **Q5, accepted:** 0x00E95380's anonymous-namespace scaffolding helper, the nf86 / nf88 form marked `// Fidelity: scaffolding, not retail`.
  NAME_FIXES_QUEUE line 6499 (land0611i) and the `acctB_b24 PRIORITY` 0x00E95380 line are both satisfied by this landing.
- **Q6, accepted:** the vftables now carry retail slot identities, and 0x00E95F10's two vftables grow from 2 to 7 slots as retail's do.
  The three `?Init@CM_*` slot symbols with no Leaf or table line are queued with NF336's other NAME_FIXES (`nf336`, 10 entries).
