# agentNF29 header overlay

Full copies of the current repo headers with `#ifdef` additions only (default branches unchanged). Grade with
`FABLE_HDR_OVERLAY=$S/w22/agentNF29/hdr`.

| Header | New opt-in | Used by | What it adds |
|---|---|---|---|
| `engine/NPlayerGui_CDrawBase.h` | `FABLE_RETAIL_NPLAYERGUI_CDRAWBASE_SCALEALPHA` | row 0x00643D5D (CDrawBase::ScaleAlpha itself) | The `ScaleAlpha(unsigned char) const` declaration (0x00643D5D, `?ScaleAlpha@CDrawBase@NPlayerGui@@QBEEE@Z`, the same declaration as the HANDLES opt-in) without the HANDLES opt-in's `CEnginePrimitiveHandle` element type, which needs the persistent-primitive headers. The block is skipped when HANDLES is defined. Declaration only, no layout change. |
| `engine/CUserProfileManager.h` | `FABLE_RETAIL_CUSERPROFILEMANAGER_SETSOUNDASMUTED` | row 0x00408340 (SetSoundAsMuted itself) | `void SetSoundAsMuted(bool);` (Ego_r.pdb 0x004062F0 + FableWin.pdb 0x0187E2D0 `?SetSoundAsMuted@CUserProfileManager@@QAEX_N@Z`), placed after the SETTERS block. Declaration only, no layout change. |
