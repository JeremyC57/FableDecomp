# acctB hand-back, chunk 2043 (1 row matched)

Authored by an Opus 5.5 agent, re-verified by the acctB local coordinator. Nothing is landed.

| address | result | notes |
|---|---|---|
| 0x0064ED4F `?Draw@CDrawDamage@NPlayerGui@@QAEXABVCEnginePrimitivePrepareInfo@@@Z` | RELOCATION_MATCH 2314v2314, test PASS (g++ and VC7.1) | size TU (`#pragma optimize("s", on)`), no flags, no device; 4 grades, all in pass 1 |

Coordinator re-verification (cl 13.10.3077 under Wine, `FABLE_HDR_OVERLAY=hdr/`): `coordverify/` (grade, both test runs,
read-only vland: 1 win, no warnings of any kind). Purity and padded-struct greps are clean; the only pragma is the TU
mode line.

## Review items for landing

1. **Same-TU body of `NPlayerGui::CDrawBase::ScaleAlpha` (0x00643D5D)** is in the row file, out of line:
   `return PlayerGui->ScaleAlpha(alpha);` through FableWin's inline `CPlayerGui::ScaleAlpha`. Compiled out of line it
   gives retail's 29 bytes exactly (agent's measurement), and it is what makes VC7.1 schedule the DigitGraphics lookups
   and the ftol after the call, as retail does. The landed 0x00643D5D source is a free-function stand-in
   (`Cons_val_CEntry`): it can be re-authored with this member body (pair-landing candidate). The row's object also
   emits `CEnginePrimitiveHandle::Clear`; only the Leaf lands.
2. **Name fix** (`NAME_FIXES.tsv`): 0x00643D0C is `?GFAnchorToBottomRight@NPlayerGui@@YI?AVC2DVector@@ABV2@@Z` (both
   PDBs and that landed row's own Leaf); landed row 0x00658020 binds it as an invented `GFAPositionToScreen`.
3. **Labels** (`LABELS.tsv`): the row, ScaleAlpha 0x00643D5D, `VALUE_DISPLAY_DURATION` 0x013BA860, the uncatalogued
   `GFAnchorToTopLeft` 0x00643C94 (identity body, byte-identical to the row's inline), and -- marked probable --
   `GFToScreen(const C2DVector&)` 0x00643BE2, catalogued as `NUISystem::CComponent::CanMove` (looks propagated).
4. **Headers** (`hdr/README.md`; default text unchanged): `engine/CPlayerGuiDef.h`
   `FABLE_RETAIL_CPLAYERGUIDEF_DAMAGE` (DigitPos / DigitHeroPos / DigitHeroOffset as C2DVector, DigitGraphics as
   `std::vector<long>` at +0x814 / +0x81C / +0x824 / +0x82C; the flat members stay on the `#else` path),
   `engine/NPlayerGui_Graphics.h` `FABLE_RETAIL_NPLAYERGUI_ANCHORS` (four GFAnchorTo* helpers), new
   `engine/NPlayerGui_CDrawDamage.h` (CDrawDamage : CDrawBase, nested `SDamageValue`, DamageValues list at +0x28,
   sizeof 0x2C asserted).
5. Behaviour kept as retail has it: the handle vector is resized to size()*4 but the index advances by 3 per value, so
   a 4-digit value's last handle is the next value's first.

## How to land

`rows/0064ed4f.cpp` + `rows/0064ed4f.test.cpp` with the three `hdr/engine/` files, then `payload.json` through your
usual vland. `LESSONS.md`: when retail shows inline-lookup loads after a same-TU callee, give that callee its real body
before rewriting vector access as raw pointers (the landed CDrawPerceivers::Draw 0x0064499C used a raw `Start[]` model
for the same symptom).
