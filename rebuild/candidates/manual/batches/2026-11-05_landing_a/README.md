# Landing 2026-11-05a: chunk 774 (8 rows, including 2 former asm bakes)

Authored by one expert agent (owner, 2026-11-05). The asm rules are in `docs/pipeline/ASM_RULES.md`.

## Landed

| Address | Name | Grade | Purity |
|---|---|---|---|
| 0x00F21590 | NScript::CQ_ArenaScript::GetEndRoundQuestion | RELOCATION_MATCH | genuine |
| 0x00E80060 | NScript::CV_FishingCompetitionScript::GetHeroTitleTextTag | RELOCATION_MATCH | genuine |
| 0x007A4520 | CTCWillEnflame::PlaySoundW | RELOCATION_MATCH | genuine |
| 0x007A5580 | CTCForcePushPower::PlayCastSound | RELOCATION_MATCH | genuine |
| 0x00C4B890 | PKCBufferedReader::ReadBlock (/EHsc) | RELOCATION_MATCH | genuine |
| 0x00C4B300 | PKCBufferedReader::PKCBufferedReader (/EHsc) | RELOCATION_MATCH | genuine |
| 0x0042DF9E | CNewFrontendGameComponent::Draw (former bake, /O1 /Oy-) | RELOCATION_MATCH | `retail_asm` through a new helper |
| 0x00551340 | NUISystem::CTable::GetDataFromDef (former bake) | RELOCATION_MATCH | genuine |

- **The six new rows** went through `prep_landing.sh` and `vland --land` (`logs/land.log`).
- **The 8 changed headers:** hdrcheck2 reads `default=same` on each, and all 59 sibling rows still grade RELOCATION_MATCH PASS.
- **0x0042DF9E:** retail runs `fnstcw` / `and byte [cw+1],0F3h` / `fldcw`, which is FableWin.pdb `?GFFPUSetRoundingNearest@@YIXXZ`.
  - 32 C++ and 12 intrinsic probes never emitted it (`ATTEMPTS_0042df9e_fpu.tsv`).
  - The new shared helper `engine/GFFPUSetRoundingNearest.h` has a `FABLE_PORTABLE` twin. Its dual-path test is
    `rebuild/tests/portable/test_GFFPUSetRoundingNearest.cpp`; `portable_twins_check.py` reports 404 steps, 0 failed.
  - The row entry no longer has `Grade = 'asm_bake'` and now has `CompilerFlags = '/O1 /Oy-'`. Its entry is unaligned, right
    after a neighbour's `leave; ret`, the mark of an /O1 TU.
- **0x00551340:** the catalog oracle was cut 3 bytes short. The jne at 0x005514E4 jumps to 0x005514EB `add esp,10h`, and `ret`
  is at 0x005514EE, so the length is now corrected to 431. The C++ source grades RELOCATION_MATCH PASS against the corrected
  bytes, and its `asm_bake` grade line is removed.
- Both former bakes were regraded with `--recheck-landed` (`logs/recheck_*.log`).

## Parked
In `pending/2026-11-04_linux_hand774/`, each with its residue and next idea (`agent_NOTES.md`):

| Address | Name | Grades | Result |
|---|---|---|---|
| 0x00E7FD50 | GenerateSignText | 10 | DIFFER(802v784) |
| 0x00C40D10 | — | 12 | DIFFER(252v272) |
| 0x00597BF2 | GotoPreviousScreen (bake) | 40 | DIFFER(517v517) |
| 0x009BEF80 | SetViewport (bake) | 30 | register allocation |
| 0x0053C332 | DoRecomputeOffsets (bake) | 57 | DIFFER(1111v1111), one store pair swapped |

## Names
- `ghidra_out/labels_landing_2026-11-05a.tsv`: 31 labels, 8 catalog and 12 manifest renames.
- 16 NAME_FIXES are queued (tag `land0511a`). Among them, 0x0041E5F2 is `CManager::GetInstance`; landed 0x005820E7 binds it under
  the CFrontEndManager spelling.
