# chunk 807 header overlay

All three headers keep the repo default path textually unchanged (copied fresh from rebuild/include, then diffed again at hand-back:
the repo copies had not moved; `diff` shows additions only, no `<` lines).

## engine/CQ_UndeadRisingScript.h (new)
- NScript::CQ_UndeadRisingScript : CLevelScriptBase<CQ_SunnyvaleMasterData>, the whole header behind `#ifdef FABLE_RETAIL_NSCRIPT`
  (no `#else`, no `#error`).
- Evidence: Ego_r.pdb layout (struct_layouts_egor.tsv) moved to retail offsets (-0x14 for the 0x48 retail CLevelScriptBase, -4 after
  the first STLport vector), checked against Main 0x00E0B160 (+0x50, +0x5C, +0x60, +0x6C, +0x48/+0x4C), the landed LampChecker
  0x00E0C4C0 (+0x54, +0x60) and WatchForTermination 0x00E0C350 (+0x52/+0x53); vtable 0x012DFD3C (RegisterMain, Main, Init) from the
  landed ctor 0x00E0AFE0. sizeof 0x78. quest_status enumerators are descriptive (the PDB type is unnamed).
- Includers: none landed (the 42 sibling rows keep local models). Used by src/00e0b160.cpp (RELOCATION_MATCH).

## engine/CQ_NewOakValeIntroScript.h (opt-in FABLE_RETAIL_CQ_NEWOAKVALEINTROSCRIPT_VICTIM_LIVEFATHER)
- Adds CQ_NewOakValeIntroScript::CNOVI_Victim and ::CNOVI_LiveFather (entity scripts; Alloc, GetParentScript, private Main/Init,
  ParentClass +0x14, PMasterData +0x18, Victim DisplayedGameInfo +0x1C / DoneThanks +0x1D, LiveFather PenniesGiven +0x1C; 0x20 each)
  in one block after the default body (before its sizeof assert), behind its own macro.
- Evidence: Ego_r.pdb layouts (entity base 0x14 in both builds), entity vtables 0x012D87B8 / 0x012D8388 with their Allocs
  0x00DAEDE0 / 0x00DAC2C0, retail Main reads of the offsets.
- Chunk 806 edits this header too (its ENTITIES2 branch, already in the repo copy I started from). My addition is a separate opt-in
  macro, so the two merge as independent `#ifdef` blocks.
- Landed includers rechecked read-only with this overlay (vland --recheck-landed): 00db09e0, 00db2b10, 00db6c60 -> RELOCATION_MATCH PASS.

## engine/CV_TourGuideScript.h (opt-in FABLE_RETAIL_CV_TOURGUIDESCRIPT_GUIDE)
- Three opt-in blocks: the includes/forward declaration it needs (top), four script methods inside the class (MoveToNextWaypoint
  0x00EE6850, WatchForNoFollowers 0x00EE6A40, WatchForClosingTime 0x00EE4A00, WatchForGuideKilled 0x00EE4A70), and the
  CTourGuideGuide entity class (vtable 0x012F511C, Alloc 0x00EE4BB0; saveWaypointIdx +0x1C, lastRandomSpeechIdx +0x20).
- Evidence: Ego_r.pdb / FableWin.pdb name sets, Ego_r layout of CTourGuideGuide, landed Init 0x00EE4C60 (sets both members to -1).
- Landed includer rechecked: 00ee42a0 -> RELOCATION_MATCH PASS. Used only by the parked differ/00ee57b0.cpp.

## engine/CV_SickChildScript.h
- Unchanged. The parked 0x00ECF7F0 uses the default body. Chunk 806 edits this header in its own overlay; nothing of mine conflicts.
