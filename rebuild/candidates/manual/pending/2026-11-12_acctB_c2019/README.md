# acctB hand-back, chunk 2019 (1 row matched)

Authored by an Opus 5.5 agent, re-verified by the acctB local coordinator. Nothing is landed.

| address | result | notes |
|---|---|---|
| 0x006B42F0 `?RenderUpdate@CEnvironment@@QAEXM@Z` | RELOCATION_MATCH 1543v1543, test PASS (g++ and VC7.1) | no flags, no device; 5 grades, all in pass 1 |

Coordinator re-verification (cl 13.10.3077 under Wine, `FABLE_HDR_OVERLAY=hdr/`): `coordverify/` (grade, both test runs,
read-only vland: 1 win, no RELOC_IDENTITY / NOT_GENUINE). Purity and padded-struct greps are clean: no asm, no
`volatile`, no pragma, no forced inline. vland warnings: three `RELOC_NAME` (wrong catalog labels, item 3) and one
`RELOC_CONFLICT` on `_$E2`, the TU-local atexit stub name (GOTCHAS 2026-10-28 / 2026-10-29: per-TU static-initialiser
locals, not a shared symbol; do not bind it).

## Review items for landing

1. **`GFLimit(frame_fraction, 0.0f, 1)` -- the int upper bound is a measured lever, not source evidence.** Both
   `GFLimit<float,float,int>` (`??$GFLimit@MMH@@YIXAAMABMABH@Z`) and `GFLimit<float,int,float>` are FableWin.pdb instances
   and give identical clamp bytes. The choice matters only for the SIB base / index order of 9 stores in the scalar-copy
   loop (0x006B4455..0x006B44A4): agent's c2 trace shows the record with the smaller c2 id becomes the index; with
   `(0.0f, 1.0f)` the loop induction variable gets a fresh record above the per-store `_M_start` reload temporaries, with
   the int bound the folded conversion temporary frees a record the induction variable reuses, which gives retail's
   `[edx+ecx+d]` and changes nothing else. Your call whether that reading is acceptable.
2. **Headers, land order** (`hdr/README.md`; all opt-ins or new files, defaults preprocess the same):
   `engine/CEnvironment.h` is based on the pending **c2020** copy (keeps its opt-in; adds
   `FABLE_RETAIL_CENVIRONMENT_TICKSTATES`, which types the nested `CEnvironmentGameTickState` and the three state members
   at the same offsets and cannot be combined with `_WEATHERSCALERS` / `_SUNLIGHTATTENUATOR`, and
   `FABLE_RETAIL_CENVIRONMENT_SPELLEFFECTINTERFACE`, FableWin's inline `GetSpellEffectInterface()`);
   `engine/CIDrawEnvironmentParams.h` is based on the pending **c2030** copy (adds
   `FABLE_RETAIL_CIDRAWENVIRONMENTPARAMS_SCREENEFFECTS`: CThemeChannel ctor / dtor declarations, ScreenEffectParams at
   +0x60); new `engine/CIDrawEnvironmentScreenEffectParams.h` (four Ego_r parameter structs);
   `engine/CEnvironmentSpellEffectInterface.h` gets `FABLE_RETAIL_CENVIRONMENTSPELLEFFECTINTERFACE_RENDERFRAME` (three
   GetRenderFrameInterpolated* getters) and a guard macro on the existing enum. The c2020 and c2030 header bases are not
   in `rebuild/include` yet: land those first or merge these copies over them.
3. **Name fixes** (`NAME_FIXES.tsv`): 0x006B5FB0 landed as a top-level `??0CThemeChannel@@QAE@XZ` (real:
   `??0CThemeChannel@CIDrawEnvironmentParams@@QAE@XZ`); 0x006B5B80 catalog label CBufferPairTriple (real:
   `??1CThemeChannel@CIDrawEnvironmentParams@@QAE@XZ`); 0x006B5D80 landed leaf CBufferPairTriple (real:
   `??1CBlendedEnvironmentTheme@NEnvironmentInternal@@QAE@XZ`); 0x00454886 / 0x0045EF2D extra wrong bindings (they are
   `vector<long>` / `vector<float>` `operator=`); 0x00A14440 one wrong `?Normalise@` binding (AccurateNormalise);
   0x008857E0 catalog label SetAsAlliedWithFaction (real: GetRenderFrameInterpolatedRadialBlurParameters).
4. **Atexit row 0x01229390**: the landed stub row has a shortened stand-in body for its owner function; this row's file
   is the real owner (static `interpolated_theme` and its guard word, `LABELS.tsv`) and can replace it.
5. Header debt: FableWin lists default constructors for the screen-filter and screen-effect parameter structs that are
   not modelled. The agent ran about 110 compile-only probes, some as small sweeps of 4..9 variants compared with each
   other (none diffed against retail, none delivered).

## How to land

`rows/006b42f0.cpp` + `rows/006b42f0.test.cpp` with the `hdr/engine/` files (after c2020 / c2030), then `payload.json`
through your usual vland. `LESSONS.md`: the static's mangled scope number (`?4?`) is set by the braces and clamp form
before it; `FABLE_RETAIL_STLP_VECTOR_ASSIGN` is needed when a struct of arrays is copied with its implicit `operator=`;
SIB base / index order follows c2 record ids; accessor v direct member read fixed the register roles in the tail.
