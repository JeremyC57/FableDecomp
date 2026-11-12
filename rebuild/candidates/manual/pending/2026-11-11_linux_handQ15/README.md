# Chunk Q15 (port-first near misses, Opus 5.5)

2 of 24 rows matched: 0x0092E130 CAIStateGroup_MinionSleep::IsRunnable and 0x00713D50 CActionFollowCreature::SetAsSneaking,
landed in landing cz. 5 more are ported onto the shared headers but still DIFFER: 0x0090C900 (345v345, one jump), 0x00738A80
(177v177), 0x00AC7CB0, 0x0086B9D0 and 0x00AC7300. The other 17 were regraded only. All 22 are in differ/ with the residue on
line 2. In both matches the parked "register tie" was the local GetInterface model, not colouring. Q16 reworks the two closest
and the six rows that carry the same model.
