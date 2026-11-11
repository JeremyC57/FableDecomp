# Chunk Q5 (Opus 5.5): deep pass on the four closest fast-lane near misses

No rows matched. The four rows used 30 grades in total. Each row's best source is in differ/, with the remaining diff on line 2.
- 0x00497FD0 CProgressDisplay::DisplayCenteredText: DIFFER(1076v1076), ratio 0.928 -> 0.967.
  - Fixed: the line-count spill now matches.
  - Left: the copy order of `shadow = position` in the fourth draw (0x00498328-0x0049834B).
- 0x007CCF50 CTCMultiStrike::ApplyEffectOnWeapon: DIFFER(1088v1088). The edi/esi reload order at 0x007CD314 is unchanged under about 12 code-neutral spellings.
- 0x008F61A0 CHeightMap::Resize: DIFFER(518v530). Retail keeps sizeX+1 in a stack slot.
- 0x0078C900 CTCBerserk::InitAbility: DIFFER(1132v1152). The flag word for the `&&` temporaries wins ebx in ours: c2prio gives it priority 11, against 10/9/9 for the result bools.

Two header opt-ins in hdr/engine/CHeightMap.h are used only by the draft: FABLE_RETAIL_CHEIGHTMAP_GETEXTRACELLS and FABLE_RETAIL_CHEIGHTMAP_INDEXFLIP.
