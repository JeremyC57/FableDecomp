# tools (2026-11-09 session; adapted from chunk 883's copies of the hand886/hand885/hand890 tools)
- lb.sh <probe>...   grade probe/<name>.cpp one at a time (gp.sh, own row only, timeout 240) + LEADERBOARD.tsv line
- cmpx.py <obj> 0061e6d0 <leaf> [--segs [-v]] [--first N] [--mask none|regs|frame|both]   shares / call-segment diff v retail
- bcalls.py <obj> [b]   call-list diff v retail (c.txt), names shortened; `b` prints ours only
- odiff.py <a.obj> <b.obj> [--mask frame|both]   our-v-our instruction diff (is a spelling code-identical?)
- c2/nl.sh <src> <tag>   c2 IL tuple count (regime gate 10000, c2.dll 0x1077BE49)
- inl/inl.sh <src> <tag>   inline-decision trace (sz, a, g, pen, f, score per site); late.py <tag> [gmin] lists the late
  CCountedPointer / CAction() / ConvertVector sites -- the fitness for the budget-consumer search (~80 s)
- c2p.sh / c2pf.sh / wi/   c2prio trace and priority what-if (not used on this row this session)
