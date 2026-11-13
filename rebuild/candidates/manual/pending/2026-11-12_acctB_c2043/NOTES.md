# Hand chunk 2043 (NPlayerGui::CDrawDamage .. NPlayerGui::CDrawDamage): 1 of 1 byte-exact

## What matched (1)
- 0x0064ED4F `NPlayerGui::CDrawDamage::Draw(const CEnginePrimitivePrepareInfo&)` -- RELOCATION_MATCH 2314v2314, behaviour test PASS
  (g++ and VC7.1). Draws each queued damage value as 1..4 red digit sprites (CEnginePrimitive2DSprite, layer 0x43, blend alpha,
  flags 2, SetPersistentPrimitive flags 0xC0) into PrimitiveHandles[index + n], index advancing by 3 per value (retail resizes to
  size()*4, so a 4-digit value's last handle is the next value's first: kept as retail). Grades: 4 in pass 1 (2 DIFFER, 2 MATCH).
  Shape levers, in order of effect:
  1. **Same-TU ScaleAlpha body** (the decisive one): c1 evaluates inline calls in an argument list (vector operator[], begin(),
     GFToScreen, C2DVector ctors) ahead of the pushes; with CDrawBase::ScaleAlpha only declared, the DigitGraphics start load and the
     ftol stay ahead of the ScaleAlpha call. Retail has them after it. With ScaleAlpha's real body visible in the TU (it is in the same
     retail TU at 0x00643D5D; out of line: callee size > 15 in a size TU) c2 knows the call writes no memory and schedules the
     lookups after it (probe g1/g2; grade 3 went from 2356v2314 to RELOCATION_MATCH). Body: `return PlayerGui->ScaleAlpha(alpha);`
     through the FableWin.pdb inline CPlayerGui::ScaleAlpha (FABLE_RETAIL_CPLAYERGUI_QUEST_DRAW), which compiles to retail's 29 bytes.
  2. A named `long duration` for the reload-and-compare / multiply, the 8-bit arm reading `it->Duration` (probes a1..a8: a named
     duration in the arm too gives `lea ecx` + reuse of al; it->Duration everywhere gives `cmp [mem],ebx` and a reload for the multiply).
  3. A named `float time` keeps (float)Duration on the x87 stack for both the alpha and the offset (`fld [T] ; fdivr st(1)`).
  4. Hero offset built member by member: `def.DigitHeroOffset * time` (C2DVector::operator*) put def+0x824 in a CPU location
     (`add eax,824h ; fmul [eax]`); retail uses `fmul [eax+824h]`.
  5. Hero position = inline `GFAnchorToTopLeft(def.DigitHeroPos)` (identity, the temp-then-copy at [ebp+3Ch]); sprite position =
     inline `GFToScreen(draw_pos)` (the temp built ahead of the pushes).
  6. Three / four digits: the first digit's graphic is looked up into a named local before the pull-left and the clamp (retail's ftol
     precedes the subtract and the graphic is parked at [ebp+54h] across the clamp branch).
  7. The `Change >= 0` guard around the `< 10 / < 100 / < 1000 / < 10000` else-if chain (`test ah,1 ; jne` to the shared tail).

## Names and evidence
- Row: Ego_r.pdb 0x00691CB4 and FableWin.pdb 0x020E690D `?Draw@CDrawDamage@NPlayerGui@@QAEXABVCEnginePrimitivePrepareInfo@@@Z`
  (non-virtual, one reference argument = `ret 4`); members from Ego_r.pdb CDrawDamage / SDamageValue / CPlayerGuiDef.
- Callees, all relocation-checked against retail (rel.py: every call `same`): PeekGuiDef 0x00438570, for_each 0x0043F360 (+ Clear
  0x0041BE70), vector<CEnginePrimitiveHandle>::resize 0x00443F90, ScaleAlpha 0x00643D5D, CEnginePrimitive2DSprite 0x00493AC2 (the
  enum-typed PDB spelling already bound by 0x0064F6DC), __ftol2, VALUE_DISPLAY_DURATION 0x013BA860.
  GFAnchorToBottomRight 0x00643D0C is spelled with its PDB name; reloc_bindings has the invented `?GFAPositionToScreen@@...` from row
  0x00658020 -> NAME_FIXES.tsv.
- No descriptive (made-up) names. Labels proven: LABELS.tsv (incl. GFAnchorToTopLeft 0x00643C94 and the probable GFToScreen 0x00643BE2).

## Parked for Fable 5.1 (0)

## Skipped
- none. Extent confirmed: previous function (CDrawPerceivers ctor) ends `ret 8` at 0x0064ED4C; this row's only `ret 4` is its last
  instruction (0x0064F656); the CDrawQuestInfo ctor starts at 0x0064F659. No SPLITS.tsv needed.

## Label corrections
- 0x00643BE2 catalogued as `NUISystem::CComponent::CanMove` is probably NPlayerGui::GFToScreen(const C2DVector&) (see LABELS.tsv).
- 0x00643D5D's landed source models ScaleAlpha as a free `Cons_val_CEntry` stand-in; its real body is `return PlayerGui->ScaleAlpha(alpha);`
  on CDrawBase (proven byte-identical here).

## Header debt
- New overlay header engine/NPlayerGui_CDrawDamage.h and opt-ins in CPlayerGuiDef.h / NPlayerGui_Graphics.h (hdr/README.md).
- CEnginePrimitivePrepareInfo is used through its default (pointer) branch; the PDB reference-typed opt-in gives the same code here
  (not needed).

## Commands
- Grade: `bash tools/gr.sh <tag>` (q.py with the Leaf + FABLE_HDR_OVERLAY, then md.py masked summaries).
- Free probes: `bash tools/pr.sh probe/<x>.cpp '<leaf>'` (compile-only + disassembly of one symbol, no comparison with retail).
- Call targets: `python3 tools/rel.py 0064ed4f '<leaf>'` after a grade.
