# NF254 header overlay (2026-11-22)

Base copies in ../orig/ (copied from the repo working tree at start). Each change is one new add-only `#ifdef` block under a new
member-specific macro that no landed row defines; the default path is token-identical (equiv_check below).

## stlport/fable_containers.h
- New opt-in `FABLE_RETAIL_STLP_DESTROY_AUX_PAIR_LONG_CCHARSTRING_SIZE_BODY` (row 0x0074FCE0), appended at the END of the
  file (just before the closing `}` of namespace std), so no existing line moves (a first placement next to `pair` shifted `__LINE__`
  for two tests that `#define abort() fixtureAbort(__LINE__)`: 0x0064E978 / 0x0064AE80; moved, equiv rerun).
  Content: `#pragma optimize("s", on)` + the explicit specialization `template<> inline void __destroy_aux<pair<long,CCharString>*>(...)`
  with the generic STLport _construct.h body + `#pragma optimize("", on)`. Size-pragma scaffolding (EXTRA_RULES 2026-11-20):
  (1) 0x0074FCE0's own leaf grades the same with and without it (RELOCATION_MATCH both; only the __destroy_aux COMDAT changes:
  36 B NEQ -> 29 B EQ v 0x004DD782); (2) the owner row 0x004DD782 sets `#pragma optimize("s", on)` itself; (3) the comment says the block
  ends with optimize("", on) and the rows using it carry no row-head optimize pragma. Needs CCharString complete (fable_string.h first).
- pin_asserts.py: 0 pinned (no FABLE_STATIC_ASSERT after the insertion point).
## engine/CLipSyncDataBank.h
- New opt-in `FABLE_RETAIL_CLIPSYNCDATABANK_UPDATE` inside the existing FABLE_RETAIL_CLIPSYNCDATABANK_CLASS branch: `virtual void Update();`
  (declaration only; Ego_r.pdb / FableWin.pdb ?Update@CLipSyncDataBank@@UAEXXZ; an override of CBankFile::Update, no new slot). Row 0x00A427A0.
- pin_asserts.py: 5 pinned (the five FABLE_STATIC_ASSERT lines of the CLASS branch moved 6 lines -> FABLE_STATIC_ASSERT_AT(45..49, ...)).
## engine/CEngineSkyRenderer.h
- New opt-in `FABLE_RETAIL_CENGINESKYRENDERER_BUILDBUFFERS_CALLEES` (private, after the BUILDCLOUDMESH block): BuildOuterSkyMesh /
  BuildBaseBandMesh (CTVertexColTex1**, unsigned short**, long, long) and BuildStarFieldVB(), both PDBs. Declarations only. Row 0x00B620A0.
- pin_asserts.py: 0 pinned.
## equiv_check
- Run 2 (all three overlay headers in a hard-link copy of rebuild/include, v the repo rebuild/include): 57,125 rows checked (sources and tests), 0 differ (work/equiv2.log / equiv2.tsv). Run 1 (fable_containers.h with the opt-in placed mid-file) showed 2 tests differing on __LINE__ only (0x0064E978, 0x0064AE80): moved to the end of the file.
- Landed includers: CLipSyncDataBank.h 6 files, CEngineSkyRenderer.h 20, fable_containers.h most of the tree; none defines any of the three
  new macros (grep of rebuild/src, rebuild/include, rebuild/tests: 0 hits), so none changes. No other agent overlay has these headers
  (NF253's hdr/ holds only engine/CThingCreatureBase.h).
