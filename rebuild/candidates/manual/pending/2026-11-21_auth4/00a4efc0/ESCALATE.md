# ESCALATE 0x00A4EFC0 C3DAnimationSequenceData::InitialiseFromFileData (3491 B) -- AUTH4, 2026-11-21 (early return: context)
S = /tmp/claude-0/-home-user-FableDecomp/5221bbec-fb18-575f-8649-a32d96bb1c2d/scratchpad. Full log: S/w22/auth4/00a4efc0/LOG.md.

## Best source
S/w22/auth4/00a4efc0/a2.cpp (= AUTH2 a2, V1 opt-in renamed FABLE_RETAIL_CSIMPLEMEMORYPOOL_ALLOCATETYPE_IFBODY), /Gr,
FABLE_HDR_OVERLAY=S/w22/auth4/00a4efc0/hdr: DIFFER(3491v3491), residue 16 (unchanged). Rule-clean fallback a1 (26).

## Mechanism: PROVEN (step 2 of the method)
The residue is the EVALUATION ORDER of the position `compressed_size < uncompressed_size` operands in c2's tree optimiser, not the
register allocator:
- IL dump (kit dump2/pp2): ours evaluates compressed first in all three blocks; retail does so for rotation and scaling only.
- Keeping 103's dropped finish piece (C2X_KEEP) and forcing it to ebx (C2X_SETREG) does NOT change the order (kp40 / kp40b).
- The compare's operands carry keys su<<24 | size<<16 | hash16 (0x1075d388; hash 0x1075d54e = sum child_low16 << (i&7) + op - 0x24a).
  At the final tree pass both are su 1 / size 6, so hash16 decides: left 0x7a9b > right 0x3d0a -> left first.
- FORCED (tools/il/kforce.py: right child's key 01063d0a -> 01067fff at the compare whose key is 020df4ed): the object is
  RELOCATION_MATCH, residue 0, pad included (h/kf1.obj).
Leaves (joined by address, symlog x dump2): L = cnt + 2*pal_start + 2*pal_finish + 0x99, R = pos_finish + 2*pos_start + 0xc9 (mod 2^16);
cnt = k15 CSE temp #3086 (<<6, so mod 1024 counts), finishes = k4 fields #2493 / #2484 (<<5|1, mod 2048), starts = k4 #29 / #23.
Retail must have R > L for position and L > R for scaling and rotation.

## What it takes
Linear windows: the position count temp's number in [-490,-247] or [+534,+777] of its current value (mod 1024), or the finish fields
moved by >= +534 / <= -749. Small uniform shifts never flip it. But numbering jumps: the default AllocateType (a1) moves the count
word 0x0380 -> 0xf380, so a text change anywhere can re-draw c (about a 24% window). Scored (dump2, 55 s each) without a flip:
a1, four C3DVectorWord::operator== spellings, ISNONZERO_ZYX off. AUTH2's AllocateType variants (V1/V3/V5/V7/V10/V11) were graded
by AUTH2 without fixing it, consistent with this.

## VC10 evidence (new lane compiler)
operator== (Ego_d 86 B one line): 16 spellings give 71..96, none 86. GetBoneName (Ego_d 16/27/6): every spelling 14/27/6: the return
line matches, the prolog is 2 B short (one more 4-byte frame slot in Ego_d). Both helpers really differ from Ego_d, but no
candidate text found. The row body under VC10: 3297 v Ego_d 3872 (later revision; its loop compare lines are 79 B v ours 32).

## Questions for the advisor
1. The lever is a numbering draw (the count CSE temp's k15 number, or the two finish fields' k4 numbers). Is a search over
   Ego_d/VC10-backed helper texts, scored by tools/ord.sh and kept only if every landed user still matches, the right next step, or
   is a re-draw this coarse (any text anywhere moves c by hundreds) a "fitted" device by the 0x00B24850 / AllocateType rulings?
2. Which helper texts count as evidenced candidates here: only those whose VC10 size equals Ego_d's (none found yet for operator==
   / GetBoneName), or also Ego_d-shaped body changes in the row (separate CArray declarations as Ego_d 1100..1105)?
3. Budget: grades 3, readouts about 27 (stall rule 25 / 40 not reached; returned early for context).

## Tools (all in S/w22/auth4/)
rd.py / ocmp.py (residue, relocs masked), rng.py, lin.py (split lineage), c2x/c2prio_x.py (+ C2X_FORCE2 at resorts (ICE: unsafe),
C2X_KEEP, C2X_SETREG, resort readout fixed to walk eax), tools/il/{dump2,pp2,keys,kforce,symlog,order}.py, tools/ord.sh, keys_all.py,
keytree.py, force.sh.
