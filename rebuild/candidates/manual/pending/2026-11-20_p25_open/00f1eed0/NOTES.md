# 0x00F1EED0 PlayWave (p25Q, 2026-11-21): natural text decoded, inline split open
- Natural text (two sources: the 360 port's names and Ego_d's line table): top-tested outer while, plain for loops, no guards /
  `&&1` / `||0` / asserts (natural_base_n0.cpp). DIFFER(11221v9262), 38 exit-kind mismatches at default. Ego_d contradicts all three
  fitted devices of the 9262v9262 acctC draft (if-do `>=1`, `||0` bottom, 15 dead asserts).
- /d2inlS: S3..S8 = default, higher S only worse. /d2inlT: erratic (best T55, 20 mismatches). T x S: no improvement. Not
  threshold-ordered (site_table_p25Q.txt: 65 T-check pads, per-site kinds).
- Retail calls ~CTimer at ten sites one conditional level deeper than its expanded ones in the same regions; regions would be
  needed at 10 sites and 5+ vector-dtor flips stay wrong on every base. No regions added.
- Caller-size hypothesis refuted: every Ego_d oversize line in TLC-common code is a real statement (only the post-TLC tail
  1512..1536 is unexplained), so Ego_d-decoded dead asserts = 0; moving the draft's 15 asserts from top to end is byte-identical
  (penalty is function-global); adding 15/45 asserts to the natural text leaves 38 (20 at T55) mismatches. The colder sites come
  from c2's frequency model. Open question for the c1xx inline-info harness (p25R).
