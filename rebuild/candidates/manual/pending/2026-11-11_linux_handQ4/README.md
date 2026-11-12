# Chunk Q4 (fast-authoring lane, Opus 5.5): the 41 rows Q3 did not match

No rows matched (23 grades). The closest near misses:
- 0x007CCF50 CTCMultiStrike::ApplyEffectOnWeapon: DIFFER(1088v1088). Only one reload order differs, at 0x007CD314.
- 0x00497FD0 CProgressDisplay::DisplayCenteredText: DIFFER(1076v1076). The line-count spill slot and the placement of the fourth draw's Y load differ.
- 0x008F61A0 CHeightMap::Resize: DIFFER(518v530). Retail keeps sizeX+1 on the stack at [esp+1Ch]; ours keeps it in registers.
- 0x0078C900 CTCBerserk::InitAbility: DIFFER(1132v1152). The compiler's flag word for the && temporaries goes in ebx instead of [esp+10h].

Labels are in ghidra_out/labels_handQ4_2026-11-11.tsv: 6 names, each from both PDBs. Four header opt-ins are used only by the drafts here (hdr/README.md).
