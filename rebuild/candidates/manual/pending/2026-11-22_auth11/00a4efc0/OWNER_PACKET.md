# OWNER PACKET 0x00A4EFC0 C3DAnimationSequenceData::InitialiseFromFileData (3491 B) -- AUTH11, 2026-11-22
S = /tmp/claude-0/-home-user-FableDecomp/5221bbec-fb18-575f-8649-a32d96bb1c2d/scratchpad. Files: S/w22/auth11/00a4efc0/
(CHECK.md, LOG.md, hdr/, c1_00a4efc0.cpp, users/, h/c1.il.json). History: pending/2026-11-21_auth7/00a4efc0/ESCALATE.md.
Ruling applied: ADVISOR_DECISIONS_B 2026-11-22 (Account A, AUTH10 / AUTH7 escalations), 0x00A4EFC0 items.

## Recorded best text
b3 (S/w22/auth7/00a4efc0/b3_00a4efc0.cpp, /Gr, overlay = S/w22/auth11/00a4efc0/hdr, AUTH7's overlay re-based on HEAD 0fbb3d82fa):
DIFFER(3491v3491) residue 16, one hunk: the position block's `compressed_size < uncompressed_size` operand order. The row body is
Ego_d's line for line (158/158 lines, 3872 B under VC10); the helpers are Ego_d-sized except the named exceptions below.

## What this step tested (pre-registered, graded once)
Check (CHECK.md): Ego_d's AllocateType block local `overflow_check` is NOT assert-only. Ego_d line 25 (16 B for sizeof 1 and 12)
is `if (overflow_check > 0xffffffff)`, VC10-calibrated; FableWin.pdb records the local in all 12 instantiations. Ego_r.pdb's
release record has only `this` / `elements` (optimised; it neither shows nor contradicts the local), so the ruling's
"Ego_r / FableWin show it" is met by FableWin only: please confirm that reading.

Test c1 = b3 + Ego_d's exact AllocateType shape (opt-in FABLE_RETAIL_CSIMPLEMEMORYPOOL_ALLOCATETYPE_EGOD_LOCAL: parameter
`elements`, block local `unsigned __int64 overflow_check`):

| graded once | result | note |
|---|---|---|
| row 0x00A4EFC0 | DIFFER(3491v3491), 16 | object byte-identical to b3 |
| order readout | position L=9b9b R=398a, L>R (b3 7a5b / 3cea) | finish fields re-drawn (pf 3781->4821, qf 3661->3301), no flip |
| 0x00A50510 | DIFFER(1748v1748), 35 | byte-identical to AUTH2's V13 (`count` / `total_size`) |
| 0x00AA0FA0 | DIFFER(4460v4460), 15 | byte-identical to V13 (relocation addends) |
| 0x00BD2D90 | DIFFER(721v721), 9 | byte-identical to V13 (x87 operand order) |

Acceptance (row flips AND the three users still match) is not met. Names have no code effect: c1 equals AUTH2's V13 everywhere.

## The users answer two different questions
- 0x00AA0FA0 and 0x00BD2D90 are the users that test the local: they match with the no-local if-body (IFBODY) and break with any
  named local (V12, V13, c1). 0x00BD2D90's own header says its x87 order follows the count of live symbols that Ego_d's local
  set fixes, so it is a non-fitted witness against a named local in retail's revision.
- 0x00A50510 tests if-body against ternary: it breaks under every if-shaped body, with or without the local (V1 10, V3/V5/V7,
  V12/V13/c1 35), and matches only the landed one-line ternary. Its header calls its x87 order c2-order fitted.

## Why this is the owner's decision
1. The row is a proven c2 order tie: the text is Ego_d-exact wherever Ego_d can measure it, and retail evaluates the position
   compare the other way. Header-set probes (AUTH7 b3 / b4) do not move the fields; only text the row expands moves them.
2. No calibrated freedom is left. Remaining uncalibrated bodies are the STLport CArray / vector members and std::copy
   (retail STLport v Ego_d Dinkumware) and the CCharString getters (Ego_d's 8-B CCharString is another revision). The ruling
   says a pick there would be chosen by readout, so none was made.
3. The one Ego_d-evidenced change left (the AllocateType local) is now tested: it does not flip the row and breaks two
   non-fitted users. So either retail's revision had no named local (Ego_d / FableWin being a later revision there), or the
   users' fits are wrong; the evidence here favours the first.

Options: (a) leave the row unlanded as a documented order tie with b3 as its text; (b) allow one fitted text in the STLport /
CArray bodies under a stated rule (the ruling currently forbids it); (c) something else the owner names.

## Queued, not applied
NAME_FIXES.tsv (S/w22/auth11/): AllocateType's parameter `count` -> `elements` (Ego_d, FableWin, Ego_r), needs a header claim.
Earlier queued: C3DVectorWord::operator== parameter `rhs` (AUTH7).
