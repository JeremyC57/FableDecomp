# 0x00B45A40 naming and relocation audit

Ego_r donor 0x009BACA0 and FableWin donor 0x02CEF700 agree on `?CalcSWLightingNoClipSeparateMainLight@CEngineLightingManager@@QAEXABVC3DVector@@0ABVCEngineLocalLightArray@@AAVCRGBFloatColour@@2@Z`. Ego_d gives that member in engine_lighting.obj, including the original `main_col` and `col` vectors. The final RTM object emits this exact leaf and its return removes five parameter words.

The shared color conversion preserves `main_col` as a vector, and the neighboring 0x00B45CF0 uses the same lighting array methods, vector normalization and `dot *= falloff` sequence. Retail writes three primary channels and three miscellaneous channels without changing either alpha.

The audit resolved all 20 absolute operands and reproduced all 683 retail bytes after applying the relocations.

| Offsets | Retail address | Symbol |
|---|---|---|
| +0x020, +0x031, +0x04A, +0x065, +0x081, +0x090, +0x1C5, +0x1D4, +0x238, +0x247 | 0x0122DEDC | `__real@00000000`, verified 0.0f |
| +0x03B, +0x1DC, +0x1EB, +0x229, +0x24F, +0x25E | 0x0122DED8 | `__real@3f800000`, verified 1.0f |
| +0x163 | 0x013CE558 | `?GFastOneOverSqrtTable@@3PAKA`, named at this address in reloc_bindings.tsv and NAME_FIXES_QUEUE.tsv |
| +0x187 | 0x0122DED4 | `__real@40400000`, verified 3.0f |
| +0x191 | 0x0122F59C | `__real@3f000000`, verified 0.5f |
| +0x1F1 | 0x0129BA3C | `?DELTA_NOTIONAL_ZERO@@3MB`, exact address and spelling in ESTABLISHED_SPELLINGS.tsv |
