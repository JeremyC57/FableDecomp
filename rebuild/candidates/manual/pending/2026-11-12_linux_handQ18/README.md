# Chunk Q18 (Q17 reworks and register-tie near misses, Opus 5.5)

6 of 20 rows matched, all on the first grade after the port, and landed in landing df:
- 0x008BF500 FinishAction
- 0x00555A90 UpdateVisibleList
- 0x00DE3CE0 CQ_RansomVictimScript::CNPCComment::Main
- 0x008886E0 BlendInLevel
- 0x009644F0 StealStealableItem::OnReplace
- 0x00ED94D0 WatchForResetWanderCentre

14 are in differ/ with the residue on line 2:
- 0x008780D0 and 0x006FE7C0 (680v680, one register reload order, same template body).
- 0x00837220 UpdateFacingDirection (716v716, three scheduling residues).
- 11 not yet ported.

Chunk Q19 takes all 14.
