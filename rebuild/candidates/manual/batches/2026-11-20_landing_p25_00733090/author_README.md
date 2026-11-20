# NEW catalogue rows: CCombatAbility::IsActive 0x00733090, SetAsActive 0x007330A0, SetAsFinished 0x007330C0, Reset 0x007330E0 (p25N, 2026-11-20)

Landing order 1 of 3. Companions of 0x00733CF0 CTCCombat::UpdateGetHitCounterAttack: that row's same-TU copies must be landed rows' bodies.

| row | grade | bytes | retail evidence |
|---|---|---|---|
| 0x00733090 CCombatAbility::IsActive | MATCH | 11 | `mov edx,[ecx+4] ; xor eax,eax ; test edx,edx ; setge al ; ret` |
| 0x007330A0 CCombatAbility::SetAsActive | RELOCATION_MATCH | 28 | fild ConstantFPS * [data+8] -> __ftol2 -> [this+4], `ret 4` |
| 0x007330C0 CCombatAbility::SetAsFinished | RELOCATION_MATCH | 27 | fild ConstantFPS * [data+0Ch] -> __ftol2 -> [this], `ret 4` |
| 0x007330E0 CCombatAbility::Reset | MATCH | 14 | `mov dword [ecx+4],-1 ; mov dword [ecx],-1 ; ret` |

- **Checks:** no flag, pragma, device or Inferred line. Strict vland passes (`vland.log`: LANDVERIFY_STRICT_HEADERS=1, FABLE_HDR_OVERLAY=`hdr/`,
  fresh LANDVERIFY_WORK). The run used a shadow `FABLE_ROOT` (`$W/tmp/shadow733`), as in the 0x006E7EB0 route.
- **Shadow root:** a symlink tree of the repo, with real copies of `rebuild/manifest/functions.tsv`, `boundary-starts.tsv` and
  `tools/decomp_pipeline/crawl/manifest_add_boundaries.py`. `boundary-starts.lines` was appended and `manifest_add_boundaries.py
  --write` was run there ("4 boundary starts to add (57844 manifest rows) / written").
- **Boundaries (`boundary-starts.lines`):**
  - int3 pads around each row: 0x00733081..8F after the 0x00733080 `ret`, then 0x0073309B..9F, 0x007330BC..BF, 0x007330DB..DF and
    0x007330EE..EF.
  - Catalogued neighbours are 0x00733020 and 0x007330F0. Nothing in functions.tsv lies between them.
  - None of the four has an E8/E9 caller (`xref.py`) or an absolute dword reference anywhere in Fable.exe. They are unreferenced
    out-of-line copies: 0x007332F0 IsValid expands IsActive, and 0x00733CF0 expands IsValid and SetAsActive.
- **Names:** Ego_r.pdb + FableWin.pdb ?IsActive@CCombatAbility@@QBE_NXZ, ?SetAsActive@CCombatAbility@@QAEXABVCCombatAbilityData@@@Z,
  ?SetAsFinished@..@QAEXABVCCombatAbilityData@@@Z and ?Reset@CCombatAbility@@QAEXXZ. The Ego_r order is IsActive 0x0074B240 /
  SetAsActive 0x0074B250 / SetAsFinished 0x0074B270 / Reset 0x0074B290, the same order and spacing as retail 0x00733090 / 0x007330A0 /
  0x007330C0 / 0x007330E0.
- **Text oracle:** Ego_d.pdb tc_combat.cpp (line-table file id 3264, also the file of 0x00733CF0).

| row | Ego_d lines | Ego_d bytes at /Od | ours |
|---|---|---|---|
| IsActive | 194 | 14 | 14 |
| SetAsActive | 219 | 28 | 28 |
| SetAsFinished | 225 | 27 | 27 |
| Reset | 231 / 232 | 10 / 9 | 10 / 9 (ActiveTimer stored first, as retail) |

  Parameters are Ego_d's (combat_ability).
- **Define block:** the tc_combat.cpp rows' common block, identical in all seven files (these four, 0x007332F0, 0x00733620 and
  0x00733CF0), so every same-TU copy compiles from one text.

## Header changes (`hdr/`, `hdr.diff`; strictly add-only; the same overlay for all three batches)
| header | opt-in | content |
|---|---|---|
| CCombatAbility.h | **new** FABLE_RETAIL_CCOMBATABILITY_MEMBERS | declarations: IsActive / IsValid / SetAsActive / SetAsFinished / Reset |
| CTCCombat.h | **new** FABLE_RETAIL_CTCCOMBAT_GETHITCOUNTERATTACK | declarations: private GetCombatAbilityData(TCombatAbilityList::iterator&, ECombatAbilityInterfaceType), public UpdateGetHitCounterAttack(const CThingCreatureBase&) |

- The only `-` lines in hdr.diff are CCombatAbility.h's four FABLE_STATIC_ASSERT -> FABLE_STATIC_ASSERT_AT pins (original lines 11, 29,
  30, 31).
- newly_flagged_rows: none (`newflags.py hdr/`).

## Tests and mutants
- **Tests:** each test links the REAL row object through an `/alternatename` __fastcall stand-in. ConstantFPS is a test variable, also
  through `/alternatename`. The tests check the stored timer, the truncation toward zero (negative products too), and that the other
  timer and the data are untouched.
- **Mutants:** 16/16 KILLED (rerun on the final define block) (`mutants/*.out`), including the RepeatTimer / ActiveTimer header swap.

## Manifest (main applies)
1. Append `boundary-starts.lines` (four TSV lines) to `rebuild/manifest/boundary-starts.tsv`.
2. Run `python3 tools/decomp_pipeline/crawl/manifest_add_boundaries.py --write`.
3. Land `payload.json` / `oracle.tsv`. The oracle bytes are from retail; the lengths are measured to the `ret`.
