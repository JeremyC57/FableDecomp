# NF289 overlay headers

## engine/CPlayerManager.h (base: orig/hdr/engine/CPlayerManager.h = repo HEAD copy, 2026-11-23)
- New add-only opt-in `FABLE_RETAIL_CPLAYERMANAGER_FLAT_CREATEPLAYERS`, in the flat (`#else`, pack(1)) branch only, after the
  FREEZEPLAYERCONTROLS block and before the class's closing brace: a comment and the declaration `void CreatePlayers();` (0x0044A530).
  - Spelling: Ego_r + FableWin name sets `?CreatePlayers@CPlayerManager@@QAEXXZ`; the landed self row 0x0044A530 defines it.
  - User: binder 0x004166A8 only (CMainGameComponent::CreatePlayers; its CPlayerManager_Methods stand-in derives the flat class).
- No existing line edited; no layout change (a non-virtual member function in a pack(1) class). Every assert in the flat branch is
  already `FABLE_STATIC_ASSERT_AT`, so nothing moves (pin_asserts has nothing to do).
- equiv_check (evidence/equiv_r4.txt): the header's transitive landed includers plus every row including a relative source path, 2112
  rows: 0 differ. The planted-token control (a member typedef just above the opt-in, outside it) flags 493.
- Other agents' overlays: NF288's hdr/ held no CPlayerManager.h when checked (2026-11-23).
