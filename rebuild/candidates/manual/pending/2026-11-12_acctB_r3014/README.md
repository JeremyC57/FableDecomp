# acctB hand-back, near-miss retry 3014 (1 previously parked row: matched)

Authored by two successive Opus 5.5 agents (the second continued in the first one's directory) starting from c2031's
parked source, re-verified by the acctB local coordinator. Nothing is landed.

| address | result | notes |
|---|---|---|
| 0x00B65A20 CEngineSkyRenderer::RenderOuterSky(float) | RELOCATION_MATCH 1893v1893, test PASS (g++ and VC7.1) | **supersedes `2026-11-12_acctB_c2031/rows/parked/00b65a20.cpp`** (was DIFFER 1897v1893, three spots in the shader tail). No header change. 2 grades in this retry (plus c2031's 6) |

What changed against the parked source (I diffed them; two things, both in the row):

1. `#define FABLE_RETAIL_CARRAY` -- the EXISTING opt-in of `rebuild/include/engine/CArray.h` that makes
   `CArray::operator[]` go through `begin()`;
2. the two theme colours are copied straight into named locals,
   `CRGBFloatColour colour0 = params.ThemeChannels[0].Colours[12];` / `colour1 = ...Colours[13];`, instead of binding a
   const reference and then copying.

One cause for all three residues: with the begin()-based operator[] the element address is a 3-address form in c2's IL
instead of a two-address `add`, which (a) removes the scheduler tie of the blend vector's dead Y store (0x00B65E42), (b)
changes what the scheduler's rename walk visits so the `UpdateFlags |= 2` temporary lands in ebx (0x00B65E5A), and (c)
makes colour 13's `add eax,0D0h ; mov ecx,[eax]` order hard (0x00B65E97). The mechanism and the free probe that showed
it (`tools/walk2.sh` on the agent side) are in `NOTES.md` / `LESSONS.md`.

Coordinator re-verification (cl 13.10.3077 under Wine, `FABLE_HDR_OVERLAY=hdr/`): `coordverify/` (grade, both test runs,
read-only vland: 1 win, no RELOC_IDENTITY / RELOC_CONFLICT). Purity and padded-struct greps are clean; `hdr/` is c2031's
overlay unchanged apart from README.md.

Review items for landing:

- **FABLE_RETAIL_CARRAY is a per-row choice of a shared inline's body** (two texts of CArray::operator[] exist in the
  repo header). This row needs the begin() form; other landed rows use the default. Worth a note in the header debt list:
  retail compiled one text.
- NAME_FIXES.tsv / LABELS.tsv are c2031's (0x00989B50 is the CRGBFloatColour overload
  `?SetUserVertexShaderConstant@CShaderRenderManager@@QAEXKABVCRGBFloatColour@@@Z`; landed rows that spell it with
  `PBVC4DVector` need respelling; 0x0041B9C0 `CResource::TouchResource`; the others listed in c2031's README).
- Descriptive names: locals only.

To land: `hdr/` = c2031's overlay (see `2026-11-12_acctB_c2031/README.md` for its review items, incl. the CIDrawWorld /
CWorld.h slot-order debt). Rebuild the oracle from Fable.exe (no retail bytes are in this folder).
