# Hand chunk 3018 (CTCTavernGameCardPairs): 1 of 1 byte-exact (RELOCATION_MATCH in continuation session 2)

All grades with `FABLE_HDR_OVERLAY=/opt/fable_lane/w22/agent3018/hdr` (the prior 2016 overlay, unchanged; probe-only header copies moved
to probe/hdr_probes/). Grade log: grades.log (one line per full-row compile, `gr.sh <src> <note>`). Candidates: cand/.

## What matched (1)
- 007d6870 CTCTavernGameCardPairs::ProcessInput (vtable 0x0126F6FC slot 28) RELOCATION_MATCH 1778v1778, 571/571 unmasked: src/007d6870.cpp,
  test/007d6870.cpp (PASS g++ + VC7.1; the session-1 model test, unchanged semantics). Read-only vland: RELOCATION_MATCH PASS, only
  RELOC_NAME label WARNs (0x00A0D390 / 0x00687D00 / 0x0129BA3C carry other DB labels; spellings as session 1 / prior 2016).
  Shape lesson: see "Session 2" below.


## Session 1 state (superseded; its differ/ files are in prior/session1_differ/)
- 007d6870 CTCTavernGameCardPairs::ProcessInput DIFFER(1778v1778): 566/571 equal unmasked; only the final dot's term order differs:
  retail 0x007D6ED1 `fxch st(2) ; fmul st(5) ; fxch st(2) ; fmul st(3) ; faddp st(2) ; fmul st(3)` (X, Z, then Y), ours
  `fmul st(3) ; fxch st(1) ; fmul st(4) ; faddp st(1) ; fxch st(1) ; fmul st(4)` (Z, Y, X) | tried: see "Work log" | next: find natural
  IL worth ~+55..60 c2 record ids before the tree linearizer (see "Final dot mechanism").

## Work log (this session, 167 graded compiles)
1. fwsize (FW_EXTRA=/EHsc): GetRight RIGHT_COMPONENTS text was 43i v FableWin 15i -> default `Forward ^ Up` (15i/35b SAME):
   540 -> 549 masked (first cross product / first dot loads fixed).
2. Frame 0x30 v 0x38: frame-packer trace (pk.sh, tools/t13 from agent3016) showed the 4 string temporaries do not conflict with |dy|
   (n17) / dy (n15). Moving the movement block INSIDE the switch (MAIN_GAME falls through into `case TURNING_UP: case TURNING_BACK:`,
   `default:` base call + break) reproduced retail's exact slot layout in the free trace and graded 557 UNMASKED (frame exact, GFFloatAbs
   copy scheduling fixed).
3. Release copies (eax/edx swapped): `if (aligned) break; if (dot > 0.0f) return; PositionMesh..` (c08) -> 562.
4. Cross-product loads (F.Y*U.Z, U.Y*F.X): one extra named symbol -- `const C3DVector& forward = GamePositionRHSet.GetForward();` used by
   the second dot (s3, 564) plus `const C3DVector& best_pos = best->LocalPos;` in the final dot (f2, 566). Dead-long rulers: cross
   product right at +1/+2/+6 symbols over s3 (record-id tie).
5. Final dot: invariant to every spelling tried (named / const-ref / ordered difference vectors, `0.0f <` compare, named float dot,
   GFLimit clamps (also moves one store), position zero-ctor / Initialise / via three dots, index loops, const_iterator, if/else
   direction forms, named subtitle renderer, GetGameState() switch, explicit / named sound strings (code changes), GetDef inline body
   (expands: code changes), CDefPointer release / operator-> texts, GFFloatAbs `bits` text, Dot / cross / subtract named-result and
   named-float texts (same code, no order change)).

## Final dot mechanism (measured with the agent3010 linearizer hook, copied to tools/c2h/ + kz.sh, free)
- The final sum's products are of unnamed temporary fields: X (ids 609, 573), Y (630, 626), Z (632, 628); hash
  (32*max + 64*min + 48) & 0xffff, summed in descending hash order -> Z, Y, X. Retail X, Z, Y = Z/Y hashes wrapped past 65536.
- Uniform shift model (all ids >= 100): retail orders for dot 1 (4 terms) AND the final dot only for D = +55..+60.
- Diagnostics (NOT deliverable): 24, 25, 23, 26, 28 dead `long rN = 0;` locals anywhere in the function -> 571/571 equal (fs24 / fs25 /
  fr23..fr28; 32 / 40 fail); 8, 9 or 16 dead longs inside C3DVector::operator- (3 expansions) -> 571/571; inside Dot / operator^: no.
  A dummy function with 60 locals earlier in the TU changes nothing (records are per function).
- Natural record sources measured (kz.sh): explicit `CCharString("...")` PlaySoundW arguments +8 ids each (but `push eax` code);
  named phrase removed -1; one extra inline layer in operator-> 0; GFLimit +4; named final vectors +1.
- So retail's function has ~25 dead-long equivalents of extra c2 IL (or an IL change in operator-'s body worth ~8 dead longs per
  expansion). FableWin /Od sizes hint operator- (34i v ours 28i), operator^ (49i v 43i), Dot (23i v 21i) have more text, but every
  named-result / named-float text either changes code (named result: extra copy of the RELATIVE offset temp) or adds no records.

## Names
- Row: ?ProcessInput@CTCTavernGameCardPairs@@UAEXABVCInputEvent@@AAVCProcessedInput@@PAVCInputProcessTavernGame@@@Z (vtable 0x0126F6FC slot
  28; Ego_r.pdb + FableWin.pdb) -- as in prior/2016. Callees / globals as in prior/2016/NOTES.md and LABELS.tsv (no new names).
- Descriptive: UseCardBaseInput 0x01383C3F (unchanged from 2016); local names.

## Header debt
- None new. hdr/ = the 2016 overlay unchanged (CCard.h, CTCTavernGameCardBase.h, CTCTavernGameCardPairs.h new; see hdr/README.md).
- probe/hdr_probes/: C3DVector.h / CDefPointer.h copies with probe-only opt-ins (SUBTRACT_FLOATS / MEMBERWISE / NAMED, DOT_NAMED,
  CROSS_NAMED, FABLE_PROBE_* dead-local hooks, operator-> variants) -- diagnostics, not for landing.

## Session 2 (continuation, this run) -- 566 -> 571 (RELOCATION_MATCH)
Residue at start: only the final `(position - H->LocalPos).Dot(position - best->LocalPos)` term order (0x007D6ED1). Mechanism (session 1):
c2 tree-linearizer hash keys over record ids; the function needed MORE IL (f2 + 24 dead longs matched, diagnostics only).
What was measured (all full-row grades, cand/s2/, gq.sh prints which record-keyed sites still differ: CROSS = GetRight cross product
0x007D6BF3..C2F, DOT = first dots, FINAL = 0x007D6ED1):
- FableWin helper sizes are NOT the lever here: operator- 34i / operator^ 49i / Dot 23i are VC10 by-value / float-return artefacts
  (r3010 notes); named-result operator- (probe/hdrA FABLE_PROBE_SUB_NAMED / SUB_INIT) changes the RELATIVE offset arm's code (514 eq).
- A dead-local ruler counts the same anywhere in the function (start / main-game / movement / loop all equal); dead references count as
  well; a used, forwarded named value counts LESS than a dead one (often 0..1/2 unit). Dead constant initialisers (`long direction = 0;`,
  `C3DVector position(0,0,0)`) count 0.
- `C2DVector(card_dx, card_dy).GetSquaredMagnitude()` in the loop moves many records but makes the loop's square order record-keyed
  (fragile); dropped.
- Code-neutral natural pieces (each graded alone, cand/s2/pieces.py + piece2.py): b1 `switch (GetGameState())`; yes_pressed / selected
  bools; `const CTavernGameDef* game = game_def.Get()` (+ game->Continue); player_gui / subtitle_renderer refs; `ETavernGameStates state`;
  pointer / pointer_pos in the RELATIVE arm; `height` in the ABSOLUTE arm; `C3DVector right` + right_dot / forward_dot / move_x / move_y;
  `position.Z = offset.Dot(up)` then `position.Z = 0.0f`; highlighted pointer + highlighted_pos / width / height; `long frame`;
  loop `if (card != HighlightedCard) {` with card_pos / card_width / card_height; best before best_distance; `bool aligned`;
  movement_x / movement_y for the alignment test; `float dot`. Pieces that CHANGE code (do not use): quit block `game_def.Get()`
  (P5), `best_pos` before the alignment test (R1), named continue phrase (S4), named abs values (n10), C2DVector delta (k2/k7).
- Final (src/007d6870.cpp = cand/s2/c/zh2.cpp re-indented): all of the above together. Rulers: one diagnostic dead local more or less
  flips CROSS (0x007D6C22 `fld [edi+4]` / `fmul [edi+0Ch]`) or FINAL, so the hand-in sits in a one-unit window.
FITTED v PROVEN: the switch/fallthrough shape, release copies, frame and GetRight text are proven by bytes (session 1). Which named
intermediate values the original had is FITTED: the bytes only prove the AMOUNT of IL (record count), not these particular names. The
names are descriptive local names (no PDB local names available); every member / callee spelling is unchanged from session 1.
grades.log: lines 168..536 are this session (~140 DIAG rulers marked "DIAG"; lines graded for a missing file (r6, r7, r13, zr8, zs2/zs3
first try) are junk repeats of the previous build and carry no information).
