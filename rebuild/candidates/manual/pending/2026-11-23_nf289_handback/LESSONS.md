# NF289 lessons (2026-11-23)

- **tri3's STALE test misses most stale witnesses.** A binder counts as stale there only when none of the stand-in's tokens appear in its code.
  A real class token, such as `CTCSoundPlayer` or `CThing`, masks a missing identifier like `PlayAttachedSampleByName_0066b120`.
  tools/tri4.py flags a binder whose stand-in identifier itself is gone. It found 0x0066B1D0 (two entries) and 0x00ACD840 stale, and
  0x00EE3EE0 (identifier only in a comment). Template and operator spellings have no usable token, so compile their binders and run objbind.
- **Check headers for existing declarations first.** Many `X_Methods : RealClass` stand-ins date from before the shared header gained
  the member. Grep the header for the member name before writing anything: CPlayerManager.h already had
  `FABLE_RETAIL_CPLAYERMANAGER_MULTIPLAYERCOLOUR`, and CTCBuyableHouse / CTCDoor / CTCQuestCard / CTCCombat had their declarations too.
  Often the header copy sits in the retail branch and the binder uses the flat one, so check which branch the binder compiles.
- **A row-local model with the real class name takes the member directly.** Move the declaration from the `_Methods` stand-in into
  that local class (0x00B92070 CShadowClipVolume, 0x009F35D0 CSurface). Byte-neutral, no header change.
- **A Methods-derived binder cannot reach a private base member.** Read the PDB access (`AAE`) before planning a fix: such a binder
  is a self port.
- **An opt-in that spells `class ::T` needs T declared first.** VC7.1 and clang both reject the qualified elaborated name otherwise.
  A one-line forward declaration before the include fixes it (0x00644424), and objsec stays ALL_EQ.
- **Keep line counts in a changed row.** Merge two comment lines, or put a comment on a `#define` / `#pragma` line, to make room for a
  new line. Every later line, and so every `FABLE_STATIC_ASSERT` typedef name in the row, keeps its number.
- **Linking against the owner row can expose its own stand-in self.** An unresolved PDB spelling after the respell, which is also
  unresolved for the existing PDB binder, is not a regression. Record it in NAME_FIXES as an owner self port.
- **Run zs.py with a planted-error control.** `cl /Zs` returns in under a second under the persistent wineserver; work/zs_ctl.log shows
  the control failing.
