# AUTH6: Account A authoring agent (2026-11-21): the NShape pair, bounded follow-up of AUTH5

Owner orders: do not park; keep working until the rows match. Stalls go to the advisor through the coordinator. Locks read AUTH6.
Be frugal with context: residue counts and differing hunks only, never whole listings.

## Read first
1. `$S/w22/auth1/BRIEF.md` (Paths, Tools, Hard rules, Hand-back apply; outputs go to `$S/w22/auth6/`),
   `S=/tmp/claude-0/-home-user-FableDecomp/5221bbec-fb18-575f-8649-a32d96bb1c2d/scratchpad`.
2. AUTH5's hand-back: `$S/w22/auth5/BRIEF.md`, `STATUS.tsv`, `MATCHES_PENDING_RULING.tsv`, `007602f0/{NOTES,LOG}.md`,
   `00763060/{ESCALATE,LOG}.md`, its step0 texts and overlays (`$S/w22/auth5/hdr`, `hdr.diff`). Start from its files, do not redo Step 0.
3. odall10.py docstring (`tools/decomp_pipeline/linux_lane/lane_tools/odall10.py`); FINDINGS_LOG entries for p25Q (0x00763060).

## 0x007602F0 NShape::IsShapeConvex (AUTH5: byte MATCH with PAREN_CAST; Ego_d 313 / 342 not reproduced)
Advisor (2026-11-21): decode 313 / 342 as OBJECT LIFETIME, not spelling.
1. In AUTH5's own VC10 listing of line 342, check whether the 12 B gap is exactly one more destructor sequence (EH-state store 4,
   `lea ecx` 3, call 5) = one more checked iterator alive at the `return`. Same idea for 313 (100 v 91).
2. Use the two Ego_d reads AUTH5 did not: the S_BLOCK32 tree with each block's nested locals (scopes, not just frame offsets), and
   the line table's separate address ranges for 313 and 342 (are the extra bytes at the loop top or bottom?).
3. Grade the decoded text under VC7.1 (retail's iterators may be plain pointers: then a scope-only change may cost nothing).
4. Terminal: still MATCH with PAREN_CAST as its only Inferred item -> MATCHES.tsv; breaks retail, or undecoded after ~30 VC10
   measurements -> ESCALATE.md (owner question).

## 0x00763060 NShape::FractureShape (AUTH5: g1 Ego_d-exact = DIFFER(1134v1146) 78 lines; e5 fitted = 2 lines)
Advisor: no open pass on g1.
1. One C2X_FORCE readout on the begin / zero register pair (ebp/ebx swap) on g1 or h1: if the preheader lines remain, that lever is closed.
2. Check Ego_r.pdb's types: is TLC's `vector<C3DVector>::iterator` a raw pointer (STLport 4.6.2)? If yes, the iterator-model lever is closed.
3. Grade the minimal-deviation text: g1 everywhere, e5's text ONLY at lines 195-204. Record that region's VC10 sizes against Ego_d, and
   that Ego_d lines 288 / 291 are debug-only code retail lacks.
4. Before calling 197 "settled": B's p25Q FINDINGS decodes Ego_d 197 (74 B) as `it_anchor = begin() + 1`; AUTH5 decodes it as
   `C3DVector a = *it_anchor++;`. Measure B's text with `--stl` in AUTH5's step0 frame; if it is also 74, write which constraint selects.
5. Terminal: a match (MATCHES.tsv, with the Inferred lines listed), or an owner packet in ESCALATE.md (the e5-region deviation, its
   VC10 sizes, and why no Ego_d-neutral lever remains).

## Stall rule
20 graded variants on a row with no residue reduction, or every idea left breaks a rule: ESCALATE.md and return. Keep STATUS.tsv / LOG.md
current. Never write into /home/user/FableDecomp.
