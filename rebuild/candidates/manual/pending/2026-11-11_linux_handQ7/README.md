# Chunk Q7 (parked near misses, Opus 5.5)

**Landed: 5 rows**
- 0x0086BFB0, in landing cj.
- 0x0047E651, 0x0047E9AE, 0x0047F30F and 0x00697490, in landing cl, after they were reworked onto shared headers.

**Not landed**
- 0x00A1CC30 matches only through a stand-in, so it is not landable.
- The rest are same-length DIFFERs. differ/ holds their latest drafts; STATUS.tsv notes the gap for each.

LESSONS.md holds the general lessons, also copied into GOTCHAS. The main one: redundant float parentheses move an x87 residue by one scheduling slot.
