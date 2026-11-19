# 2026-11-19 landing nf105: the GameCODA short-reverb / DSP-engine family, whole (37 rows)

Records: `naming/2026-11-19_agentNF105/` (selfcheck 0 problems). NF105 resumed NF102's step 2 and closed the family whole. 29 entries: 27 done
(q11492, q11493, q11512..q11515, q11518..q11522, q11543..q11558), 1 blocked (q11517 `?m_pDSP@CReverbDspProcessor@@1PAVCReverbFns@@A`: five rows
0x00C6A8B0 / 0x00C74360 / 0x00C74420 / 0x00C74450 / 0x00C74480 need a CReverbDspProcessor class port), 1 open (q11559, vtable 0x012B89C4).

## Rows (37): 12 MATCH + 25 RELOCATION_MATCH, every test PASS (`logs/recheck.log`)
All 37 are self renames to their GameCODA 2.10 CodeView spellings on the shared headers (RENAMES_applied.tsv; 74 file moves). Each body is
byte-identical to a CODAApi.lib release member; the second source is the byte-identical SDK caller's relocation at the retail call site (or,
for 0x00C69B60, its vtable slot). The closure was 37 rows, not ~27: eight rows were forced in (0x00C766F0, 0x00C76560, 0x00C767E0, 0x00C76C20,
0x00C75820, 0x00C89350, 0x00C75010, 0x00C75F80), plus 0x00C6A5D0 ProcessReverb and 0x00C75C30 CFloatShortReverb::Create.
- **Casts (advisor decision, owner delegated; GOTCHAS nf105 line):** 10 rows convert the SDK's own `void*` / `void**` (`m_pvParams`,
  `pvMemory`, `pvInput`, `apvOutputs`, `m_pDSP`) to the member types their callees take (`static_cast` / `reinterpret_cast<float**>`). Both
  sides are SDK-declared types (the leaves encode `PAX` / `AAPAX`; `CReverbDspEngine.h` declares `void* m_pvParams` as the CodeView member
  record does), so C++ requires the conversion: source-faithful, not debt.
- Headers: 11 overlay headers with 37 add-only `FABLE_RETAIL_*` opt-ins (NF99 4, NF102 5, NF105 28; `naming/.../hdr/README.md`): the A holds on
  CFloatReverbDspEngine.h, CFloatShortReverb.h, CFloatShortReverbModule.h, CFloatCyclicBuffer.h, CFloatAllpassSection.h, CFloatDiffuser.h,
  CFloatReflection.h, CFloatLongReverbModule.h are released with this landing.

## Checks
assert_names_check 0; bindings_multi_check 1 (NullCoord); funclet_includes_check 1355 / 0. Includers + exe-scan referencing rows: 11 good / 0
bad. No-new-split gate with the "before" set from the 37 rows compiled at HEAD (self targets use the HEAD Leaf line): 0 created; 0x00C6A460,
0x00C75820, 0x00C891A0, 0x00C89350 are existing splits (their old leaves already differed from what 0x00C6A8B0 / 0x00C75A20 / 0x00C75AB0 bind;
those three rows are queued in NAME_FIXES).
