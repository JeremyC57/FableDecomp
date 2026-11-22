# Lessons from Codex pass 28: why 21 long rows were stuck and how they came unstuck

**Inputs.**
- Our 25 pre-Codex drafts, listed in `pending/2026-11-22_codex_pass28/rows.tsv`.
- Codex's hand-back in `batches/2026-11-22_landing_p28/codex/` (the committed subset of the hand-back).
- The package we sent, in the pass 28 package (scratch; rebuilt by `pending/2026-11-22_codex_pass28/build.py`).

**Outputs** (committed beside the landing, `batches/2026-11-22_landing_p28/lessons/`).
- `PER_ROW.md` and `PER_ROW.tsv` give the per-row analysis.
- `CAUSE_MATRIX.tsv` holds one line per (row, cause) pair. Every count below is computed from it by `_work/count_causes.py` (scratch analysis, not committed); the output is in `_work/cause_counts.tsv` (scratch analysis, not committed).

**Evidence marks.**
- **P**: a probe shows the byte effect. The hand-back has no probe sources, so a P attribution rests on the probe tag and its PROBES.md description, not on a source diff.
- **V**: I verified it against our draft.
- **N**: Codex's notes say so, and I did not verify it.
- **C**: correlate only; no probe isolates it.

## Ranked lessons (causal)

| # | Lesson | Rows | Evidence | Row list |
|---|---|---:|---|---|
| 1 | Rebuild the statement structure from the debug records. Use every local the source had, in its scope block and with its type: loop-control flags, a single shared local where we wrote two copies, separate branches the compiler merges, function-scope versus nested lifetime. The records are `egod_locals.txt`, `egod_lines.txt` and the FableWin/Ego_r procedure records. | 16 | 12 P, 4 N | 00491ad2 00559a90 00560d00 006c6040 0078ecd0 007ebea0 008101f0 008664f0 008e8060 00b19c60 00d43db0 00dc08c0 00dfd600 00e03f70 00e49020 00ec32f0 |
| 2 | A residue we described as compiler behaviour was a source-shape problem in every case: register colouring, slot packing, "VC7.1 peels a compare", inline budget, cross-jump, "lane gap, none in source". Treat such a diagnosis as a hypothesis to falsify, never as a parking reason. In 00dfd600 and 00e49020 the final still needs an inline device (`/d2inlT70`, per-exit `inline_depth(0)`), so there our inline diagnosis was incomplete rather than wrong: the structural fix had to come first. | 13 | V (our own notes) | 00491ad2 00559a90 006c6040 0078ecd0 007ebea0 008664f0 008e8060 00b19c60 00bbe090 00dfd600 00e03f70 00e49020 00ec32f0 |
| 3 | Toggle callee-body visibility. Make each callee's real body visible, inline or out of line, or make it declaration-only. This changes escape analysis, frame size, slot reuse and register homes even when the call stays out of line. Both directions occur: 00ec32f0 needed declaration-only. Then fix the emitted copies' owner settings so pick-any passes. | 11 | 5 P, 6 N | 00491ad2 00559a90 0078ecd0 008664f0 008e8060 00b19c60 00be6880 00d43db0 00e03f70 00e49020 00ec32f0 |
| 4 | Call exactly what the source called, not an equivalent. Examples: the wrapper `IsDead`, not `!IsAlive`; the accessor `GetVector`/`SetVector`, not a member copy; `*ParentThing`, not `GetParentThing()`; `GetPreviousSelected()`, not the global; the helper with a constant argument, `SetupBlendOpState(ADD)`, not its folded effect; the SDK helper `InitFont2DVertex`, not an invented `MakeVertex`; the named vtable slot (`ResetRHSet`), not a placeholder `gNN()`. | 10 | 3 P, 4 V, 3 N | 00559a90 00560d00 006c6040 008101f0 008664f0 00a87960 00ab7c20 00b19c60 00bbe090 00e03f70 |
| 5 | Sweep expression shape late, as the last step. Parenthesise each float sub-expression or cast in turn, re-evaluate a repeated sub-expression instead of naming it, and pass literals to `const CCharString&` parameters implicitly rather than as `CCharString("...")` temporaries. This was the final flipping probe in 5 rows. | 7 | 5 P, 2 N | 00491ad2 00647ab4 008101f0 008e8060 00ab7c20 00b19c60 00be6880 |
| 6 | Finish the function before calling it stuck. 3 of the 7 were stubs of 115 B or less: the head draft 00647ab4 (87 B), the skeleton 00a87960 (115 B) and the prefix 00be6880 (89 B). One, 00d43db0, was never hand-edited ("NOT_REACHED"). The other 3 were single first drafts: 00560d00, 008101f0 and 00ab7c20. Codex matched all 7. | 7 | V | 00560d00 00647ab4 008101f0 00a87960 00ab7c20 00be6880 00d43db0 |
| 7 | Prove the mangled leaf and every callee's PDB signature: name, class, const, parameter types and order, return type. Return by value where the source does; a hidden result slot is not an out-pointer. A wrong row name also stops the package builder finding any PDB record (00560d00 shipped with none). | 5 + 3 | 4 V, 4 N | identity/signature: 00560d00 00647ab4 00ab7c20 00e03f70 00e49020; by-value: 008664f0 00ab7c20 00be6880 |
| 8 | A pick-any NEQ can be a wrong binding line. Check `reloc_bindings.tsv` against two sources before reshaping code for it. GetHomeBuilding was bound to an event getter; a placeholder class name hid a vector copy constructor. | 4 | N | 00491ad2 006c6040 007ebea0 00dfd600 |
| 9 | Copy the TU flags from same-TU neighbours (`/Gr`, `/GS`). A per-row `/d2inlT<N>` is a legitimate device after the structure is right. | 3 | 1 V, 2 N | 00560d00 006c6040 00dfd600 |
| 10 | Write behaviour tests with a distinct value for every argument and a multi-line or multi-element case. Length and shape do not catch a duplicated or wrong argument: 00560d00 hashed the user name twice, 00491ad2 took the highlight's Y offset from the wrong extents, and 008664f0 called the wrong virtual (per Codex). | 3 | 2 V, 1 N | 00491ad2 00560d00 008664f0 |
| 11 | When length and every instruction align but bytes still differ, compare every cleanup and termination branch target with retail (TERMINATION_PATHS.tsv). A wrong cleanup block means a wrong scope exit. | 1 | P | 00dfd600 |
| 12 | Our q.py ignored the `// Flags:` line, so /GS rows graded without their cookie unless passed by hand. This is our harness, and our own note records it. | 1 | V | 007ebea0 |

## Process finding (not a cause of being stuck)

- **Re-grade the draft on today's headers before trusting its recorded grade.** The grade in rows.tsv was not the grade of the file it points to in 10 of 25 rows, 8 of them among the 21 matched: 00647ab4 006c6040 0078ecd0 008664f0 008e8060 00be6880 00dfd600 00e49020. The unmatched two are 00850a80 and 00879900.
  - The worst case was 0x008E8060: recorded DIFFER(5v1784), but the draft is 1758v1784. Our brief therefore told Codex it was a stub.
  - Two more drafts (00879900 and 00a2f740, both from 2026-11-16) did not compile on the package headers at all.
  - The table is `_work/grades.tsv` (scratch analysis, not committed).

## Correlates (always present, byte effect not isolated)

- **Row-local models instead of shared headers: 20 of 21.** Every pre-Codex matched draft except 00559a90 defined local struct/class bodies, from 1 to 29 types. Codex's finals define none. This is required for landing anyway. As a byte cause it is mixed:
  - 006c6040: the `shared` probe left the bytes unchanged (2809).
  - In several rows the first shared-header probe moved away from retail before the other fixes: 0078ecd0 2032 to 1878, 008101f0 1608 to 1554, 008e8060 1758 to 1699, 00e03f70 1986 to 1747.
- **Header additions were needed: 17 of 21.** These rows needed gated header patches, such as a new class header or a missing inline body or member (`batches/2026-11-22_landing_p28/codex/hdr/PATCHES.json`). The four that did not are 00dc08c0, 00dfd600, 00e03f70 and 00e49020.
- **NScript generator form: 5 of 5 script rows.** The rows are 00d43db0, 00dc08c0, 00dfd600, 00e03f70 and 00e49020.
  - The form is `TRetVoid _Fail_Val;`, `return _Fail_Val.Return();` and `for (;; Game->NewScriptFrame())`.
  - It is in every Codex final and in none of our drafts. Our GOTCHAS gained this rule on 2026-11-12, after all five drafts (2026-10-31 to 2026-11-08) were written. None was re-tried with it.
  - Codex still used `inline_depth(0)` at some exits in 4 of the 5, but only after the structure was right.
- **Debug-record locals: about 0 in our drafts, all in Codex's.** Our drafts used 0-5 of the row's debug-record local names, mostly incidental `i`/`it`; Codex used all or nearly all (`_work/local_name_usage.txt` (scratch analysis, not committed)). Our draft folders are dated 2026-10-23 to 2026-11-08. The package's per-row Ego_d extracts were made for pass 28, but Ego_d was used in places from 2026-10-31 (FINDINGS_LOG line 980). So I cannot say these drafts lacked access, only that they did not use it.

## Where Codex's evidence came from

| Kind | Can our agents do it today? | Rows that depended on it |
|---|---|---|
| Package PDB records: Ego_d locals, scopes and lines; Ego_r/FableWin signatures, locals and indirect call-site records | Yes | most rows |
| Facts that need donor (debug-build) code: offsets inside debug functions, direct-call counts, call order, loop shape | **No.** FINDINGS_LOG 2026-11-19 says FableWin.exe, Ego_d.exe and Ego_r.exe are not on our lane, and the package `fablewin_proc.txt` has only indirect-call records. | See below |
| Native probes and audits | Yes; tooling partly exists | all rows |

Donor-code facts by row:
- **00560d00**: "the donor calls GetSignedInPassword for the second MD5 at +0x359".
- **008101f0**: "FableWin 0266AF75 delegates to the fade member".
- **008664f0**: "donor IsNonZero reads Y then X then Z"; the cheat-byte conditional.
- **00dc08c0**: "FableWin shows a loop with a frame-advance increment and an explicit termination return".
- **00dfd600**: "FableWin call sequence corroborates ...".
- **00e03f70**: "four direct calls to CScriptThing::IsDead".
- **00ec32f0**: "FableWin shows the returned string copy first".
- **00d07490** (unmatched): "FableWin constructs the predicate as an unnamed temporary".
- **Unclear source** ("the donor ..." with no record named): 00ab7c20 (InitFont2DVertex call) and 00bbe090 (the two helper calls in the displacement arm).

So **7 matched rows (00560d00 008101f0 008664f0 00dc08c0 00dfd600 00e03f70 00ec32f0) cite at least one fact we cannot reproduce**, and 2 more are unclear. In each of these the decisive probe was still a source change we could have tried blind, such as a scope exit, a wrapper call or a lifetime. The donor fact told Codex which change to try first. Getting the donor executables onto the lane, which is the owner action in FINDINGS_LOG 2026-11-19, would make those checklist items direct.

## Authoring checklist (run before declaring a long function stuck)

Items that need donor code are marked **[donor]**; the rest work with what we have.

1. **Regrade the draft.** Compile the draft on today's headers and record that grade. Do not trust the record: 10 of 25 records differed from their file.
2. **Check the draft is complete.** Is every block written? A head, prefix, skeleton or "approximate" block is new work, not a residue.
3. **Prove the identity.** Get the full mangled `// Leaf:` from Ego_r/FableWin name sets, never a bare name. Confirm class, const, parameters and return type. If the row name was wrong, re-extract the PDB records under the right name.
4. **Copy the flags.** Take `// Flags:` from same-TU neighbours (`landed_sources.tsv`): `/Gr`, `/GS`, `/Os` pragmas. Make sure the grader actually applies them (q.py did not read the line).
5. **No local types.** Every class must come from `include/engine/<Class>.h`. Add missing members or bodies as gated header patches.
6. **Take every local from the debug records.** Use `egod_locals.txt` and FableWin S_BPREL32 for name, type and `{}` scope, and put each one in that scope. Look for:
   - a flag local that controls a loop (`new_lines`);
   - one shared local where you wrote two copies (`inform_buildings_villagers`);
   - a function-scope object you nested (RockTroll, the vector in 00ec32f0).
7. **Check line sizes.** Read `egod_lines.txt` for statement count and size per line. A line that is a bare call means a helper call: write the call, do not hand-fold it.
8. **Check every callee signature.** Look up the PDB signature of each callee (return type, parameter order, by-value versus reference) and the real vtable slot name. Never call a placeholder `gNN()` slot.
9. **Use the wrappers and accessors the source used.** For example `IsDead`, `GetVector`/`SetVector`, `*ParentThing`, static list accessors, SDK/STLport helpers. Pass string literals implicitly to `const CCharString&`.
10. **Write branches as the source did.** Use the source's else-if order and nesting, including where a negated test sits (007ebea0's `else if (!UseableByHero)` arm, not a trailing `else`). Write identical-looking arms out and let VC7.1 cross-jump them. Do not duplicate code the source shared: 006c6040 had one village walk fed by both arms.
11. **Use the generator form for NScript rows:** `TRetVoid _Fail_Val`, `for (;; Game->NewScriptFrame())`, a termination check per statement and `return _Fail_Val.Return()`. Only then add per-exit `inline_depth(0)`.
12. **If only the frame or registers differ, toggle callee-body visibility** (visible inline / visible noinline / declaration-only) one callee at a time. Then re-check the emitted copies' owner settings with pick-any.
13. **If size and instructions align but bytes do not:**
    - list every branch target that leads to cleanup or termination, ours against retail;
    - check jump-table entries;
    - fix the scope exit that sends a path to the wrong block.
14. **Then sweep expression shape.** Parenthesise each float term or cast, re-evaluate a repeated term instead of naming it, and remove helper locals the debug record does not have. Log each variant.
15. **Do not trust the aligned-instruction count across switch tables.** It mis-counts table data as code (00560d00, 00e49020, 00ec32f0 match at 536/550, 630/635, 553/568). Decide on masked bytes plus the relocation audit.
16. **Run the operand and pick-any audit.** Before reshaping code for a NEQ copy, check its binding line against two sources, then hand back a corrected binding.
17. **Write the behaviour test** with distinct values for every argument and a case for every branch. Run it before shape work, so semantic errors such as a duplicated hash argument surface early.
18. **Do not park on a compiler-artefact diagnosis.** If the residue note says register colouring, slot sharing, inline budget, cross-jump or lane gap, items 6 to 14 have not all been tried. Park only after at least 5 genuinely different source shapes, each logged, with the residue still not shrinking.
19. **[donor]** Read the debug-build code for the call order, direct-call counts and the loop/return shape of the function and its caller. Codex used this in 7 matched rows; we cannot until the donor executables are on the lane.
20. **Re-try old drafts after a new rule lands.** When a GOTCHAS or ASM rule is added (TRetVoid 2026-11-12, Ego_d extracts), re-grade the parked drafts it applies to. None of the five script drafts was re-tried after the TRetVoid rule.

## What Codex's process did that ours did not

1. **It used facts from debug-build code** (see the table above). This is the one thing we cannot copy today.
2. **It ran many more probes.** That was 1,937 probes on the 21 matched rows: median 74 per row, range 27 to 300. The first masked-equal came at a median of probe 48, mean 71. Our notes record roughly 1 to 15 lettered variants per row: 00491ad2 a-j, 006c6040 i1-i11, 007ebea0 a-k, about 10 in 00dfd600. Several rows have only one graded draft.
3. **It ran its probes in named families,** for example 22 `round2_subparen_*` variants of one expression. Every line in `PROBE_HISTORY.tsv` records a source hash and a token-shape hash, so repeated shapes are visible. Aligned-instruction count was its fitness signal, kept separate from the byte grade.
4. **It started from a baseline regrade.** It regraded the received draft on the package headers first; that is the `baseline` line of each row.
5. **It produced a fixed set of evidence files per row.** All 25 rows got:
   - `OPERANDS.tsv` and `OPERAND_SUMMARY.json`: every relocated operand bound at its exact retail site (BINDING_AT_EXACT_SITE / LITERAL_AT_EXACT_SITE).
   - `PICKANY.txt` and `PICKANY_OPERANDS.tsv` (23 rows), plus `PICKANY_CORRECTED.txt` and `BINDING_EVIDENCE.json` where the table was wrong.
   - `STRONG_COPIES.tsv`, plus `STRONG_COPY_OPERANDS.tsv`: extra strong owners verified at their own retail addresses.
   - `JUMP_TABLE(S).tsv` (3 rows) and `TERMINATION_PATHS.tsv` (00dfd600).
   - `RESULT.json` (include-snapshot digest, flags, byte status, hunks) and `TEST.json`.
   - `NAME_FIXES.tsv` (17 rows).
   - `PROBES.md`, with what each probe changed.
6. **It verified extra owners.** Same-TU helpers it emitted were checked as whole extra functions at their retail addresses: GetSubDef<CTavernDef>, the marker search at 0x008E8AC0, GetComponent at 0x00867F60, SetTargetVertexNormal, TU-mates of 00560d00 and a scalar deleting destructor.
7. **It ran large behaviour fixtures,** from 14 scenarios up to 4,608 cases per row (00b19c60). They found a real semantic bug (00560d00) that length did not.
8. **It ablated every inline device.** Each `// Inline-budget:` line has probes showing that plain inline, `/d2inlT` and FORCEINLINE variants fail (00bbe090, 00ec32f0, 00e49020).
9. **It proved its header patches safe.** All patches are gated, with an equivalence check over 3,629 landed TUs and 3,132 tests that reported 0 changed preprocessing entries (`hdr/EQUIVALENCE.md`).

## The four rows Codex did not match

- **0x00850A80 CContextSensitiveItemDef::IsValidOverride.**
  - Codex's final is 2,608 B against 2,576 B retail, 32 B long, at 604/852 aligned, after 100 probes.
  - All 13 mapped copies and both indexed jump tables (14 entries) agree.
  - Codex names the remaining residue as frame packing, conditional cleanup merging, the x87 digging-distance expressions, the early required-component paths, register assignment and cleanup exits.
  - Our draft had 35 local types. Codex added 11 gated header patches.
- **0x00879900 NCameraModes::CProjectileWeapon::Update.**
  - The received draft did not compile on the package includes: its acctC header overlay was never ported into `include/`.
  - Codex restored the projectile, profile and ranged-attack owners and proved the static `InvertY` at 0x013BAF98, but stopped at 3,430 B against 3,415 B after 225 probes.
  - The lead residue is three extra 12-byte lock-on vector slots.
- **0x00A2F740 C3DMeshStats::InitialiseFromFileData.** It also started COMPILE_FAIL for the same overlay reason.
  - Codex reached 3,017 B against 3,051 B (34 B short) after 60 probes, with all 27 mapped copies equal.
  - GetRoot (101 B), AllocateBuffers (183 B), PackNames (502 B) and the vector<char> insertion (218 B) match as separate owners.
  - Frame offsets and access scheduling remain. `/d2inlT` budgets from 35 to 110 swing the size between 2,169 and 3,431 B without converging.
- **0x00D07490 CQ_BanditCampBossBattleScript::CGate3Guard::Main.**
  - This row had the most probes, 324, and no Ego_d record. Codex ended at 2,190 B against 2,182 B.
  - Its notes list predicate-destructor expansion at entry, the timer index register home, and the conditional string and movie-release cleanup split.
  - Nearer builds of 2,178 B and 2,186 B were rejected because a secondary owner went unequal; Codex chose verified owners over length.

In all four the blocker is a frame, register or cleanup-merge residue that remained after the structural evidence ran out. Two of them (00879900, 00A2F740) also lost their head start, because the hand-off's header overlay did not compile.
