# Chunk Q16 (GetInterface-model reworks and register-tie near misses, Opus 5.5)

3 of 20 rows matched: 0x00957DB0 CAIStateGroup_MinionBehaviourCrowdBase::Update, 0x00774730 CTCShop::SellItem and
0x008378D0 CThingTrackNode::SetAsLinkedTo, landed in landing db. 13 are in differ/ with the residue on line 2:
- Q15's two closest (0x0090C900, 0x00738A80) are still one instruction off and are now set aside.
- 0x00F3ABF0 and 0x00A90260 were ported.
- Nine were regraded only.

4 were not worked. Chunk Q17 takes the nine regraded rows (this/iterator swaps; try the SetAsLinkedTo IL-reference lever) and the
4 unworked ones.
