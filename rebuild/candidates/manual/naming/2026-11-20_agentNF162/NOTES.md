# NF162 notes (round 10, NScript quest script naming, CQ_/CV_ A-M)

Resumed after a container restart (2026-11-20). The earlier session's rows were re-checked: every inherited census was re-run with
callers.py against the exact new leaf (0x00D3AB00, 0x00D3ABE0, 0x00EA7460, 0x00F058D0: each sole binder already binds the new leaf),
0x00D3ABE0 got its objdump defined-symbol diff vs HEAD (only the own leaf changes; no casts of this remain), 0x00F081E0 was re-graded
(RELOCATION_MATCH), and recheck_final.log re-grades all 25 rows. One "SRC_FAIL at HEAD" row (0x00D16460) was re-graded at HEAD after the
restart: still C2143 / C2501 at engine/CSpawnedFunc1.h(25) (FABLE_FORCEINLINE used without fable_compiler.h), so that report stands.

## Families (land together)
- 0x00E9E270 (Transfer<BriarRoseOpinion>) + 0x00E9E400 (its TransferOut): the Transfer row's object now defines the component vtable COMDAT
  ??_7?$CPersistContext_TransferableComponent@W4BriarRoseOpinion@...@@6B@, whose slot 0 is the TransferOut leaf of 0x00E9E400 (slot 1 is
  NF136's landed TransferIn 0x00E9E3E0).
- 0x00E6A7F0 + 0x00E6A9E0 (DoMission calls EndMission; both renamed into NScript).
- 0x00E7F7D0 + 0x00E7F730 (GetNextPrizeCategoryWeight and its only binder).
- 0x00E7F410 + 0x00E80F10 (both binders of 0x00E7FD50 GenerateSignText).

## Callee stand-ins replaced by PDB spellings (the address's spelling count does not grow)
- 0x009E2560: ?TransferOut_009e2560@@YIXPAVCPersistContext@@JPAJ@Z -> ?TransferOut@CPersistTraitsSLONG@@SIXAAVCPersistContext@@AAVCCharString@@ABJ@Z
  in 0x00E9E400 / 0x00E38F60 / 0x00EB6730 / 0x00EB68E0 (existing split on 0x009E2560 shrinks; 0x00566360 / 0x00E1E0F0 already bind the PDB spelling).
- 0x00E9E350: ?TransferBinaryIn@?$CPersistTraits@W4EOpinionCurrent@@@@... -> FableWin.pdb ?TransferBinaryIn@?$CPersistTraits@W4BriarRoseOpinion@...@Z
  in 0x00E9E270 (self row 0x00E9E350 keeps its stand-in free-function leaf: count stays 2; NAME_FIXES).
- vtable 0x012ED7D4: ??_7 of the EOpinionCurrent component -> the BriarRoseOpinion component (0x00E9E270, its only binder).
- 0x00E6A7F0: EndMission 0x00E6A9E0 / WatchForLeaving 0x00E6A940 / CSpawnedFunc vtable 0x012E8094 -> NScript spellings.
- 0x00EDC450: EndMission 0x00EDC640 / WatchForLeaving 0x00EDC5A0 / vtable 0x012F3C7C -> CV_TempleOfLightScript (NScript) spellings.
- 0x00F058D0: RefreshSpyAllegiences 0x00F05A10 -> NScript spelling. 0x00F081E0: Add -> AddSpawnedFunction (in place, its existing local CScriptBase
  model; its object's SpawnFunction COMDAT is unchanged vs HEAD and no shared header defines it). 0x00EA8130: see below.

## Narrow-rule row (header-less local class on a shared base)
- 0x00EA8130: the row-local NScript::CV_MazeResearchScript (no header anywhere under rebuild/include: grep `class|struct CV_MazeResearchScript`)
  keeps its token and now derives from the shared engine/CScriptBase.h, so its spawn binds CScriptBase::AddSpawnedFunction 0x00CB7E50.
  Defined symbols vs HEAD unchanged. The row's other local models (CCharString, CScriptThing, CEntityScriptBase ...) are as at HEAD.

## File moves (self renames move the parent's file)
All self-renamed rows (0x00D3AB00 0x00D3ABE0 0x00D8E990 0x00E2BAC0 0x00E2BB10 0x00E38F60 0x00E69F90 0x00E6A0B0 0x00E6A340 0x00E6A7F0
0x00E6A9E0 0x00E7F7D0 0x00E9E270 0x00E9E400 0x00EA7460 0x00EB6730 0x00EB68E0 0x00EDC450 0x00F058D0). Includer scan: no other file in
rebuild/src/compiled or rebuild/tests names any changed row's HEAD basename, so no EH funclet needs a regrade.

## Tests
Tests unchanged where they are host models that name no shared-header class leaf (0x00D3AB00, 0x00D3ABE0, 0x00E6A0B0, 0x00E6A340, 0x00EA7460,
the four TransferOut rows, 0x00E9E270: its HEAD test is a host model of CPersistContext, left as at HEAD, not new). Test mocks renamed in
rows/<addr>_test.cpp for 0x00D8E990, 0x00E2BAC0, 0x00E2BB10, 0x00E69F90, 0x00E7F7D0.

## __assume
- 0x00E9E270: the HEAD row's bare `default: __assume(0);` became the guarded EXTRA_RULES form (`#ifdef _MSC_VER __assume(0); #elif
  defined(__GNUC__) __builtin_unreachable(); #endif`) with an `// Inferred:` line; not a new use.

## Flags lines
None. No row needed flags its build entry lacks.

## Descriptive names
None added. (0x00E9E270's descriptive EOpinionCurrent is replaced by the PDB BriarRoseOpinion.)

## Header items for the coordinator
- engine/CSpawnedFunc1.h uses FABLE_FORCEINLINE (via FABLE_SPAWNEDFUNC_INLINE) without including fable_compiler.h: 5 landed rows SRC_FAIL at
  HEAD (0x00D16460 0x00E0DAA0 0x00E76920 0x00F09420 0x00F09530); re-confirmed on 0x00D16460 after the restart.
- q6380 / q6274: the CSpawnedFunc1 two-view CScriptThing shape (0x00E6AAC0, 3 failed forms) blocks the remaining stand-in Add binders.

## Rule slips
- One Bash call exited non-zero: a `pgrep -f probe.py` as the last command after the census probe had finished (exit 1).
- After the restart, one Bash call exited non-zero: a `cat` of a lock owner file that did not exist (0x00E9E350, unlocked) as the last command (exit 1).
