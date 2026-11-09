# agent885 tools (coordinator: land what is useful; all need `source tools/pk/env.sh` for S/A/wine/overlay)
- pk/fit.sh <src> <tag> -- cheap fitness (~60 s): one probe compile -> m3 exact/regs/regs+frame, frame items at retail offset, first diff; appends s6/LEADERBOARD.tsv.
- pk/wi.py <src> <tag> 'name=prio,...' [watch,...] (+ gdb side pk/wig.py) -- c2 what-if: overwrite a candidate's priority (or key) at insertion 0x10782b26 and log chosen registers; ALL=1 logs every choice; BLOCKS=1 dumps the first watched block (slow).
- pk/chain.py <prio.trace> <line> <id> -- follow a split piece back to its root through c2prio's reused ids; prints each split pass and the decision that exhausted it, and the piece's priority per re-sort.
- pk/first.py <obj> [--noframe] [--n N] -- first N instructions differing from retail (relocs normalised; --noframe also masks [ebp+-x] so it finds the first REGISTER decision).
- pk/odis.py <obj> [from] [n] -- ours v retail side by side by instruction index.  pk/m3x.py <obj> -- m3 with counts.
- pk/pk.py + pk/pkg.py -- c2 frame-packer probe (items, sizes, weights, flags, offsets; also /FAs); pk/pkmap.py maps items to names and retail offsets; pk/sim.py layout model (quicksort 0x107ae0c4 + density comparator 0x107ae9c2; 905/905).
- pk/slots.py, rslots.py, rorder.py -- retail v ours slot order; pk/ws.c (+ wsin.py, wsearch.py) -- greedy weight search for the frame inequalities.
- pk/c2dis.py <va> <va> -- capstone disassembly of c2.dll by VA.  side.sh <retail> <ours> -- ann.txt/ours.txt side by side.
