# hdr/ -- chunk 840 header overlay

One header, one opt-in branch, default path textually unchanged (diffed against rebuild/include before hand-back: the repo copy
had not moved; only the added block differs).

## engine/NUISystem_CUIScreenUnitConverter.h
- What: an `#ifdef FABLE_RETAIL_CUISCREENUNITCONVERTER_VECTOR` block appended after the existing
  `GetBackBufferWidth / GetBackBufferHeight` declarations, declaring the two out-of-line vector converters
  `static C2DVector __fastcall ConvertVector(C2DVector v);   // 0x0052E580` and
  `static C2DVector __fastcall UnConvertVector(C2DVector v); // 0x0052E530` (by-value C2DVector argument built in its stack slot,
  hidden C2DVector result in ecx). No layout change, no reflow of existing lines.
- Evidence: Ego_r.pdb / FableWin.pdb `?ConvertVector@CUIScreenUnitConverter@NUISystem@@SI?AVC2DVector@@V3@@Z` (bound by the landed
  0x0052EEC0) and FableWin.pdb `?UnConvertVector@CUIScreenUnitConverter@NUISystem@@SI?AVC2DVector@@V3@@Z`; the UnConvertVector body
  is the exact inverse of ConvertVector (divide by the back-buffer size, multiply by the 640x480 screen size), called by 0x0052EA6B
  (ChangePositionDelta, row 0x0052EA30 of this chunk) and 0x0052EB2B (ChangeTargetPositionDelta).
- Used by: src/0052ea30.cpp (`#define FABLE_RETAIL_CUISCREENUNITCONVERTER_VECTOR` before the include; the row includes
  engine/C2DVector.h first through engine/NUISystem_CComponent.h).
- Includers rechecked read-only with the overlay: 0x0041CF47 NUISystemCManager_GetUIScale (the only landed includer) RELOCATION_MATCH
  (SIBLINGS.tsv). The overlay was confirmed active with a deliberate byte change that turned the row DIFFER(185v181), then restored.
