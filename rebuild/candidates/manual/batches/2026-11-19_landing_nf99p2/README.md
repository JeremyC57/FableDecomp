# 2026-11-19 landing nf99p2: naming round 9, GameCODA part 2 (16 rows)

Records: `naming/2026-11-19_agentNF99P2/` (selfcheck 0 problems). 30 entries: 17 done, 13 blocked (0 grades spent on them).

## Landed: the Translate / Commit family
- 12 self renames to the GameCODA 2.10 CodeView names (RENAMES_applied.tsv): 0x00C783F0 CFloatReverbParameterTranslator::Translate and its
  callees UpdateSize / UpdateEchoTime / UpdateModulationDepth / UpdateReflectionsOutputGain / UpdateReverbOutputGain / UpdateReverbPan /
  PanToGains (0x00C78150, 0x00C77FC0, 0x00C78040, 0x00C77AE0, 0x00C77C00, 0x00C783B0, 0x00C77D90), mBToLinear 0x00C77700,
  CFloatFilterParams::Set_HighPass 0x00C74BB0 / Set_LpButterworth2 0x00C74AB0, CFloatReverbEngine::Commit 0x00C6AEF0. 11 bodies are
  byte-identical to the CODAApi.lib members; 0x00C77AE0 is named by Translate's calling relocation and the body shape.
- Binders: 0x00C54510 and 0x00C54190 (calls only; 0x00C54190 found by the exe scan, not by reloc_bindings), data respells in 0x00C75CE0 /
  0x00C75E90 (0x012BF138, 0x012BF088 to the SDK `extern "C"` names). EH funclet 0x01216140 (includes 0x00C54510) regraded RELOCATION_MATCH
  PASS with its funclet offset (`logs/funclet_01216140.log`; the plain recheck's DIFFER(456v8) is the known whole-parent artifact).
- **Advisor decision (owner delegated):** 0x00C54510 / 0x00C54190 reach Commit through `reinterpret_cast<CHLRvbParams*>` of their +0x290 block,
  the same cast both rows already use 16 times for the setters; landed as existing debt. Queued as one entry (the 16 CHLRvbParams::Set*
  setters 0x00C53440..0x00C53A40 + both binders; done when neither row has a reinterpret_cast).
- Headers (additive opt-ins): CFloatFilterParams.h, CFloatReverbEngine.h (`COMMIT`), CFloatReverbParameterTranslator.h (`TRANSLATE`,
  `PANTOGAINS`), CReverbConstants.h (`INLINE_CTOR`, `#ifdef/#else` keeping the original declaration). Held for the family: CFloatReverbDspEngine.h,
  CFloatShortReverb.h, CFloatShortReverbModule.h.

## Blocked: the short-reverb / DSP-engine family (q11415, q11492, q11493, q11512..q11515, q11517..q11522)
No hand-back can rename those eight owners without splitting a name: the smallest split-free set is about 27 rows (CFloatFilter::Clear
0x00C744E0 has seven landed callers). NOTES lists the rows and headers; NF99's drafts stay in the agent workspace.

## Checks
16 rows MATCH / RELOCATION_MATCH, every test PASS (`logs/recheck.log`). bindings_multi_check 1 (NullCoord); funclet_includes_check 1355 / 0.
Includers + exe-scan referencing rows: 11 good / 0 bad. No-new-split gate: 0 created (`logs/split_gate.txt`; 0x012B89C4 is an existing split).
