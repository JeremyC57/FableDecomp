# 2026-11-19 landing nf102: naming round 10, the GameCODA CHLRvbParams setters (18 rows)

Records: `naming/2026-11-19_agentNF102/` (selfcheck 0 problems). 48 entries: 17 done (q11560..q11576), 1 done-already (q11542), 1 deferred
(q11415, held by acctB), 29 open (the short-reverb / DSP-engine family, stopped part-way for context: NF105 resumes from NF102's
`work/fam/FAMILY.tsv`; q11559, the vtable 0x012B89C4).

## Landed
- 16 self renames to the GameCODA 2.10 `CHLRvbParams::Set*` names on engine/CHLRvbParams.h (0x00C53440..0x00C53A40; RENAMES_applied.tsv),
  bodies unchanged: each is byte-identical to the CODAApi.lib SalSwReverb.obj COMDAT of that name, and the SDK `CSalSwReverb::Update`
  (889 bytes = retail 0x00C54190) has its 23 relocations at the same offsets, naming every setter at the matching retail call site.
- Binders 0x00C54190 / 0x00C54510 hold the +0x290 block as the header `CHLRvbParams`: the 16 setter casts and nf99p2's Commit cast are gone
  (closes q11576, the advisor's follow-up to nf99p2). 0x00C54510 also types 0x012B89E8 as `_RVB_VECTOR` (its only landed reference).
- Headers: CHLRvbParams.h opt-ins `FABLE_RETAIL_CHLRVBPARAMS_SETTERS`, `_SETLEVEL_INLINE`, `_SETPAN_INLINE` (additive). The family's
  opt-ins (CFloatCyclicBuffer, CFloatAllpassSection, CFloatDiffuser, CFloatReflection, CFloatLongReverbModule + NF99's three) stay held.
- EH funclet 0x01216140 (includes 0x00C54510) regraded with its funclet offset: RELOCATION_MATCH PASS (`logs/funclet_01216140.log`).

## Left / queued
0x00C54190 keeps two pre-existing `reinterpret_cast<C3DSoundSource*>(this)` (callees 0x00C53D50 / 0x00C53E40). NF102 found CDecodedStream
looks like GameCODA's CSalSwReverb (Update / Initialize / SetReflectionsPan / SetReverbPan match), but the retail vtable 0x012B89C8 has 8
slots and the SDK's 22: queued as one entry for a ruling before any class rename. NAME_FIXES (SetLevel / SetLevelLF / SetLevelHF,
sal_levelToGain, the two pan setters) queued.

## Checks
18 rows MATCH / RELOCATION_MATCH, every test PASS (`logs/recheck.log`); assert_names_check 0; bindings_multi_check 1 (NullCoord);
funclet_includes_check 1355 / 0. Includers + exe-scan referencing rows: 11 good / 0 bad; no-new-split gate: 0 created (the exe scan finds no
landed binder outside the two rows).
