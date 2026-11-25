# NF424 round 4 hand-back (2026-11-25)

This round applies the R-NF424-5 ruling to the 85 CActiveEntityScript<T> pairs, under the 2026-11-08 owner rule that inline-budget residues may use an inline control. It covers 170 ACCEPT rows plus 35 HOLD rows. Nothing was written to the repo, and git and wineserver -k were not run.

| batch | rows | content |
|---|---|---|
| acc_aes_00 | 50 | 25 thin ??_G rows + their 25 owner edits |
| acc_aes_01 | 50 | 25 + 25 |
| acc_aes_02 | 50 | 25 + 25 |
| acc_aes_03 | 20 | 10 + 10 |
| hold_aes | 35 | RESULTS.tsv only. Same holds as round 3, regraded at /d2inlS20: 34 DIFFER 272v48, 1 DIFFER 269v48 |

The layout is the same as round 3:
- RESULTS.tsv: column 4 is the row's own self symbol.
- SELF_SYMBOLS.tsv: includes current_line.
- Also CATALOG_CHANGES, CHECKS, UNITS, TABLE_CHANGES (empty), head/payload.json, orig/, rows/, tests/ and logs/.

The acc_s48 rows are not here. Their locks were already released (lock.sh: NOT_OWNER ... released).

## 1. Probes on the pilot 0x00D032C0 / owner 0x00D03160 (with DECL, one run)

The log is `evidence/probes/grade_r4p.log`. The owner graded RELOCATION_MATCH in every case.

| flags | ??_G 0x00D032C0 |
|---|---|
| (a) `/d2inlS20`, no /Og | **RELOCATION_MATCH** |
| (b) `/Og /d2inlS17`, `/Og /d2inlS18`, `/Og /d2inlS19` | DIFFER 30v48 each |
| (c) `/d2inlS17`, `/d2inlS18`, `/d2inlS19`, no /Og | DIFFER 30v48 each |
| `/d2inlS30`, `/d2inlS40`, no /Og | RELOCATION_MATCH |

Round 3 also showed that `/Og /d2inlS10..16` fails. So the smallest N is 20, with or without /Og.

## 2. Device: `/d2inlS20` (no /Og) + FABLE_RETAIL_CSPAWNEDFUNCBASE_DTOR_DECL_ONLY

Since /Og was dropped, I reran the full 120-row sweep at `/d2inlS20` without /Og (`evidence/probes/grade_s20_00.log` and `grade_s20_01.log`; the args files are in evidence/). It gives 85 pairs where both sides match and 35 rows where the thin row differs and the owner matches. That is exactly the round-3 split, row for row.

A control in the same run graded 0x00D032C0 at `/d2inlS20` without DECL: DIFFER 30v48, so DECL is still needed. It is the **last** 0x00D032C0 line of grade_s20_00.log (args line 121, `g_0.cpp`); the first line is the real row.

**Retraction of round 3:** round 3 said /Og is needed. It is not: `/d2inlS<N>` takes effect without /Og, and N 17-19 fail and 20 matches either way. **Gotcha candidate:** this contradicts the wording of GOTCHAS line 374, which says the heuristic inliner (and so /d2inlS) runs only with /Og on the command line.

All 85 owners and 85 thin rows are respelled from `/Og /d2inlS30` to `// Flags: /d2inlS20`. No `// Flags:` line and no hand-back table still carries /Og or d2inlS30; /Og appears only in the marker's sweep evidence. The respell was done by `tools/respell4.py`.

## 3. Marker on each owner

The owner header now runs:
- the `// Leaf:` line;
- `// Flags: /d2inlS20`;
- `// Inline-budget: /d2inlS20 + FABLE_RETAIL_CSPAWNEDFUNCBASE_DTOR_DECL_ONLY -- implicit ~CActiveEntityScriptBase (~CScriptThing +0x48, ~CCountedPointer +0x34, call ~CSpawnedFuncBase) expanded in this TU's ??_G as retail does; sweep on 0x00D032C0: /d2inlS17-19 fail, 20-40 match (with or without /Og; without DECL_ONLY 30v48), 85/120 CActiveEntityScript<T> pairs match at /d2inlS20`;
- then two `// NF424 r4` lines, which replace the six r3 lines and keep the sentence "The flag emulates an unidentified build setting of the retail script TUs."

The `#define FABLE_RETAIL_CSPAWNEDFUNCBASE_DTOR_DECL_ONLY` stays where it was, before `#include "fable_compiler.h"`. Each thin row has `// Flags: /d2inlS20` followed by a short `// Inline-budget:` line that points to its owner's marker. EXTRA_RULES says to mark every row that uses the device, and some landed /d2inlS20 rows do lack a marker. No checker for the marker exists in tools/. The thin-row markers were added after the first regrade, so all 170 rows were regraded again with the final sources (section 6).

## 4. CATALOG_CHANGES

- CompilerFlags is `(none)` -> `/O2 /Oy /W3 /d2inlS20` on both rows of all 85 pairs (170 rows).
- **No CompilerFlags row has an `old` other than `(none)`.** I checked the catalog field for all 170 rows and the `// Flags:` lines of every HEAD source: there are none. So no /Gr, /d2inlT or /EHsc was replaced or lost.
- The name and Module placeholder fixes from round 3 are kept, including 0x00D30000's name `CVtable_012cc724::...` -> `CActiveEntityScript<NScript::CQ_EndGameBossBattleScript>::`scalar deleting destructor'`.

## 5. 0x00E960E0: no move to hold needed

My round-3 README misread `pk_cmp.tsv`. Its columns are **new-object NEQ count / HEAD-object NEQ count / newly introduced NEQs**, not EQ copies.

- **What the round-3 numbers meant.** In 84 objects the "1" was HEAD's NEQ copy of `??1CSpawnedFuncBase@@UAE@XZ` (0x00CDD4C0), which leaves the new object because of DECL. Pick-any, NOADDR in our build.
- **0x00E960E0.** Its HEAD owner, 0x00E95F80, is a larger TU spelling: the CV_HauntedHouseScript header set with 15 opt-ins. That copy already graded EQ there, so its HEAD NEQ count was 0.
- **Its new object** has **3 EQ copies** and 0 NEQ:
  - GetEntityScriptName 0x00E953D0,
  - ~CCountedPointer<CEntityScriptBase> 0x00CE1000,
  - its owner's leaf, 0x00E95F80.

The round-4 pk results (`evidence/pk_cmp4.tsv`, `evidence/pk/R4N_*`) were run on the vland-graded thin-row objects. Its HEAD columns come from the round-3 HEAD pk files, now copied to `evidence/pk_head/R3H_*`.
- 0 NEQ in all 85 objects.
- 84 have 1 EQ copy; 0x00E960E0 has 3.
- Totals: 87 EQ, 933 NOADDR.

## 6. Regrade (vland HEAD v new, `logs/vland_lines.txt`, CHECKS.tsv)

The final sources (with both markers) were regraded in full.

Each owner edit was checked against the current repo source: once the four added header lines and the DECL define are removed, all 85 owners are identical to HEAD, so nothing landed since is reverted. The HEAD thin-row sources are unchanged as well.


- All 170 rows are RELOCATION_MATCH/PASS on both sides. The new payload carries per-entry `flags: ['/d2inlS20']`.
- **Warns:**
  - none added except one STANDIN_LEAF on 0x00D30000, which the catalog name fix covers;
  - no RELOC_NAME;
  - no "matches no symbol".
- **Callee binding:** OK on all 85 pairs.
- **Object diff, HEAD v new owner** (`tools/objdiff4.py`, `evidence/aes_gate4.tsv`), identical for all 85 and identical to round 3:
  - gone: `??1CSpawnedFuncBase@@UAE@XZ` and `??1?$CCountedPointer@$$CBVCPredicateBase@NScript@@@@QAE@XZ` (unreferenced NOADDR);
  - new: none;
  - changed: `??1CActiveEntityScriptBase@@UAE@XZ` (unbound) and the thin row's ??_G;
  - the owner's own leaf is unchanged.
- **Identity, owner, census and second storer:** unchanged from round 3, since they do not depend on flags (V+P+C, stores=1).

## Locks

NF424 holds the 170 rows (lock.sh: 170 OK). The holds and acc_s48 are released.
