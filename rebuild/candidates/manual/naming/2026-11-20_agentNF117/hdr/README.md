# NF117 header overlay

No overlay: 0 opt-ins, no header copies in hdr/ (orig/engine/ holds untouched base copies of NUISystem_CMenuEntry.h and
NUISystem_CClickable.h, for reference only).

- 0x0053DFE0 uses engine/NUISystem_CMenuEntry.h as it is in the repo: `GetDataFromDef()` (0x0053DA30) is already declared there
  unconditionally, the members are already the retail layout (IsAvailable +0x198, PUIActionParams +0x19C ... PActionsOnVisible +0x1B8).
- pin_asserts.py: not run (no header edited, nothing to pin).
- Header users: no overlay change, so no landed includer of either header is affected.
- Note: no rule required it, but hdr/ holds no header copies (0 opt-ins); orig/engine/ keeps the untouched bases.
