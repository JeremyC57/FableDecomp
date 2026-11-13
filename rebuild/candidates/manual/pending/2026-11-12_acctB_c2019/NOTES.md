# Hand chunk 2019 (CEnvironment): 1 of 1 byte-exact

All grades with `FABLE_HDR_OVERLAY=/opt/fable_lane/w22/agent2019/hdr` (hdr/README.md lists every overlay change; all opt-ins or a new
header). Row extent checked: 0x006B42F0 `sub esp,2B4h` .. 0x006B48F4 `ret 4` = 0x607 = 1543 bytes; the catalog length is right (no
SPLITS.tsv). Read-only vland: RELOCATION_MATCH + behaviour PASS (WARNs only: RELOC_NAME for the wrongly labelled 0x006B5B80 / 0x008857E0
and the label of 0x01229390, RELOC_CONFLICT for the TU-local `_$E2` that another landed TU (0x01219F00) also uses for its own stub).

## What matched (1, RELOCATION_MATCH)
- **CEnvironment** -- 0x006B42F0 `?RenderUpdate@CEnvironment@@QAEXM@Z` (1543 bytes, grade 4 of pass 1): on the first call runs
  Update(0); clamps the frame fraction to [0, 1] in place; interpolates PrevGameTickState / GameTickState into
  InterpolatedEnvironmentParams: Time, then `ThemeChannels.resize(6)` and for each of the six channels `Prev.BlendedTheme[c].Blend(
  interpolated_theme, Cur.BlendedTheme[c], t)` into the function-local static followed by three element loops copying its Colours[19],
  Scalers[30] and TextureSets[3] into `ThemeChannels[c]`; then the wind / theme-wind vectors and speeds and the sun / moon / diffuse
  vectors (AccurateNormalise on the last three); then the spell-effect interface's radial-blur and displacement parameters into the
  params' ScreenEffectParams and its screen-filter parameters, copied into a CEngineColourFilterSettings, to
  `CMainGameComponent::Get()->GetDisplayEngine().Get3DEngine().SetScreenColourFilter(...)`.
  Shape lessons: (1) braced `if (!PassedFirstUpdate) { Update(0.0f); }` + the inline GFLimit clamp give the static's VC7.1 scope ?4?
  (retail's mangled name); an if / else-if clamp gives ?8?. (2) The texture-set copy needs vector::operator= declared
  (FABLE_RETAIL_STLP_VECTOR_ASSIGN), else the implicit operator= copies the CArrays bitwise. (3) FableWin's inline
  GetSpellEffectInterface() (not the raw member) gives retail's edx / ecx roles and the `lea edx` ahead of `rep movsd` in the tail.
  (4) `GFLimit(frame_fraction, 0.0f, 1)` (FableWin instance GFLimit<float,float,int>) instead of (0.0f, 1.0f): same instructions, but
  the folded int-to-float bound frees a c2 record that the scaler loop's induction variable reuses (id 1733 < the `_M_start` reload
  temps 2376..), so the IV becomes the SIB index as in retail (`[edx+ecx+d]`, 9 stores 0x006B4455..0x006B44A4). GFLimit(.., 0, 1.0f)
  (GFLimit<float,int,float>, also FableWin) gives identical bytes; GFLimit(.., 0, 1) (the Ego_r instance MHH) changes 92 lines.

## Grades
| pass | # | source change | result |
|---|---|---|---|
| 1 | 1 | first draft (probe/r1.cpp + GFLimit(.., 0.0f, 1.0f) + braced if) | DIFFER(1640v1543), 277/383 equal instructions: texture sets copied bitwise |
| 1 | 2 | + FABLE_RETAIL_STLP_VECTOR_ASSIGN | DIFFER(1543v1543), 363/383: residue A scaler-loop SIB order (9 stores), residue B tail 0x006B484E..0x006B4889 |
| 1 | 3 | + GetSpellEffectInterface() accessor (new overlay opt-in) | DIFFER(1543v1543), 374/383 (383/383 registers masked): only residue A |
| 1 | 4 | GFLimit(frame_fraction, 0.0f, 1) | RELOCATION_MATCH |
| 1 | 5 | comments only (final file, via gall.sh) | RELOCATION_MATCH |
Pass 2 / pass 3: not needed. On top: about 110 compile-only probes (tools/chk.sh: compile, print the scaler-loop SIB pattern and the tail;
compared with my own builds, never diffed against retail) and 9 c2 traces (tools/c2h/sib.py, recs.py from agent3005).

## How residue A (SIB base / index order) was found
- c2 SIB trace: each store's address node holds the per-store `_M_start` reload temp and the loop IV; the smaller record id becomes the
  SIB index. Base: temps 2373..2380 / 2559..2568, IV 2570 (fresh) -> index = temp -> `[ecx+edx+d]`. Final: IV 1733 (reused) ->
  index = IV -> `[edx+ecx+d]`. The disp-0 store is `[ecx+edx]` in every build and in retail.
- Diagnostics (never delivered): dead outer locals `(void)`-ed (k=1..7), loop-variable type / scope variants, a preceding function in
  the TU (ids unchanged: per-function numbering): no effect. 76..91 dead statements `dead = RegionTheme * N + dead` (before or after
  the loop) or ~140 forwarded dead names flip it (fewer do not) -> the IV must get a freed / lower record, not a shifted one.
- A CSE'd duplicate before the loop frees such a record: `(1.0f - frame_fraction)` written in the Time lerp next to the named local
  flips it but moves the Time x87 order (fxch); the int bound of GFLimit flips it with no other change.
- Tools: tools/chk.sh (probe), tools/sweep.sh + probe/gen_*.py (diagnostic generators, each a handful of variants), tools/c2h/
  (copied from agent3005, paths retargeted), tools/sibsum.py, tools/md.sh / md.py / rel.py (from agent2004), tools/cc.py, tools/odis.py.

## Names
Proven (LABELS.tsv): the row (both PDB name sets + the atexit stub), the static and its guard, the CThemeChannel ctor / dtor (nested
PDB spelling), vector<CThemeChannel>::resize 0x006B6D90 (STLport 4.6 + body), the three GetRenderFrameInterpolated* getters (Ego_r +
`ret 8` + result sizes). Callees already bound correctly: Update 0x006B3FF0, Blend 0x00887B70, ??0CBlendedEnvironmentTheme 0x008864A0,
AccurateNormalise 0x00A14440, vector<long> / vector<float>::operator= 0x00454886 / 0x0045EF2D, CMainGameComponent::PMainGameComponent
0x013B86A0 (established spelling), _atexit. Slot 61 of CIEngine = SetScreenColourFilter (CIEngine.h, both PDBs).
Descriptive: the local names `one_minus_frame_fraction`, `channel`, `i`, `screen_filter`, `colour_filter` (no source); parameter name
`frame_fraction` from the repo header's declaration.

## Label corrections (NAME_FIXES.tsv)
0x006B5FB0 (landed top-level ??0CThemeChannel@@), 0x006B5B80 (catalog CBufferPairTriple_006b5b80 dtor), 0x006B5D80 (landed leaf
??1CBufferPairTriple_006b5d80@), 0x00454886 / 0x0045EF2D (extra wrong bindings), 0x00A14440 (one ?Normalise@ binding), 0x008857E0
(catalog SetAsAlliedWithFaction).

## Header debt
- CIDrawEnvironmentScreenFilterParameters / CIDrawEnvironmentScreenEffectParams have FableWin default constructors
  (??0..@@QAE@XZ) that are not declared (bodies unknown; not needed by the row).
- CEnvironment's SpellEffectInterface / SpellEffectInterpolator stay flattened raw pointers (scoped_ptr in the PDB).
- engine/CIDrawEnvironmentParams.h is still only in the pending c2030 hand-back; my overlay copy extends it.

## Skipped
None.
