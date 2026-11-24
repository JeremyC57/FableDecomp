# NF244 overlay headers (base copies in ../orig/, taken from the repo working tree at start)

## engine/CIEngine.h (q12680 step 1: three of the four PDB slot declarations become the default; a default-path change by the entry's instruction)
- Change: the `#ifdef FABLE_RETAIL_CIENGINE_SCREENPOS / _CONVERTSCREENPOS / _CONVERTWORLDPOS ... #else VirtualNN_<addr>() = 0; #endif` blocks
  lose their `#ifdef` / `#else` / placeholder / `#endif` lines: slot 11 `ConvertScreenPosToWorldPos(const C2DVector&, C3DVector&) const`,
  slot 12 `ConvertScreenPosToWorldPosAtViewDistance(float, const C2DVector&, C3DVector&) const`, slot 13 `ConvertWorldPosToScreenPos(const
  C3DVector&, C2DVector&) const`, all pure as before (CEngine overrides Ego_r.pdb 0x009AE850 / 0x009AE730 / 0x009AE5F0, FableWin.pdb
  0x02C9FA10 / 0x02C9F7B0 / 0x02C9F4A0; the 84-symbol check below finds all three CEngine spellings in both). Those three macros are now
  no-ops in CIEngine.h (engine/CEngine.h still gates its slot 12 / 13 override declarations on two of them: unchanged).
- Slot 8 is NOT changed: `Virtual08_00b27d90` stays the default and FABLE_RETAIL_CIENGINE_RENDER keeps the PDB `Render` (and the
  CEngineInterpolationInfo forward declaration) as an opt-in. Reason: the 16-subset bisect on 0x0058578B (NOTES entry 1): {SCREENPOS,
  CONVERTSCREENPOS, CONVERTWORLDPOS} keeps it RELOCATION_MATCH, all four together do not.
- The top comment's "unnamed ... descriptive VirtualNN_" sentence is replaced by four lines saying the above. No layout or slot change.
- pin_asserts (repo tree v hdr/): pinned 4 (sizeof / offsetof asserts at old lines 168 / 188 / 189 / 190). assert_names_check (a tree copy
  with this header v the repo tree): 0 mismatches.
- Includers: CIEngine.h is included by engine/CEngine.h only (closure: 2 headers); 159 landed rows have a source or test that includes
  either (work/eq_addrs.txt).
- Equivalence (equiv_check over those 159 rows, old tree = a copy of rebuild/include, new = the copy + this header): 122 differing files over
  108 rows (107 sources, 15 tests; work/equiv1.tsv). All 108 rechecked with VC7.1 under this overlay (vland --recheck-landed, six batches,
  work/rc00..rc05.log, each under timeout 900; the ten tests that overrode the placeholders replaced by rows/<addr>_test.cpp):
  101 RELOCATION_MATCH + 7 MATCH, 108 PASS, 0x0058578B RELOCATION_MATCH. Three RELOC_CONFLICT WARNs (0x0064BFBA, 0x0064E978, 0x006B42F0)
  appear identically in a no-overlay HEAD control run (work/hc.log): pre-existing, unrelated callees.
- The ten tests (0x004A1840 0x004BC1A0 0x0054EF00 0x00646F67 0x0064A639 0x0064AE80 0x0064E978 0x00650E98 0x008764E0 0x00AEEBA0): only their
  Virtual11 / 12 / 13 overrides respelled to the PDB signatures (`{}` bool bodies -> `{ return false; }`; `{ abort(); }` kept); their
  Virtual08 lines stay (slot 8 unchanged). 0x00650E98's test overrode only Virtual11. No other file under rebuild/ names the three placeholders.

## engine/CEngine.h (q12680 step 2: the CEngine override opt-in FABLE_RETAIL_CENGINE_OVERRIDES; add-only)
- New block before `private: void SetDefaultRenderStates();`: `CEngine();`, `virtual ~CEngine();` and the 78 CIEngine overrides of vtable
  0x012A0F3C not declared in the default branch (slots 3-84; Initialise / Uninitialise / the device-reset pair are default declarations),
  in slot order with the retail override address per line, from the ctor row's EngineNative model. A member another opt-in already declares
  sits under `#if !defined(<that opt-in>)` (RENDER_PASS26, SETPERSISTENTPRIMITIVE, GETSCRATCHBUFFERTEXTUREMANAGER, GETMEMORYPOOLS,
  CIENGINE_CONVERTWORLDPOS, CIENGINE_CONVERTSCREENPOS, SETSPOTLIGHTSHADOWMODE), so no member is declared twice. A user must also define
  FABLE_RETAIL_CIENGINE_RENDER (slot 8).
- Under the same macro, `private:` / `public:` lines around the existing OnPreDeviceReset / OnPostDeviceReset declarations (the existing
  lines themselves are untouched): both PDBs spell them private (?OnPreDeviceReset@CEngine@@EAEXXZ Ego_r 0x009B0650 / FableWin 0x02C9AA40,
  ?OnPostDeviceReset@CEngine@@EAE_NXZ 0x009B0360 / 0x02C9ABC0). Access does not change the vtable layout.
- Spelling proof (work/probe2/): a TU that defines a class derived from CEngine with the opt-in compiles at /Od and its vtables reference 84
  CEngine symbols (?<Name>@CEngine@@..., ??0 / ??1): all 84 are in the FableWin.pdb dump, 82 in the Ego_r.pdb dump; the other two
  (?GetStaticMapUpdatePassCount@CEngine@@UAEJXZ, ?PreChangeResolution@CEngine@@UAE_NXZ) are in the raw Ego_r.pdb and Ego_d.pdb
  (grep -ca), so every one has two PDB sources.
- No landed file defines FABLE_RETAIL_CENGINE_OVERRIDES (grep): equiv_check with both headers (work/equiv2.tsv) is line-for-line the
  same 108 rows / 122 files as CIEngine.h alone, so this block changes no includer. pin_asserts: 0 to pin (CEngine.h's asserts are all
  FABLE_STATIC_ASSERT_AT already); assert_names_check 0.
- No user yet: step 3 (the two rows) is blocked (NOTES).
- Other naming agent's overlay (NF245): read before adding; it has no CIEngine.h / CEngine.h copy.

## Later notes (both headers)
- Unwind-funclet rows: no landed row `#include`s "../src/compiled/<file>" of any of the 159 closure sources (work/funclets.txt is empty), so
  the closure list misses no funclet.
- After the equiv / recheck runs, two comment-only edits: CIEngine.h's slot-8 sentence ("all four together move 0x0058578B; this set of
  three does not", one more comment line) and CEngine.h's opt-in comment (the step-1 dependency). Comments do not reach the preprocessed
  text (cpp -P), and pin_asserts reports nothing to pin against the repo for either header, so the assert names are unchanged: the proof stands.
- Step 2 depends on step 1: FABLE_RETAIL_CENGINE_OVERRIDES needs CIEngine.h's slot 11-13 PDB declarations (this overlay's default) and
  FABLE_RETAIL_CIENGINE_RENDER, or the slot 8 / 11-13 overrides become new virtuals after slot 85. The probe's CIEngine-part vtable has
  86 entries (slot 0 the deleting dtor, 1 ..85), slot 8 ?Render@CEngine, 11-13 the three Convert* CEngine names, 85 ?Uninitialise@CEngine.
- Existing header debt the probe shows (not changed): slot 1 binds ?Validate@CIEngine@@UBEXXZ because CIEngine.h declares Validate in
  CIEngine itself, while retail slot 1 is CInitBaseClass::Validate 0x0099A340 (?Validate@CInitBaseClass@@UBEXXZ, the spelling 0x00B260B0's
  FABLE_RETAIL_CINITBASECLASS_VALIDATE gives). A port of the ctor row (step 3) must gate CIEngine.h's Validate declaration off under that
  macro, or its vtable binds the wrong name at 0x0099A340.
