# NF291 overlay headers

## engine/CTCQuestCard.h (base: orig/hdr/engine/CTCQuestCard.h = repo HEAD copy, 2026-11-23)
- New add-only opt-in `FABLE_RETAIL_CTCQUESTCARD_FLAT_ISVIGNETTE`, in the flattened (`#else`, pack(1)) branch only, after the last member
  (`RenownReward` +0x30) and before the class's closing brace: a comment and the declaration `bool IsVignette() const;` (0x007022E0).
  - Spelling: Ego_r + FableWin name sets `?IsVignette@CTCQuestCard@@QBE_NXZ`; the retail branch already declares it under
    FABLE_RETAIL_CTCQUESTCARD_ISVIGNETTE (a distinct macro name, so the two branches never collide).
  - User: binder 0x00896A30 only (CGameScriptInterface::SetQuestCardObjective; its QuestCardMethods stand-in derives the flat class).
- No existing line edited; no layout change (a non-virtual member function in a pack(1) class). The flat branch is "the landed default,
  unchanged" (hand-corrected generated header, no do-not-edit marker there) and every assert in the file is already
  `FABLE_STATIC_ASSERT_AT`, so nothing moves (pin_asserts has nothing to do).
- Gate (prefix ruling: generated do-not-edit branches are T3): CTCQuestCard.h line 3 reads "GENERATED ... then hand-corrected to retail"
  and its flat branch (line 135) says "flattened Ego_r layout (the landed default, unchanged)", with no do-not-edit line. CTCBuyableHouse.h
  (ruled T3) has "GENERATED ... -- do not edit by hand." at line 137, its flat branch; CPlayerManager.h (NF289, landed) has the same
  marker at line 65. So this branch is not a marked one.
- equiv_check (evidence/equiv_r1.txt): the header's landed includers plus every row including a relative source path, 1,407 rows: 0 differ.
  The planted-token control (a member typedef just above the opt-in, outside it) flags 10.
- Other agents' overlays: NF290's hdr/ was empty when checked (2026-11-23).
