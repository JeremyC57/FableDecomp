# Header overlay (NF33)

## engine/NPlayerGui_CDrawNPCName.h
Copy of rebuild/include/engine/NPlayerGui_CDrawNPCName.h (2026-11-16) plus one declaration-only opt-in:
- `FABLE_RETAIL_NPLAYERGUI_CDRAWNPCNAME_SHOWSCREENSHOTMESSAGE`: `void ShowScreenshotMessage();` (Ego_r.pdb 0x00699480 / FableWin.pdb 0x02114070
  ?ShowScreenshotMessage@CDrawNPCName@NPlayerGui@@QAEXXZ, retail 0x00657280 by the header's own donor-order note). Used by row 0x004379E0.
  No layout change; rows that do not define the macro see the repo header unchanged.
