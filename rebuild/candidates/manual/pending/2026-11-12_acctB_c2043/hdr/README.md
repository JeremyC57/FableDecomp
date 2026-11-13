# Header overlay, chunk 2043 (row 0x0064ED4F NPlayerGui::CDrawDamage::Draw)

All changes are opt-in or new files; the default text of every repo header is unchanged.

- `engine/CPlayerGuiDef.h` (copy of the repo file + opt-in `FABLE_RETAIL_CPLAYERGUIDEF_DAMAGE`): DigitPos / DigitHeroPos /
  DigitHeroOffset typed as `C2DVector` and DigitGraphics as `std::vector<long>` at the same retail offsets (+0x814 / +0x81C /
  +0x824 / +0x82C). Evidence: (1) Ego_r.pdb member types (struct_layouts_egor.tsv: C2DVector DigitPos / DigitHeroPos /
  DigitHeroOffset, vector<long> DigitGraphics); (2) retail Draw passes def+0x814 to GFAnchorToBottomRight(const C2DVector&)
  (0x0064EDC9), copies def+0x81C as a vector (0x0064EDE2), scales +0x824 / +0x828 (0x0064EE62) and indexes the start word of
  +0x82C (0x0064EF22). The flat `_X` / `_Y` default members and their asserts stay on the `#else` path.
- `engine/NPlayerGui_Graphics.h` (copy + opt-in `FABLE_RETAIL_NPLAYERGUI_ANCHORS`): the four GFAnchorTo* helpers with their
  Ego_r.pdb / FableWin.pdb spellings (`?GFAnchorTo...@NPlayerGui@@YI?AVC2DVector@@ABV2@@Z`); GFAnchorToTopLeft has its inline identity
  body (retail's uncalled out-of-line copy 0x00643C94 is byte-identical to it compiled out of line), the other three are declarations
  (landed rows 0x00643CA1 / 0x00643CDA / 0x00643D0C). The existing GFToScreen inline is unchanged.
- `engine/NPlayerGui_CDrawDamage.h` (new): NPlayerGui::CDrawDamage : CDrawBase with the nested class SDamageValue (Ego_r.pdb Duration /
  Change / Hero, FableWin.pdb ctor ??0SDamageValue@CDrawDamage@NPlayerGui@@QAE@JM_N@Z), DamageValues std::list<SDamageValue> at +0x28
  (retail: Draw 0x0064ED5D, the landed Update 0x00644CE2 / AddDamageChange 0x006472B2) and the PDB-named members Update /
  AddDamageChange / Draw. sizeof 0x2C asserted.
