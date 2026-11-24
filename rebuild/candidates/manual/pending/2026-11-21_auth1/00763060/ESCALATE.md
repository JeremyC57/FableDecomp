# ESCALATE 0x00763060 NShape::FractureShape (1146 B) -- AUTH1, 2026-11-21 (stall clause 2: remaining ideas contradict Ego_d)
P = /home/user/FableDecomp/rebuild/candidates/manual/pending/2026-11-20_p25_open

## Best sources (repo, unchanged)
- Rule-clean best: P/00763060/00763060.cpp, DIFFER(1132v1146) (AUTH1 regrade today, /Gr: 58 normalised lines; p25L counted 160 g.py lines) (Ego_d plain text; helpers 0x00760020 / 0x00760190 now landed,
  so they are verbatim same-TU copies).
- Bracket (fitted RELOCATION_MATCH, not landable): P/00763060/evidence/bracket_match_anchor.cpp (a before it_anchor,
  `const C3DVector& anchor = *it_anchor++`).

## Remaining diff
The find_if start increment: ours `add esi,0Ch` at the loop top before the __find_if call; retail peels the first increment into the
preheader (`lea eax,[ebx+0Ch] ; mov ecx,eax ; ... ; add eax,0Ch ; mov esi,eax`), increments after the call (0x0076310B) and aligns the
loop head (`lea esp,[esp]` 0x007630B9) = post-increment IL `find_if(it_start++, ...)` with it_start one past it_anchor before the loop.

## Tried (p25L / p25Q, repo notes)
Ego_d plain E0 1132/160; a-first f1 1132/90; `b = *it_anchor++` + find_if(it_start++) E1 1137; `C3DVector b; b = *it_anchor++;` E2
1146v1146 / 82 (a-first 2 lines); local find_if template / iterator_category / for(;!quit;) / do-while: 1132/90; 1..32 dummy sweeps
(192 compiles) never move it; p25Q probe: VC7.1 keeps `g(++s,...)` at the loop top and moves `g(s++,...)` after the call.
AUTH1 spent no grade: no new Ego_d-consistent idea.

## Hypotheses
Retail TLC's text at the b / it_start statements (Ego_d 197..207) differs from the Ego_d revision (Ego_d and FableWin both 2245 B, so the
difference is size-neutral at /Od). Not testable by the standing call-level criterion (no named callee differs).

## Question for the advisor
Same TU as 0x007602F0 (tc_geometric_shapes): is anything admissible before the Windows line records arrive? One untested lead in the
repo note: an Ego_d-order spelling that assigns it_start inside the find_if argument (`find_if(it_start = ..., ...)`). Worth a bounded
pass, or already covered by the p25Q pre-increment probe?
