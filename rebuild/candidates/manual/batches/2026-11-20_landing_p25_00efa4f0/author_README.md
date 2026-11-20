# 0x00EFA4F0 NScript::Cbandit_level_cutsceneScript::Main (p25F)

**Grade:** RELOCATION_MATCH (1362/1362) with `// Flags: /d2inlT36`.
- The strict vland run passes (LANDVERIFY_STRICT_HEADERS=1, empty overlay `hdr/`, fresh LANDVERIFY_WORK; `vland.log`), with no warnings.
- The real-object behaviour test passes.
- **Header changes:** none. `hdr/` and `hdr.diff` are empty, and newly_flagged_rows.tsv has the header line only.
- **NAME_FIXES:** none (header line only).

## What changed against the draft (pending/2026-11-07_linux_hand837 = 2026-11-13_codex_released, DIFFER(1365v1362))
- The draft included `engine/Cbandit_level_cutsceneScript.h`, which never reached the repo (only hand837's overlay has it), so the draft
  no longer compiled. The class is now modelled locally, the way the 12 landed rows of this TU do (ctor 0x00EFA420, Alloc 0x00EFAF30,
  Cwelcome_guy::Alloc 0x00EFAA60 ...). **Coordinator's choice:** hand837's header could be landed instead. That would newly flag those
  12 landed rows under strict headers, so this batch does not do it.
- **The device:** one per-row flag, `/d2inlT36` (ladder step 2). The draft's three header devices are all dropped:
  - FABLE_RETAIL_CWIDESCREENMAGICPAUSEENTITIES_FORCEINLINE_CTOR;
  - FABLE_RETAIL_CSCRIPTGAMERESOURCEOBJECTMOVIEBASE_FORCEINLINE_CTOR;
  - the declaration-only `CCountedPointer<CScriptGameResourceObjectMovieBase>::~CCountedPointer` specialization.
- **Names and locals from Ego_d:** the movie local is renamed `speech_movie`. `TRetVoid _Fail_Val` and `return _Fail_Val.Return();` are
  added. `bool use_preload = true; if (use_preload) {...}` is added round the preload block.
- **Header comments:** line 2 is now the `// Inline-budget:` label, a `// Flags:` line is added, and the stale Grade / Next-idea lines are gone.

## 1. Mechanism (c2 trace, `evidence/c2_trace_*.txt`)
Retail expands the implicit ~CScriptGameResourceObjectMovieBase at the first three movie-scope exits:
- the SPLINE_04 wait return;
- the post-SPLINE_04 check;
- the SPLINE_03 wait return.

Each expanded copy is `call 0x006E7AB0 ; GROBase vtable store ; call 0x0099A430`, at 0x00EFA9D0 / 0x00EFA9DD. Retail calls the COMDAT
0x006E7B80 at the other four sites: the post-SPLINE_03 check, the SPLINE_01 wait return, the post-SPLINE_01 check and the scope end.

The seven dtor sites sit at f = 0.625 / 0.3125 / 0.156 / 0.078 / 0.039 / 0.0195 / 0.0195.

| T | nested ~CCountedPointer (c 50, f 1.0, score 50) | movie dtor cost | site scores | expanded | grade |
|---|---|---|---|---|---|
| 74 (default) | expanded | 60 | 65 / 78 / 106 / 161 / 270 / 490 / 490 | 1 | DIFFER(1389v1362) |
| 36 | called (retail's call 0x006E7AB0 inside the expanded copies) | 19 | 20 / 24 / 33 / 51 / 85 / 155 / 155 | 3 | RELOCATION_MATCH |

- **Draft (with the declaration-only spec, default T):** dtor cost 19, scores 20 / 24 / 33 / 51 ..., so 4 sites expand: DIFFER(1365v1362).
  That fourth site, the post-SPLINE_03 check, was the draft's residue.
- **COMDAT evidence:** the TU's ~CCountedPointer<MovieBase> COMDAT is byte-equal to retail 0x006E7AB0 (53 B, relocations only).
- **Caveat:** the TU's movie-dtor COMDAT is 25 B (it calls 0x006E7AB0), while retail's 0x006E7B80 is 66 B with the release expanded. So
  retail's linker took that COMDAT from another TU, or this TU had another budget. A per-row flag cannot show which.

## 2. Inline-budget ablation (line 2, every device; `evidence/ablations.txt`, `evidence/flag_sweep_v3.txt`)

**Final source (v5 = src):**

| change from the matching source | grade |
|---|---|
| no flag | DIFFER(1389v1362) |
| /d2inlT35 | DIFFER(1346v1362): the CEntityScriptBinding ctor (score 35) is called |
| /d2inlT36, T38, T40, T43, T46, T48, T50 | RELOCATION_MATCH |
| /d2inlT51, /d2inlT52 | DIFFER(1300v1362): the nested ~CCountedPointer expands, so no site expands |

`/d2inlS` was graded on v3 (the draft's devices, no flag): S10 DIFFER(1365v1362), S15 / S20 DIFFER(1318v1362).
- **Chosen:** T36, the lowest N of the window, as in the landed 0x00D6C830 / 0x00D8A240.

**The draft's devices, each re-added singly to a matching source** (measured on v4 = v5 plus all three, at T36):

| device | removed from v4 at T36 | at the default T |
|---|---|---|
| FABLE_RETAIL_CWIDESCREENMAGICPAUSEENTITIES_FORCEINLINE_CTOR | RELOCATION_MATCH (neutral) | (v4 with all three, no flag: DIFFER(1365v1362)) |
| FABLE_RETAIL_CSCRIPTGAMERESOURCEOBJECTMOVIEBASE_FORCEINLINE_CTOR | RELOCATION_MATCH (neutral) | |
| declaration-only ~CCountedPointer<MovieBase> spec | RELOCATION_MATCH (neutral) | |

- **v4's own window:** T35 DIFFER(1346v1362), T36 and T51 RELOCATION_MATCH, T52 DIFFER(1365v1362). The v3 sweep (the same text on hand837's header) matches at every value from T36 to T51.
- **So:** no device is needed besides the flag, and all three are dropped.

## 3. Byte-neutral Ego_d constructs (coordinator-overridable)
Each was removed from the final source in turn and graded at T36.

| construct | grade without it | evidence |
|---|---|---|
| `bool use_preload = true;` + `if (use_preload) {...}` round the five preload points | RELOCATION_MATCH (neutral) | Ego_d local `use_preload` in the loop-body block. Ego_d line 214 (4 B) = our `= true` (4 B) and line 219 (12 B) = our `if` (12 B), both exact under VC7.1 /Od /EHsc (`egod_lines/od_calibration_row.txt`). Only these two lines calibrate exactly: Ego_d is a later revision (VC10 /Od), and its other lines differ by +6..+14 B (e.g. CameraUseCameraPoint 81 v 68/70). |
| `TRetVoid _Fail_Val;` + `return _Fail_Val.Return();` | RELOCATION_MATCH (neutral) | Ego_d local `_Fail_Val`, as the landed generated-script rows. |

- `speech_movie` and `PauseMyEntitiesPlease` are Ego_d's local names.

## 4. Loop form (Inferred, labelled in the source)
- `for (;; Game->NewScriptFrame())` matches.
- `while (true) { ... Game->NewScriptFrame(); }` gives DIFFER(1318v1362) at T40. Its c2 trace (`evidence/c2_trace_while_true.txt`) puts
  all three post-wait checks at the same f 0.1429 (score 111 at cost 60, 35 at cost 19). So post-SPLINE_04 and post-SPLINE_03 always flip
  together, and no T can match that form.

## Mutants and test
- **Mutants:** 17 of 17 killed (`mutants/00efa4f0.out`).
  - The `"welcome_guy"` mutant was re-run on occurrence 2, the code literal; occurrence 1 is the header comment.
  - `use_preload = false` is killed because it drops the five preload points.
- **Test:** `test/00efa4f0.cpp` links the real row object, with stubs bound by /alternatename and a fake 420-slot Game vtable.
  - It checks one whole pass token by token.
  - It then terminates at each of the 12 checks in turn. Checks 4..9 are the six movie-scope exits, three expanded and three through the
    COMDAT. Each must end with the pause and movie destruction (`P0 MD`).
