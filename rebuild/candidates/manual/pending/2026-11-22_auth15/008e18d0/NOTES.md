# 0x008E18D0 NCreatureMode::CModeThunderLightningStorm::UpdateLightningBeams (2073 B): landing notes (AUTH15, 2026-11-22)

S = /tmp/claude-0/-home-user-FableDecomp/5221bbec-fb18-575f-8649-a32d96bb1c2d/scratchpad; W = S/w22/auth15/008e18d0.
Source `008e18d0.cpp`. It is AUTH14's e5n with comment-only edits; its non-comment lines are byte-identical to e5n.

## Breaks for the coordinator to decide
- **0x007BE3E0 and 0x007BE4E0 (InitAttachedEffects for ForcePushBall and DrainLife) break the pick-any rule (nf201) when the opt-in is defined.** No change was made to either.
  - Their leaf still grades RELOCATION_MATCH with the define (G3).
  - Both TUs are size-optimised (`#pragma optimize("y", off)` + `("s", on)`). With an implicit destructor they emit their own `??1?$CParticleUpdateParameters@KK@NParticleEngine@@UAE@XZ` COMDAT.
  - That copy is 33 B (`call ~vector<pair<ulong,ParamDesc>>`, `free`, `jmp ~CBaseClass`). Retail's 0x006E2500 is 73 B, with the inner ~vector expanded. The masked compare has 20 differing bytes (`dtor_comdat_check.txt`).
  - On the default path (G3c control, once each) they emit no body, only the external reference, so they pass today.
- **Every other live binder** (12 of the 14 live rows, plus this row) emits a dtor COMDAT byte-equal to 0x006E2500 with relocations masked (73 B, 0 diffs). Relocation targets are not checked by vsretail.py, except in this row: leafrel, below.
- **So "implicit everywhere" holds for the bytes of all 23 rows' leaves, and for the COMDATs of every live TU except those two size-optimised ones.**
  - Making the opt-in the default would need a per-TU answer for those two.
  - Possibly the -Os copy is simply the one the linker discarded. Under nf201 that is still a mismatch.
  - This is the coordinator's decision.
- **Proposed comment change for the coordinator (hdr/ left as proven):** the opt-in comment says the COMDAT "compares SAME with retail 0x006E2500". That holds in this TU and in 12 of the 14 live binders, but not in a size-optimised (`optimize("s",on)`) TU. Suggested added line: `// Not in an optimize("s") TU: there the implicit copy calls the inner ~vector (33 B; rows 0x007BE3E0 / 0x007BE4E0).`

## Identity
- Leaf `?UpdateLightningBeams@CModeThunderLightningStorm@NCreatureMode@@AAEXXZ`, private, thiscall.
- **Ego_r.pdb:** 0x00823B70, release\creature_modes.obj #186, 2310 B (ghidra_out/egor_obj_functions.tsv).
- **FableWin.pdb:** 0x0285AA60, debug\creature_modes.obj #230, 3407 B (fablewin_obj_functions.tsv).
- **Ego_d.pdb:** its line table has the same 3407 B, with the same locals and blocks (MEASURE.md).

## Flags: `/Gr`
- **Ego_d.pdb:** creature_modes.obj's carved S_ENVBLOCK (byte offset 0x1DA9CB0) has `... -Od -Oy- ... -GS- -Gy ... -GR ... -Gr -TP`. See `egod_compile_line.txt`.
- **FableWin.pdb:** module 212 `debug\creature_modes.obj` has the same command line (`egod_compile_line_raw.txt`). That is the odall10 fablelib preset.
- **Retail side:** in this TU the STL helpers 0x0076ADA0, 0x00768FC0 and 0x00769000 compare SAME only under `/Gr` (COORDINATION_LOG, 2026-11-21 B->A).
- **TU-mates:**
  - **0x008DD7E0 UpdateDamage:** landed with no flags (no `// Flags:` line, no CompilerFlags). Graded once at `/Gr`, repo source, no overlay (G2): **RELOCATION_MATCH**. So `/Gr` is consistent with that mate; it was not changed.
  - **The other mates in creature_modes.obj, for information only:**
    - 0x008DDAD0 CreateLightningBeams is landed at `/O2 /Oy /W3 /d2inlT65`.
    - 0x008E16A0, 0x008E20F0 and 0x008E24F0 are landed flagless.
    - Those three match with the opt-in defined (G3 below).
    - None of these four was regraded at `/Gr`.

## The IMPLICIT_DTOR opt-in (`hdr/`, `hdr.diff`; add-only)
- **The opt-in:** `FABLE_RETAIL_CPARTICLEUPDATEPARAMETERS_IMPLICIT_DTOR` in engine/NParticleEngine_CParticleUpdateParameters.h. With it defined there is no user-declared destructor. The default path is unchanged.
- **PDB evidence** (`pdb_dtor_evidence.txt`): `~CParticleUpdateParameters<unsigned long,unsigned long>` is `public virtual compiler-generated` in all three PDBs:
  - Ego_d, field list 0x232B4;
  - FableWin, 0x232B4;
  - Ego_r, 0x213ED.
- **So the name stands:** IMPLICIT_DTOR, the repo's convention for a compiler-generated destructor. Only the comment was rewritten, to cite the evidence.
- **Observation only, not acted on:** the same three PDBs also mark both constructors compiler-generated (method lists 0x232B0 and 0x213E9). They also mark `operator=` that way. The header's default path declares `CParticleUpdateParameters() { }`.
- **The overlay holds only this header.** AUTH11's CThing.h GETTC_EGOD_WRAPPER (C5) is left out: it changes no bytes (AUTH14: e5.q.out == e5n.q.out). The row uses the repo's `FABLE_RETAIL_CTHING_GETTC_EXPANDED` arm.
- **Regrade of every landed row that binds 0x006E2500:** retail has 28 call sites (xref.py).
  - 22 are in 22 landed rows, one each. One is this row (0x008E1C3A). Five are in unlanded functions.
  - The 22 rows plus the owner row 0x006E2500 make 23 grades.
  - Each was graded once, untouched, at its landed flags plus `/DFABLE_RETAIL_CPARTICLEUPDATEPARAMETERS_IMPLICIT_DTOR`, with the overlay (`grades.txt`, `binders_class.tsv`).
  - "live" means the define changes the row's preprocessed text. "no-op" means a local model, so the text is unchanged.
  - The call sites 0x006E25B3, 0x007C6436, 0x0083991E, 0x008F7BB7 and 0x008F7E5A lie in unlanded functions (`dtor_callers.tsv`).

| row | class | landed flags | grade with the define | dtor COMDAT v 0x006E2500 |
|---|---|---|---|---|
| 006e2500 | owner | none | RELOCATION_MATCH | owner row |
| 006e2550 | no-op | none | RELOCATION_MATCH | n/a (no-op: local model, text unchanged) |
| 006e26c0 | no-op | none | RELOCATION_MATCH | n/a (no-op: local model, text unchanged) |
| 006e26f0 | no-op | none | RELOCATION_MATCH | n/a (no-op: local model, text unchanged) |
| 00767da0 | no-op | none | RELOCATION_MATCH | n/a (no-op: local model, text unchanged) |
| 007a1380 | live | none | RELOCATION_MATCH | 73 B, 0 diffs |
| 007a1b20 | live | none | RELOCATION_MATCH | 73 B, 0 diffs |
| 007be3e0 | live | none | RELOCATION_MATCH | **33 B, 20 diffs (NEQ)**; default path: no body |
| 007be4e0 | live | none | RELOCATION_MATCH | **33 B, 20 diffs (NEQ)**; default path: no body |
| 007c2480 | live | none | RELOCATION_MATCH | 73 B, 0 diffs |
| 007c25a0 | live | none | RELOCATION_MATCH | 73 B, 0 diffs |
| 007f2dc0 | live | none | RELOCATION_MATCH | 73 B, 0 diffs |
| 008548b0 | live | none | RELOCATION_MATCH | 73 B, 0 diffs |
| 00854b90 | no-op | none | RELOCATION_MATCH | n/a (no-op: local model, text unchanged) |
| 008e16a0 | live | none | RELOCATION_MATCH | 73 B, 0 diffs |
| 008e20f0 | live | none | RELOCATION_MATCH | 73 B, 0 diffs |
| 008e24f0 | live | none | RELOCATION_MATCH | 73 B, 0 diffs |
| 008f7950 | live | /Gr | RELOCATION_MATCH | 73 B, 0 diffs |
| 008f7bd0 | no-op | /Gr | RELOCATION_MATCH | n/a (no-op: local model, text unchanged) |
| 008f7e70 | live | none | RELOCATION_MATCH | 73 B, 0 diffs |
| 008f7f50 | live | none | RELOCATION_MATCH | 73 B, 0 diffs |
| 00af10e0 | no-op | none | RELOCATION_MATCH | n/a (no-op: local model, text unchanged) |
| 00af5900 | no-op | none | RELOCATION_MATCH | n/a (no-op: local model, text unchanged) |

- **Owner row 0x006E2500:** its landed source is a local model with no user-declared destructor. So it already spells the destructor as compiler-generated, as the opt-in does.
- **No-op rows:** 0x006E26C0, 0x00767DA0, 0x00854B90, 0x008F7BD0, 0x00AF10E0 and 0x00AF5900 declare the destructor in a local model. 0x006E2550 and 0x006E26F0 use local models that do not include the shared header (0x006E26F0 binds it as `?Cleanup@Sub28@@QAEXXZ`).
- **The sibling 0x007A1380** CTCWillLightning::CLightningBeam::Activate is live: RELOCATION_MATCH, and its dtor COMDAT is byte-equal (relocations masked).
- **Scope:** only the binders were regraded. After this landing all 14 live rows stay on the declared-destructor (default) path; only this row defines the opt-in.
- **0x00767DA0:** a first pass flagged it as including the shared header. That was a false positive: its local model has the same member spelling. It is a local model (no-op), and binders_class.tsv is corrected.

## Corrections (C1..C4), each with its Ego_d evidence (AUTH11 MEASURE.md, VC10 odall10 fablelib)
- **C1, `GetParentThing()` for all six ParentThing reads** (Ego_d lines 3297 / 3298 / 3312 / 3313 / 3340 / 3341).
  - Each of those lines is +4 B in Ego_d: `call GetParentThing ; mov ecx,eax` against `mov ecx,[ecx+4]`.
  - Ego_d / FableWin have a GetParentThing proc, 17 B.
  - The repo opt-in is FABLE_RETAIL_CMODEBASE_PARENTTHING. The TU-mate 0x008DD7E0 uses it too.
- **C2, the storm rotation:** `float storm_rotation = StormRotation; long storm_rotation_int = (long)storm_rotation; storm_rotation -= storm_rotation_int;`
  - Ego_d 3288 / 3291 / 3292 = 15 / 11 / 9 B.
  - **Inferred:** the `-=` spelling. `storm_rotation = storm_rotation - storm_rotation_int` gives the same bytes.
- **C3, `CRightHandedSet rhs;` before `weapon_pos`:** Ego_d 3296 = 8 (the ctor), and 3297 = 27 (weapon_pos, 23 + 4 for C1).
- **C4, the VC6 for-scope idiom `if (false) {} else for (n = 0; ...)` on both loops:**
  - Ego_d 3302 / 3332 = 39 = 30 + 9 (xor / je / jmp).
  - The TU-mate 0x008DD7E0 has it too (FableWin `xor edx,edx ; je` at 0x0285A2AC).
  - **Inferred:** the exact idiom.
- **Result:** 47 of Ego_d's 54 lines are equal in size (MEASURE.md table, "corrected" column).

## Locals, compared per block (coordinator ruling 2026-11-22; `locals_per_block.txt`)
Ego_d's record (pending/2026-11-22_auth14/008e18d0/egod_locals.txt) against the source's declarations, block by block:

| scope | Ego_d | e5n | extra / missing |
|---|---|---|---|
| function | 12 named locals + `this` | the same 12 | none / none |
| block 1 (loop-1 if-body) | vtStartPos, end_pos, vtEndPos, beam_direction, upParams, ptc_particle_emitter | the same 6 | none / none |
| block 2 (loop-2 if-body) | end_pos, beam_direction, ptc_particle_emitter | the same 3 (no vtStartPos / vtEndPos / upParams) | none / none |
| block 3 (sky if-body) | vtStartPos, end_pos, vtEndPos, upParams, ptc_particle_emitter | the same 5 | none / none |

- **Types agree:** pair<unsigned long,C3DVector> is 0x300E6, CParticleUpdateParameters<ulong,ulong> is 0x232B5, C3DVector is 0x2460, CRightHandedSet is 0x9B55, and ptc is a pointer.
- No text change was needed.

## Remaining Ego_d differences (no correction proposed)
- **Lines 3279 and 3305, +3 B each: member offset.**
  - Ego_d's LightningBeamParticleEmitters is at +128, because its Dinkumware vectors are 20 B (TPI 0x2B26D). So its `add ecx` / member reads take a 32-bit displacement.
  - Retail's member is at +0x74, a disp8.
  - The members at +128 and above already agree (3330, 3334).
- **The four IsDebuggerPresent assert lines: 3298 (+489), 3316 (+481), 3344 (+502), 3356 (+503).**
  - FableWin's record has exactly four S_CALLSITEINFO `int ()` (IsDebuggerPresent) entries, at +0x322 / +0x61E / +0x9FA / +0xC4D, one inside each of these lines.
  - Each is a debug-only assert (a VERIFY-style macro whose release form is unknown). Retail keeps the call it wraps (GetDummyObject called, GetTC expanded).
  - One FableWin-style assert with inline asm measures 281 B; these are larger. Not modelled.
- **The prolog, line 3278 (+10 B): unexplained.**
  - Ego_d's frame is about 552 B (`this` at BP-552), well under a page, so it is not `__chkstk`.
  - It is not the EH frame or the `this` slot: both are present in ours.
  - It is not the inline-asm assert: the probe keeps 36.
  - The epilog is equal (14).
  - No text candidate.

## Checks
- **G1:** `q.py 008e18d0 008e18d0.cpp <leaf> --flags /Gr`, FABLE_HDR_OVERLAY = hdr/ (this header only): **RELOCATION_MATCH**. The final text, graded once.
- **D1:** vland dry run, LANDVERIFY_STRICT_HEADERS=1. The payload has the source, the test, the leaf, flags `["/Gr"]` and the pattern `008e18d0_TEST PASS` (`d1_payload.json`, `d1_vland.log`).
  - Result: **`008e18d0 RELOCATION_MATCH PASS`**, with no header-policy warning.
  - It matched at the base flags: G1's no-sweep grade of the same text and flags matched.
  - There are two `WARN RELOC_NAME` lines, for the std::sort COMDATs 0x0076C000 / 0x0076C1A0. The manifest labels them with demangled template text; the targets are right. Informational.
- **Pick-any readout for this row (not a grade):** auth9 `leafrel.py` on the G1 object: PROBLEMS 0. EQ: LowerBound 0x0040F020, _M_insert_overflow 0x0076ADA0, ~CParticleUpdateParameters 0x006E2500. It is identical to AUTH14's readout (`g1_leafrel.txt`).
- **Payload check:** d1_payload.json's source and test equal the handed files byte for byte. d1_oracle.tsv (retail bytes) was deleted; it regenerates from S/batch/auth1_oracle.tsv.
- **Behaviour test:** `008e18d0_test.cpp` (C++03, CRT headers only, self-contained test doubles, no engine class modelled). Its cases:
  - four beams with one dead and one missing, and two end beams with one dead (rotation 2.0);
  - a positive fractional rotation (3.25);
  - a negative rotation (-1.25, truncation toward zero);
  - loop 2 using 1 / num_beams;
  - the caster-height Z;
  - the START / END CRC keys, sorted by key;
  - no weapon-focus dummy;
  - sky beam alive (+15), dead and missing;
  - zero beams.
  - It PASSes under the lander (D1) and under host g++ -std=c++03.
  - Gap: loop 2 has a dead end beam but no missing (null) one. Loop 1 and the sky cover both.
- **Header proof:**
  - `pin_asserts.py rebuild/include hdr engine/NParticleEngine_CParticleUpdateParameters.h`: pinned 0. The header has no FABLE_STATIC_ASSERT.
  - `assert_names_check.py eq/old eq/new`: 0 mismatches.
  - `equiv_check.py eq/old/rebuild/include eq/new/rebuild/include`: 57,020 rows checked, **0 differ** (proof_equiv.tsv is empty).
  - So there is nothing to recheck with recheck_rows.py.
  - eq/old is a snapshot of the working tree's rebuild/include, which had uncommitted edits from another lane to unrelated headers. eq/new is the same snapshot plus this header only.
- **Grades used:** G1 1, G2 1, G3 23, G3c 2 (controls for the COMDAT breaks), D1 1. No variant, no re-fit.

## Files (W)
- **Source and test:** `008e18d0.cpp`, `008e18d0_test.cpp`, `MATCHES.tsv`, `STATUS.tsv`.
- **Overlay:** `hdr/` and `orig/`, `hdr.diff`.
- **Logs and grades:** `LOG.md`, `grades.txt`, `g/*.q.out`.
- **Header proof** (the eq/ include snapshots were deleted after use, for disk): `proof_pin_asserts.log`, `proof_assert_names_check.log`, `proof_equiv.tsv` and `.log`, `proof_recheck/`.
- **Dry run:** `d1_vland.log`.
- **Evidence:**
  - `pdb_dtor_evidence.txt`, `egod_compile_line.txt`, `egod_compile_line_raw.txt`, `locals_per_block.txt`;
  - `dtor_callers.tsv`, `binders.txt`, `binders_class.tsv`, `dtor_comdat_check.txt`, `g1_leafrel.txt`.
