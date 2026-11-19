# CDefString binding swap (2026-11-19): 0x00415D70 = GetString, 0x00415DD0 = operator CCharString

Owner decision (hand1 lane, 2026-11-18/19, "Swap and re-land"; recorded in
`rebuild/candidates/manual/pending/2026-11-18_hand1/IN_PROGRESS.md`): the two identical 25-byte bodies are
`?GetString@CDefString@@QBE?AVCCharString@@XZ` at 0x00415D70 and `??BCDefString@@QBE?AVCCharString@@XZ` at 0x00415DD0,
the reverse of the naming wave 13 binding (`2026-11-01_wave13`, family cds). Evidence (Fable advisor fable3):
- VC7.1 /O1 /O2 emit an inline callee before its caller (112 of 112 small retail caller->callee pairs put the callee at the
  lower address); FableWin.pdb (/Od) keeps source order (??B 0x018C20E0 before GetString 0x018C2120). The conversion
  forwards to GetString, so GetString comes first: GetString 0x00415D70, ToWideString 0x00415D90, ??B 0x00415DD0.
- 0x008E7040 CMeleeCombatAbilityDef::IsAbilityValid on the shared header in its natural spelling (`.GetString().GetLength()`
  plus an implicit conversion argument) matches only under the swap; 0x0066A9E0 CTCSoundPlayer::PlayCollisionSound (FableWin
  shape, plain inline heuristics) likewise; the /O2 game rows that needed `FABLE_RETAIL_CDEFSTRING_FORCEINLINE_CONVERSION`
  (0x006DEEE0, 0x007A1B20, 0x007EF200, 0x008A8C60) match on the plain header.

Retail callers (`.text` E8 scan): 139 call sites, 90 to 0x00415D70, 49 to 0x00415DD0, in 44 landed rows (the rest are in
unlanded functions). The split is clean: the NScript TUs and 0x008E7040 call 0x00415D70; the game TUs call 0x00415DD0.

## What changed
- The two leaf rows swapped names and files (`00/41/CDefString_GetString_00415d70.cpp`,
  `00/41/CDefString_operatorCCharString_00415dd0.cpp`), each with a real test (the old 0x00415D70 test only printed PASS).
  Catalog names (auto-re-candidates.tsv), build_candidates.ps1, ARTIFACT_INDEX.tsv, labels
  `ghidra_out/labels_cdefstring_swap_2026-11-19.tsv`.
- reloc_bindings.tsv: GetString -> 00415d70 (first caller 00e02e50), ??B -> 00415dd0 (first caller 005c0e81); the made-up
  `??BCDefString@N_00415dd0@@QAE/QBE` pair of 0x008E7040 removed. bindings_multi_check stays at its baseline (1, NullCoord).
- engine/CDefString.h (default path unchanged, hdrcheck2 `default=same`):
  - `FABLE_RETAIL_CDEFSTRING_OUTOFLINE_GETSTRING` (new): forwarder inline, GetString declared only. The NScript TUs (23 caller
    rows + 19 same-TU siblings that only carried the define), replacing `OUTOFLINE_CONVERSION`.
  - `FABLE_RETAIL_CDEFSTRING_OUTOFLINE_CONVERSION` re-pointed: the conversion declared only (0x00415DD0). The size-optimised game
    TUs (0x00417A58 InitSound, 0x0065B854 AddThingToVillage, 0x0041D21B CreateComponent) and 0x005F6E40, where VC7.1 expands
    the forwarder around GetString.
  - `FABLE_RETAIL_CDEFSTRING_FORCEINLINE_CONVERSION` removed (no user left); `FORCEINLINE_FORWARDER` /
    `FORCEINLINE_GETSTRING` comments corrected (bodies unchanged).
- Caller rows (see ROWS.tsv): local CDefString models now declare the conversion where retail calls 0x00415DD0;
  explicit `.GetString()` sites that reach 0x00415DD0 bind the conversion to a const reference
  (`const CCharString& name = def;`): copy- or direct-initialising a CCharString from a CDefString makes VC7.1 build a
  temporary and copy it (+17..+68 bytes), a reference binding constructs in place. 0x005F6E40 needs the explicit
  `.operator CCharString()` at its three sites (a reference shifts one address operand, `[edi+eax]` v `[eax+edi]`).
  0x008E7040 is the hand1 natural candidate on the shared headers.

## Pre-existing breakage fixed on the way
- 0x0065B854 CTCVillage::AddThingToVillage no longer compiled (C2011): Codex pass 16 put the real `EVillageTaskType` in
  engine/CVillageTaskBase.h while the row kept a local empty copy. The row now includes the header.
- 0x00E12580 CQ_WaspBossScript::DoMission graded DIFFER(1626v1446) in a catalog recheck: its `// Flags: /d2inlT60` (landing
  nf71) never reached build_candidates.ps1. `CompilerFlags = '/O2 /Oy /W3 /d2inlT60'` added.

## Recheck
Baseline before any change (current binding): 45/45 RELOCATION_MATCH + PASS once the two fixes above are applied.
After (`logs/recheck_after.log`, `ROWS.tsv`): **65/65 MATCH or RELOCATION_MATCH + PASS** (the 44 callers, the two leaves,
0x006DF860, the 19 NScript siblings; read-only `--recheck-landed`), no RELOC_CONFLICT or RELOC_IDENTITY on either symbol. Then
0x0066A9E0 landed (`batches/2026-11-19_hand1_cdsswap/`).

## Cost of the swap (for the record)
Under the old binding the four FORCEINLINE_CONVERSION rows needed a device; under the new one the size-optimised TUs and
0x005F6E40 need the declared-only conversion, the NScript TUs need GetString declared only (they needed the conversion
declared only before), and seven `.GetString()` sites became const-reference conversions. The emission-order evidence is
independent of these spellings.
