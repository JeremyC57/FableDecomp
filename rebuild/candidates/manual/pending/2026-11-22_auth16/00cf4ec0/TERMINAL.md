# TERMINAL 0x00CF4EC0 NScript::CQ_ArenaHoldingScriptScript::CFanMarker::Main (6164 B): outcome (A), AUTH16 2026-11-22
Ruling: ADVISOR_DECISIONS_B "AUTH13's 0x00CF4EC0 checks" (owner delegated, advisor final). Option (C) was run bounded; the 5 B was
not named by instruments (1) or (2), so the row ends in (A). Evidence: `CHECKS2.md` (this folder), `LOG.md`.

## State
- **Row unlanded.** No owner packet (owner delegated this row to the advisor; the advisor's terminal is (A)).
- **Grades used: 0.** No candidate was pre-registered, no readout run, nothing graded (step 3 needs the 5 B named first).
- **Best text** (unchanged): AUTH12 p6d = `rebuild/candidates/manual/pending/2026-11-22_auth12/00cf4ec0/00cf4ec0.cpp`, stock VC7.1, overlay
  AUTH12/AUTH13 `hdr/` (CTimer opt-in). Grade **DIFFER(6180v6164)** (AUTH13 reproduced, `$S/w22/auth13/00cf4ec0/r_p6d_00cf4ec0.cpp`).
- **Forced oracle (evidence, readout, NOT a grade):** `$S/w22/auth13/00cf4ec0/oracle/` (same result as AUTH12's `inl/best.*` run):
  AUTH10 fv.sh on p6d forcing only `??1CScriptGameResourceObjectMovieBase@@UAE@XZ` occurrences 8..14 to expand (`fm.json`):
  6164v6164, gobj.py on `p6d.f.obj` = RELOCATION_MATCH. The text is proven up to those seven inline decisions.

## Why it ends here
1. Pruning check: at the hit-block returns the movie dtor (c1 19) and ~CTimer (c1 14) have the same f, and score is proportional to c1
   at fixed f. Retail expands the movie dtor and calls ~CTimer at 5 of the 6 returns, so no S / T / frequency / loop-idiom change can
   produce that. Only a text difference that changes which callee or which inline context sits there can.
   Ego_d types the destroyed local `speech_movie` as CScriptGameResourceObjectMovieBase, the 55 stock rows' class. 0x00EB40C0's expanded
   and called dtors are one class.
2. The only open Ego_d disagreement in those blocks is +5 B per fan hit block (Ego_d / FableWin 541 / 561 v ours 536 / 556; the sibling
   0x00CF2B70 has +5 at all 7 of its lines).
   - Instrument (1): Ego_d register_scripts.obj PROCs (carved, 9640; FableWin Mod 0011, 9642, same set) and a typed whole-PDB query for
     argument-less free / static functions. Seven empty 5-B `void ()` bodies fit the profile (GFFPUClear, GFMountUtilityDrive,
     DoDirtyDisc, CEnginePrimitiveRenderUtil::FailIfXbox, CFileInstaller / CThreadedFileManager::ProcessDirtyDiscError,
     CMainGameComponent::GenerateMetFilesFromLugFiles). Without code the PDBs can neither confirm nor exclude them, and none is selected.
   - Instrument (2): every macro callee's PDB parameter list (Ego_d types + Ego_r) equals our declaration. No extra imm32 / address
     argument.
3. Not named, so the ruling's terminal applies: no spelling search (16+ spellings are already negative, AUTH12 / AUTH13). The row
   reopens only on a FableWin read.

## The one FableWin question (instrument 3, for the next Codex package)
> In FableWin.exe (the VC10 SP1 Debug build whose PDB is FableWin.pdb), disassemble `?Main@CFanMarker@CQ_ArenaHoldingScriptScript@NScript@@EAEXXZ`
> (Mod 0011 `Debug\register_scripts.obj`, section 0002 offset 0x01E8BBB0, VA 0x0361ABB0, 11940 B by the PDB; Ghidra's 7504-B body is a
> split, use the PDB length). Give the instructions of source line 887 only: offset 0x1DD2 in the function, VA 0x0361C982, 561 B, ending at
> 0x0361CBB3 (line 888). That line is one statement:
> `if (Game->GetHealth(seh_OakValeFan.GetScriptThing()) > 0.0f) { seh_OakValeFan.Speak(Game->GetHero(), "TEXT_QST_005_V2_FAN_OAKVALE_CHAT_HIT", GROUP_SELECT_NONE, false, true, false); while (seh_OakValeFan.IsPerformingScriptTask()) { Game->NewScriptFrame(); if (IsActiveThreadTerminating()) return _Fail_Val.Return(); } if (IsActiveThreadTerminating()) return _Fail_Val.Return(); }`.
> Our VC10 SP1 /Od compile of that statement is 556 B. Its listing is attached as `vc10_line289_298.cod` (the same statement split over
> our lines 289..298, compile line = FableWin Mod 0011's own S_ENVBLOCK: -Od -Oy- -Gr -GR -GS- -EHs -EHc -MDd -Gy -fp:precise, defines
> _LIB / LIB_COMP_DEBUG; the define set does not change our bytes). Which instruction (or instructions) of FableWin's 561 B is not in
> ours, and what does it call or reference (a symbol name, or the ILT / body target)? In particular, is it a `call` to one of these
> empty `void ()` functions (FableWin ILT thunk / body): GFFPUClear 0x017C164F / 0x018A6380, GFMountUtilityDrive 0x017D0A64 /
> 0x0186D080, DoDirtyDisc 0x01913A40, CEnginePrimitiveRenderUtil::FailIfXbox 0x018424F2 / 0x02E812E0,
> CThreadedFileManager::ProcessDirtyDiscError 0x01818A76 / 0x03010DF0, CFileInstaller::ProcessDirtyDiscError 0x0304CA60,
> CMainGameComponent::GenerateMetFilesFromLugFiles 0x017F670A / 0x018A9C61?
> Optional cross-check: CBowerstoneFan::Main VA 0x03616030 (7476 B), Ego_d lines 498 / 503 / 513 / 518 (390 B each), 550 (396 B),
> 555 / 569 (402 B): the same macro, each 5 B larger than ours (385 / 391 / 397).

Attach: `od/vc10_line289_298.cod` (our VC10 compile, no retail bytes) and `fw_fm_lines.txt` (FableWin line table, from the PDB).

## Queued either way (from the ruling, unchanged)
- The 16 landed /d2inlS rows that use this class: per-row S values are fits (one TU, one S). Debt.
- (B)'s finding as a family item: Ego_d's 19-B `~CCountedPointer<MovieBase>() { ReleaseData(); }` v the header default (112 B). Acceptance
  is the 55 rows. Name 0x006E7AB0.
- Land blockers if the row ever reopens: pick-any `??1CIsThingAlive` copy 14 B v owner 0x00E241B0 84 B (NEQ, from the inline_depth(0)
  device). The movie dtor copy is 25 B v 0x006E7B80's 66 B under the declared-only specialization. The local EScriptAIPriority enum must
  still be graded under strict headers. Still to check: CTimer forcing v a row-local body, and CTimer.h's wave-8 comment.
