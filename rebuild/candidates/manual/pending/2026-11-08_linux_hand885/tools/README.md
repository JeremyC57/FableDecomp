# agent885 tools (coordinator: land what is useful; all need `source tools/pk/env.sh` for S/A/wine/overlay)
- pk/fit.sh <src> <tag> -- cheap fitness (~60 s): one probe compile -> m3 exact/regs/regs+frame, frame items at retail offset, first diff; appends s6/LEADERBOARD.tsv.
- pk/wi.py <src> <tag> 'name=prio,...' [watch,...] (+ gdb side pk/wig.py) -- c2 what-if: overwrite a candidate's priority (or key) at insertion 0x10782b26 and log chosen registers; ALL=1 logs every choice; BLOCKS=1 dumps the first watched block (slow).
- pk/chain.py <prio.trace> <line> <id> -- follow a split piece back to its root through c2prio's reused ids; prints each split pass and the decision that exhausted it, and the piece's priority per re-sort.
- pk/first.py <obj> [--noframe] [--n N] -- first N instructions differing from retail (relocs normalised; --noframe also masks [ebp+-x] so it finds the first REGISTER decision).
- pk/odis.py <obj> [from] [n] -- ours v retail side by side by instruction index.  pk/m3x.py <obj> -- m3 with counts.
- pk/pk.py + pk/pkg.py -- c2 frame-packer probe (items, sizes, weights, flags, offsets; also /FAs); pk/pkmap.py maps items to names and retail offsets; pk/sim.py layout model (quicksort 0x107ae0c4 + density comparator 0x107ae9c2; 905/905).
- pk/slots.py, rslots.py, rorder.py -- retail v ours slot order; pk/ws.c (+ wsin.py, wsearch.py) -- greedy weight search for the frame inequalities.
- pk/c2dis.py <va> <va> -- capstone disassembly of c2.dll by VA.  side.sh <retail> <ours> -- ann.txt/ours.txt side by side.
- pk/qc.sh <src> <tag> -- ~70 s plain compile + first.py --noframe + m3x + same.py v s7/ref_a3.obj -> s7/LEADERBOARD.tsv.
- pk/score7.py <obj> -- `bytes first <idx> fx <n>` (first register-level difference, frame-masked registers-exact difflib count); 24 s.
- pk/gen7.py <base> <out> <name[,name]> + pk/srch7.sh <base> <prefix> <names...> -- reference-binding lever search, serial, scored by score7 -> s7/SEARCH.tsv.
- pk/mcfa.py <src> <dir> -- plain compile + /FAs listing (m.obj, m.asm); pk/lpair.py <m.obj> <m.asm> <line> [line2] -- ours v retail per source line.
- pk/cdiff2.py <m.obj> <m.asm> <c2prio7.json> 233 -- c2's colouring decisions in allocation order v retail's operands (width-aware votes).
- pk/c2prio7.py (c2prio + first/last block lines per choice + `--forcereg 'K91=8'` what-if), pk/fw.sh <src> <tag> [forcereg] (~6 min), pk/jev.py <json> 233 [--upto K] [--all] -- decision list.
- pk/wi7.py / wig7.py -- PEN=1 logs each choice's penalty sources (one-register neighbour 100*spill, 0x107AC667 adds, copy preferences).
- pk/xv.py <base> <l1> <l2> <variant.txt> <tag> <retail lo> <retail hi> [-v] (XLO/XHI widen the compared source lines) -- local spelling probe; pk/frame7.py <obj...> -- __chkstk frame size.
- s8/tools/c2prio8.py -- c2prio7 + `--snapk N` (allowed set / prio / spill of every const -1/0/1 piece before each choice, `--snapnames`),
  `--dumpk K,..` / `--dumpr K,..` (every block's lines, live ids +0x50, used registers +0x54 before choice K / at the re-sort after its split),
  `--forceprio name=P` or `name@id=P` (first insertion of that root only). s8/tools/fw8.sh <src> <tag> <snapk> [args] wraps it (~7 min,
  cdiff2 with base 235). NOTE: c2prio json lines = file line - 2.
- s8/tools/snap.py, bd.py, bds.py, splitview.py, endreg.py -- allowed-set changes per K, blocks of a piece and who holds each register there,
  pieces made at a split, const pieces live in a line range.
- s8/tools/cpos.py, cuse.py, regmap.py <obj> <asm> REG L1 L2, shape.py, rdis.py <lo> <hi>, strins.py <obj> -- const materialisations,
  immediate v register constants, where retail uses REG and ours does not, masked shape hunks, retail disassembly, string instructions.
- s8/tools/genfx.py + runfx.sh -- one-lever variant generator around the fixed-register sites, serial qc.sh scoring.
