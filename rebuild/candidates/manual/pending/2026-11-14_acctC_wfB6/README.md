# acctC hand-back wfB6 (parked rows + fix-up, 2026-11-14)

| address | name | bytes | grade | test |
|---|---|---|---|---|
| 0x005379F5 | NUISystem::CList::ComputeChildrenVisibility | 2,102 | RELOCATION_MATCH | PASS |

Coordinator-verified after the fix-up (`coordverify.log`). The verifier's objection (stand-in callee bodies) is fixed: the
three visible callee bodies are now the real texts, each graded against its own retail copy:
- 0x0052E470 ConvertX / 0x0052E4D0 ConvertY: the landed rows' bodies (`float t = x / ScreenWidth; return t * BackBufferWidth;`),
  RELOCATION_MATCH each.
- 0x00535CD0 C2DBoxF::ContainsPoint: `bool` return (both PDBs `_N`; callers test `al`) with one return expression, MATCH.
REVIEW ITEMS:
- The landed 0x00535CD0 row returns `long`: respell it to the bool text (same bytes). NAME_FIXES.tsv.
- Landed 0x0052E470 / 0x0052E4D0 are named Physics_ScaleFloatByGameSpeed / LengthScaleConversion (__stdcall); they are
  NUISystem::CUIScreenUnitConverter::ConvertX / ConvertY (static __fastcall, same bytes), and their globals are the class's
  static members (IsUsingRelativeCoords, BackBufferWidth/Height, ScreenWidth/Height). NAME_FIXES.tsv.
- FITTED: four-float `C2DBoxF` constructor for `child_box`; GetTLPos()/GetBRPos() as the ContainsPoint arguments.
- `#pragma optimize("s", on)` as 41 of 45 landed CList siblings.
The other two fix-up rows (0x00772BB0, 0x007DE710) are still in `../2026-11-14_acctC_wfB6open/matched_needs_fixup/`.
