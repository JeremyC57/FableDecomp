# acctB hand-back, near-miss retry 3016 (1 previously parked row: matched)

Authored by an Opus 5.5 agent starting from the parked source of r3005 (earlier attempts: c2006, r3002, r3005),
re-verified by the acctB local coordinator. Nothing is landed.

| address | result | notes |
|---|---|---|
| 0x00550DC0 NUISystem::CTable::Draw | RELOCATION_MATCH 1399v1399, test PASS (g++ and VC7.1) | **supersedes the parked copies in `2026-11-11_acctB_c2006`, `_r3002` and `2026-11-12_acctB_r3005`** (was DIFFER 1399v1399 by two swapped frame-slot pairs). Plain C++, no flag, no device. 3 grades |

What changed against the parked source (I diffed them; the colour block only, no instruction changes):

```cpp
CRGBFloatColour parent_colour(parent->GetRenderColour());
CRGBFloatColour colour(Colour);
CRGBFloatColour render_colour = colour * parent_colour;
RenderColour = render_colour.GetRGBColour();
for (...) {
    colour = CRGBFloatColour(Children[i]->GetColour()) * render_colour;   // the table's colour variable is reused
    CRGBColour child_render_colour = colour.GetRGBColour();
```

This is the statement shape of the landed sibling CComponent::UpdateColour 0x0052F900. Because the loop reuses `colour`,
the table-colour value and the loop colour are one frame-packer node, which gives retail's four slots (parent R
[esp+74h] 0x00550EFE, table R [esp+3Ch] 0x00550F53, alpha [esp+38h] 0x00550F59, blue [esp+34h] 0x00550F63). The packer
replay and the what-if table are in `NOTES.md`; the mechanism is in `LESSONS.md` for GOTCHAS.

Coordinator re-verification (cl 13.10.3077 under Wine, `FABLE_HDR_OVERLAY=hdr/`): `coordverify/` (grade, both test runs,
read-only vland: 1 win, no RELOC_IDENTITY / RELOC_CONFLICT, no warnings). Purity and padded-struct greps are clean. `hdr/`
is byte-identical to r3005's overlay apart from README.md (I ran `diff -rq`); the agent's diagnostic `PROBE_*` operator
variants are gone from CRGBFloatColour.h.

Review items for landing:

- Descriptive names: the locals (`parent_colour`, `colour`, `render_colour`, `child_render_colour`, `position`, ...)
  and the parameter names; `colour` / `render_colour` mirror UpdateColour.
- The repo header models CRGBFloatColour's product as a free `operator*`; FableWin.pdb has a const member
  (`??DCRGBFloatColour@@QBE?AV0@ABV0@@Z`). Byte-neutral for this row; worth fixing at the next header merge.

To land: `hdr/` (r3005's = c2006's overlay with r3001's CMatrix3x4.h superset): the row uses
`FABLE_RETAIL_CRGBFLOATCOLOUR_INITIALISE_CTOR`, `FABLE_RETAIL_CTABLE_DRAW`, `FABLE_RETAIL_STLP_MAP`. Rebuild the oracle from
Fable.exe (no retail bytes are in this folder).
