# WORKLOG -- chunk 890 (agent890), 2026-11-08

1. Read CHUNK_BRIEF, PROMPT, C2_ALLOCATOR_PROBE, GOTCHAS tail, EXTRA_RULES, ASM_RULES, hand886/hand880 lessons. Verified the three
   extents with xdis.py (TRACE.md "Extents": 3,849 / 4,284 / 5,101 bytes).
2. Built the agent tools in work/: p.sh (grade with the hdr/ overlay, ATTEMPTS.log), dd.py + d.sh (strict / loose diff hunks),
   calls.py (call-sequence alignment, --counts), fas.py (/FAs frame tables), c2.sh (c2prio --trace / --rotation), inl/run.sh +
   sites.py (c2 inline decisions per site, with T / k logged), micro/f1.cpp (frequency micro probe), ann.py (annotated retail listing).
3. Row 0x00551EA0: drafted from the CTable headers (+ overlay opt-in for the slot 142-144 virtuals), 12 grades v1 .. final
   (TRACE.md E1-E8): ?: -> if/else (frame), while-loop frequency (inline split), declaration order (zero stores, imul),
   inline edge totals (x87 CSE), FABLE_RETAIL_STLP_MAP (key store), load source of the separation X -> RELOCATION_MATCH;
   D1 member-template find for the 0x00553520 symbol. src/00551ea0.cpp + test/00551ea0.cpp (g++ and VC7.1 PASS).
4. Row 0x0056AD40: identified as CTCBoastUI::Activate (PDBs + landed caller), annotated and fully decoded the listing
   (differ/0056ad40.cpp); not drafted against headers -- the needed opt-ins are listed in the differ header. 0 grades.
5. Row 0x00498490: identified (PDB private DrawRetailDisplay; landed caller's DrawProgress spelling is wrong), derived the retail
   CProgressDisplay layout, decoded the first third (differ/00498490.cpp). 0 grades.
6. Deliverables: STATUS.tsv, NOTES.md, LESSONS.md, LABELS.tsv, NAME_FIXES.tsv, hdr/README.md, TRACE.md; gall.sh / tall.sh / tall.py;
   finish.py.
Stopped by the session budget: rows 0x0056AD40 and 0x00498490 have no graded source (far below the 50-attempt bar) -- handed over
with their decodes, layouts and name evidence.

## Session 2 (2026-11-08)
### 0x00498490 CProgressDisplay::DrawRetailDisplay -- parked DIFFER(5101v5101), strict 84.1% / loose 99.9% (7 grades)
- State: differ/00498490.cpp (= probe/r3_v7.cpp) drafted against the shared headers + hdr/ opt-ins (hdr/README.md "Session 2";
  new engine/CProgressDisplay.h and engine/CNewFrontendGameComponent.h). Same size as retail, same call multiset, same registers.
- What flipped and why (TRACE.md B3..B8):
  v1 4826v5101 -> v2 5051 (EndsWith FABLE_FORCEINLINE: c2 inline cost 405 > budget, a /d2inlT threshold would also expand
  GetVStringExtents (cost 126) which retail calls; `long total = 130; if (!g) total = 465;` for retail's mov/jne/mov) ->
  v3 5085 (bar pieces: `const CPlayerGuiDef& def = PeekGuiDef();` before the draw call; first tile `C2DBoxF box; box = C2DBoxF(...)`
  for the temporary + dword copy; text window from two C2DVectors for retail's x87 order) -> v4/v5 5101 (EndsWith body
  `if (offset >= 0) return strcmp(...) == 0; return false;`; `&&` gave a branchy bool) -> v7 (BlackScreen box as a named local
  `C2DBoxF screen(...)`: zeros stored before the colour, as retail). v6 (3-arg CRGBColour) changed nothing.
- Residue: (1) frame slot packing only (same 0x5C frame; display/scaleX/scaleY/text/fraction/... 8 bytes higher, the 0x30/0x38
  groups partitioned differently); (2) `mov ecx,edi` one slot early at 0x00499094.
- Next steps, in order:
  1. Frame: c2's packer (c2.dll 0x10790CD6 item loop -> 0x10791426 / 0x10791A6A) takes items by size ascending, weight descending
     (work/pk/pk.sh) and puts each into the MOST RECENT slot it fits (item size <= 2x slot size, no interference), else a new slot
     (layout = creation order). work/s2/e10.order lists every item with our slot and retail's slot. The first divergence in the
     walk: tile 1's temporary T11920 (`GFAYToVY(256.0f) * scaleY`, w2) joins slot A (total, tag) in ours but opens retail's 0x38
     slot, so in retail something in A is live across tile 1 (or T11920 is ordered later). Find that source difference (fx.sh
     compiles in 20 s and prints the slot mismatches; e3/e10 show block scope of formattedWindow is a lever: e10 = all locals but
     barPosition/sealPosition/sealX/formattedWindow at retail's slots, frame 0x54).
  2. Seal block: retail emits `fsub 4.0` before `mov ecx,edi` (the second GetRenderTargetDimensions); try the sealX spellings.
  3. On match: behaviour test, finish.py, tall.sh / tall.py.
### 0x0056AD40 CTCBoastUI::Activate -- not drafted this session (0 grades); session budget went to 0x00498490
- differ/0056ad40.cpp still holds the session-1 decode and the header worklist (CTCBoastUI retail branch, CQuestManager
  ActiveQuests / GetBoastsForScript 0x004B19F0 / GetActiveQuestCardFromScriptName 0x004B0C80, the UI list classes for slots
  144/151/152/156, CTCQuickAccessMenu / CTCQuestCard / CTCCombat / TC 157 callees, three local tree instantiations).
- Next: survey those headers first (grep rebuild/include/engine for each class), write the opt-ins, compile with fas.py, then grade.

## Session 3 (2026-11-09)
### 0x00498490 CProgressDisplay::DrawRetailDisplay -- parked DIFFER(5101v5101), strict 84.1% -> 99.2%, loose 99.9%
- State: differ/00498490.cpp (bytes = probe r3_v8, graded once DIFFER(5101v5101); r3_v9 is the same bytes after the comment /
  include cleanup, checked with work/s3/x.sh, not re-graded). Every named local now sits at retail's frame slot (frame 0x5C).
- What flipped and why (TRACE.md "Session 3 -- row 0x00498490", C1..C4; tools work/s3/is.sh + an.py hook c2's packer 0x10791426):
  (1) tiles: the corner C2DVector temporary was one 8-byte packer item; the four-float C2DBoxF ctor (??0C2DBoxF@@QAE@MMMM@Z) makes
  BRX / BRY separate 4-byte items, so BRX joins slot A and BRY opens slot G as in retail; (2) formattedWindow in an inner block
  (its live range no longer covers the dims temporaries / extents); (3) the text window as a named `C2DBoxF window` live across the
  block, so formattedWindow fails slot I as in retail. The row no longer includes fable_string.h itself (EndsWith opt-in reached
  through the normal include chain; hdr/README.md).
- Residue (all spellings tried give identical bytes, TRACE C3 / C4): (1) four x87 pairs (0x00498A7B / 0x00498B73 / 0x00498C88 /
  0x00498DA8): `BRY = barPosition.Y + t` -- retail loads barPosition.Y first, ours the temporary (commutative canonicalisation,
  VC71_CODEGEN_RESEARCH R1/R2/R4; ctor operand swap, named floats, address exposure, named extents all no-ops); (2) `mov ecx,edi`
  one slot early at 0x00499094 (named sealY, `sealX -= 4` no-ops); (3) EndsWith `lea esi,[ecx+eax]` base/index order at 0x0049941A /
  0x004994ED (`&p[offset]`, `offset + p`, tail pointer, inline difference all no-ops).
- Next, in order: (1) find the IL property that orders the fadd operands (hook c2 on the fadd emission and compare barPosition's
  IL node with the extents temp's: address-taken / volatile-like / ref count); (2) the lea operand order inside EndsWith (the
  same hook on the add that forms the strcmp argument); (3) the `mov ecx,edi` schedule. On match: test, finish.py, tall.sh/tall.py.
- Dead ends (do not repeat): see TRACE C3 / C4 lists.

### 0x0056AD40 CTCBoastUI::Activate -- first drafts against the shared headers, parked DIFFER(5039v4284)
- State: differ/0056ad40.cpp (bytes = probe r2_v3, graded). 2 grades (r2_v2 5031v4284 57.2% / 89.4%, r2_v3 5039v4284 58.6% / 90.3%).
  Eleven additive opt-ins in hdr/ plus one new header (hdr/README.md "Session 3"); two NAME_FIXES (0x004382C0 ForceClearGui,
  0x00625920 Deactivate). The session-2 decode draft is kept at work/s3/0056ad40_session2_decode.cpp.
- What flipped and why (TRACE.md "Session 3 -- row 0x0056AD40", D0..D4): v1 -> v2 dropped FABLE_RETAIL_STLP_BITSET_TEST_DECL
  (retail tests the UsingComponents word in place) and respelled 0x004382C0; v2 -> v3 made the take-boast label a named local
  (retail fetches it before the CreateMenuEntryComponent arguments).
- Residue: inline decisions. Ours expands every ~CCountedPointer (retail calls the ILT thunks 0x00429B72 / 0x00429C53 / 0x00429D8E
  at all 13 sites), both local map ctors / dtors, the first list::size, SetStaticCast, make_pair + pair converters, map find /
  insert_unique and one vector push_back; the c2 trace (work/inl/r2_v2.json) shows our scores 24..63 under the T=74 cut-off while
  retail expands sites we score 58..68, so retail's per-site scores differ (frequency or cost), not the threshold.
- Next, in order: (1) block frequency: c2's f is 0.195 at the second loop's list::size / map ctors and 0.977 in the boast loop --
  compare with retail-plausible guard shapes of the second quest loop (nested ifs instead of `continue`, the HasScript test in the
  for condition) and re-run work/inl/run.sh; (2) cost: check whether retail's CCountedPointer dtor / SetStaticCast differ (e.g. a
  Validate() call or ReleaseData-based dtor raises c1) -- only with family evidence; (3) then the 2026-11-08 ladder (per-row flag,
  then scoped `#pragma inline_depth` / `auto_inline`, line-2 Inline-budget note); (4) on match: behaviour test, finish.py,
  tall.sh / tall.py.
- Dead ends: /d2inlT45 (dtors out but bitset test out too, map ctors still in), /d2inlS5 (no change).

## Session 4 (2026-11-09, all-means push)

Leaderboard (best per row; strict / loose = fit on the graded bytes):

| row | session-3 best | session-4 best | grade | what flipped |
|---|---|---|---|---|
| 0x00498490 DrawRetailDisplay | r3_v8 DIFFER(5101v5101) 99.2% / 99.9% (11 insns) | r3_s4a DIFFER(5101v5101) strict 1417/1418 | 1 grade | 4 x87 pairs + 2 EndsWith lea solved |
| 0x0056AD40 CTCBoastUI::Activate | r2_v3 DIFFER(5039v4284) 58.6% / 90.3%, calls 213/238 | r2_s4h DIFFER(4299v4284) 65.7% / 93.3%, calls 226/238 | 1 grade | -740 bytes: 8 inline_depth(0) regions |

### 0x00498490 CProgressDisplay::DrawRetailDisplay -- one instruction left
- x87 load order of the four bar sums (TRACE F1): build barPosition with the ctor from a named float and copy it for the second box
  (`C2DVector position(barPosition); position.X += GFAXToVX(64.0f);`). Rule found with micro TUs: for two direct frame operands
  VC7.1 loads the more recently stored one first; an inline-ctor parameter temporary outranks a field-assigned member; a corner
  built by the ctor and only copied keeps corner-first.
- EndsWith `lea esi,[ecx+eax]` at both sites (TRACE F2): new hdr opt-in FABLE_RETAIL_CCHARSTRING_GETASCHARARRAY_NULLRETURN
  (GetAsCharArray in early-return form; default path byte-identical; hdr/README.md "Session 4").
- Left: `mov ecx,edi` one slot early at 0x00499094 (TRACE F3). c2's list scheduler (0x1078734C: 2 slots/cycle, one per unit,
  priority ((height + out-degree) << 10) + FP bit) issues `call GFAXToVX(32)` in slot 1 of the `fstp tv` cycle; retail's order needs
  that call one cycle later (slot 0, with fsubr co-issued). Same bytes, so retail's IL had a different DAG there (an extra latency-1
  edge or a no-op int tuple deleted after scheduling). ~40 source spellings and inline helpers change nothing.
- Next lever: find a source construct that adds a latency-1 predecessor to that call or an int tuple taking slot 1 in the
  `fstp tv` cycle (probe: work/s4/re/probe/t13.py prints regions, DAG edges and every cycle's ready list / picks).

### 0x0056AD40 CTCBoastUI::Activate -- owner ladder step 2
- Step 1 ruled out (session 3 data: no /d2inlT separates bitset::test 58 from ~CCountedPointer 47). The neighbouring size TU
  CTCInventoryExperience.cpp suggested `optimize("s")`: 3564 B, wrong (retail has the /O2 aligned frame).
- Step 2: `#pragma inline_depth(0)` regions at the first list::size, the two map declarations (with FABLE_RETAIL_STLP_MAP so retail's
  no-argument map ctors are called), SetStaticCast, the make_pair inserts, the new-group insert and the three scope exits of the
  CCountedPointer locals; each region comments the retail call it reproduces, line 2 carries the Inline-budget note.
  Graded r2_s4h DIFFER(4299v4284), strict 65.7% / loose 93.3%, calls 226 / 238.
- Left: push_back 0x0056DCE0 (0x0056BB0E / 0x0056BB75; depth 0 at those statements breaks the register web, pj), the map dtor
  0x0056D7B0 (0x0056BC36) and list::clear 0x004B8C00 (0x0056BC6F) at the quest-loop exit, then frame / register drift.
- Next lever: depth-0 regions narrowed to the two push_back calls alone (a named reference to the vector, then the push_back
  statement), and the boasts list's scope exit; re-check calls.py after each, then grade.
- NAME_FIX candidate 0x0042A2DB (CList SetStaticCast) conflicts with landed 0x00558770's binding: check before respelling.

## Session 5 (2026-11-09, all-means push continues)

Leaderboard (best per row; strict / loose = fit on the graded bytes):

| row | session-4 best | session-5 best | grades | what flipped |
|---|---|---|---|---|
| 0x00498490 DrawRetailDisplay | r3_s4a DIFFER(5101v5101) strict 1417/1418 | **RELOCATION_MATCH** (r3_s5a; src/00498490.cpp) | 2 (probe + final src) | the early `mov ecx,edi` (0x00499094): parenthesised half-width |
| 0x0056AD40 CTCBoastUI::Activate | r2_s4h DIFFER(4299v4284) 65.7% / 93.3%, 8 regions | r2_s5b DIFFER(4275v4284) 69.6% / 95.6%, 1 region | 2 | first quest loop as `while`: retail's call/expand split by frequency |

### 0x00498490 CProgressDisplay::DrawRetailDisplay -- MATCHED
- The residue was order-only: c2's list scheduler (0x1078734C) issued `call GFAXToVX(32)` one cycle early. A Python re-run of the
  scheduler on the dumped DAG (work/s5/sim.py) and single-change searches (search_*.py) showed retail's graph needs one more no-code
  x87 node in that region; a row-wide tuple dump found it: VC7.1 keeps a PAREN tuple (0x269) for a parenthesised floating
  sub-expression. Source: `float sealX = (GFAXToVX((float)display.GetRenderTargetDimensions().X) * 0.5f) - GFAXToVX(32.0f) - 4.0f;`
  (TRACE G1 / G2).
- src/00498490.cpp RELOCATION_MATCH (gall.sh); test/00498490.cpp PASS under g++ C++03 and VC7.1 (tall.py); finish.py regenerated
  payload.json / oracle.tsv. reloc_identity: HARD none, 1 WARN (ChangeGraphicsBankScene vs the 0x00437570 catalog label; already in
  NAME_FIXES.tsv). Inline device: FABLE_FORCEINLINE CCharString::EndsWith (opt-in FABLE_RETAIL_CCHARSTRING_ENDSWITH, line 2 of src).

### 0x0056AD40 CTCBoastUI::Activate -- frequency instead of devices
- c2's inline rule read from c2.dll: score = c1 * (1/f + 6) / 7 < 74 (TRACE H1). Solving it for every site whose choice retail
  shows (work/s5/want.py) gave the frequencies retail's source must produce; micro probes gave the loop-shape rules (`for` body 5x,
  code after it stays at 5x; `while` body 9x, code after it drops back) (TRACE H2).
- The first quest loop (count the quests with boasts) as `while (quest != activeQuests.end()) { ...; ++quest; }` leaves 3 of 34
  sites wrong with no device (TRACE H3); push_back x2, map ctors/dtors, SetStaticCast, every ~CCountedPointer and the quest-exit
  ~list now follow retail, and the prologue / `this` in edi / zero in ebp match. One inline_depth(0) region stays around
  `BoastGroups.insert(std::make_pair(menuEntry, *boast))` (make_pair 0x0056CB40 + converter 0x0056CB60 called).
  Graded r2_s5a / r2_s5b DIFFER(4275v4284) strict 69.6% / loose 95.6% -> differ/0056ad40.cpp.
- Left: the ~list<CBoast> on the empty-boasts `continue` (retail expands at 0x0056AFFA; ours calls ??1list: the frequency it needs
  conflicts with make_pair's, so one callee cost differs in retail's IL); frame 0x200 v 0x208 (reordered slots); register / order
  drift in the quest body from 0x0056B133 (boastSubList in esi + spill [esp+11Ch]; the detail-list / backdrop CreateComponent
  CCharString arguments built elsewhere).
- Next lever: (1) the CreateComponent argument order inside the boast loop (the 9 / 26-instruction moves at 0x0056B22E / 0x0056B242:
  try the backdrop / detail-list statements as retail orders them, then named CCharString locals); (2) c2prio on the boastSubList
  colouring; (3) the frame order with fas.py's `_name$` table against slotmap.py; (4) the ~list / make_pair cost question with header
  overlays (run2.sh + HX) on ~_List_base / make_pair / the CBoast copy before any device.
- Header changes this session: none (hdr/ untouched; the hx/ overlay under work/s5 is an experiment only).

## Session 6 (2026-11-09) -- row 0x0056AD40 CTCBoastUI::Activate (inline split solved, colouring open)
- Residue 1 (the ~list<CBoast> at 0x0056AFFA) and residue 4 (the last inline_depth device) are solved in principle: c2's
  inline costs are fixed by the callee code, so retail's split must come from frequencies, and the only shape that satisfies all
  18 constraints is `if (boasts.empty() || <folded>) continue;` plus one 0.75 fall-through upstream (TRACE S6.1-S6.3).
  work/s6/d1.cpp gets all 34 inline sites right with no device (TRACE S6.4); 30 spellings give identical bytes (S6.5).
- New blocker (residue 3): that expansion adds three loop-2 blocks that cost the activeQuests split piece 6 and the zero piece 2,
  so at the 6th re-sort the zero piece outranks activeQuests (-4 v -7) and takes edi; this goes to ebx and the whole function
  shifts registers (strict 69.6% -> 38.6%). TRACE S6.6 has the per-block bookkeeping; 24 natural levers did not move it (S6.7).
- Frame (residue 2) not attempted this session: the hand886 tools8/pack.py replay needs the colouring settled first (the
  playbook: grading is unreliable while the slot set moves).
- Leaderboard (fits; grades in ATTEMPTS.log):
  | source | bytes | strict | loose | calls | device |
  |---|---|---|---|---|---|
  | differ/0056ad40.cpp = work/s6/b0.cpp (r2_s5b) | 4275 | 69.6% | 95.6% | 238 (224 aligned) | inline_depth around insert |
  | work/s6/h2.cpp (b0 + both levers) | 4289 | 62.7% | 93.4% | 238 (217) | inline_depth + levers |
  | work/s6/h1.cpp (b0 + `||` only) | 4209 | 58.5% | 91.4% | 238 (215) | inline_depth + 1 lever |
  | work/s6/d1.cpp (no device) | 4263 | 38.6% | 93.2% | 237 (221) | levers only |
- Next lever: make the zero piece lose >= 4 more (or activeQuests gain 4) in those three blocks. Candidates: the zero's
  references in the loop-2 blocks (the list ctor's `0`, the deallocate `if (p)`), an activeQuests reference inside the quest loop
  that costs no code, or the hand885 pk/wi.py what-if (force the const-0 piece's priority to -8) to prove the colouring is the
  only thing left before searching spellings. Then pack.py on the frame.
- Header changes this session: none.

## Session 7 (2026-11-09) -- row 0x0056AD40 CTCBoastUI::Activate: RELOCATION_MATCH, no device
- Result: src/0056ad40.cpp graded RELOCATION_MATCH (4284 v 4284, strict 1110/1110), test/0056ad40.cpp PASS (g++ C++03, tall.sh,
  VC7.1 tall.py), finish.py run (payload now 3 rows). No inline device, no /d2inl flag, no header change. 3 grades this session (all RELOCATION_MATCH: c1, the final file, the final file after a comment fix).
- How (TRACE S7.1-S7.6):
  1. What-if (wi7, forceif `_activeQuests:-7=-3` at the 6th re-sort): the d1 entry colouring follows retail -> that one comparison
     was the whole entry flip; the forced object also showed a second residue d1 had hidden: the empty-boasts continue laid out far
     and the card continue cross-jumped into it (237 v 238 calls).
  2. Layout: a LEADING folded constant (`false || boasts.empty()`) keeps the `||` frequency split but retail's in-line continue;
     a trailing one (`|| false`, d1) makes c2 move the continue block next to ++quest.
  3. Colouring without force: the boast loop through `const CBoast& boast = *it` (retail 0x0056B190 `mov edi,[esp+1Ch] ; add edi,8`)
     flips the 6th re-sort to retail's (the pieces there depend on the body's earlier splits).
  4. Frame + all remaining drift: the FableWin.pdb inline accessors (GetCost / GetReward ...) load Cost / Reward early into ebx
     (0x0056B4C0 / 0x0056B637) -> every register, the 0x208 frame and the slot order match. pack.py was not needed.
- Leaderboard (compile-only fits, work/s7/LEADER.txt):
  | source | bytes | strict | loose | noframe | calls | entry |
  |---|---|---|---|---|---|---|
  | s6 d1 | 4263 | 38.6% | 93.2% | 86.1% | 237 | this ebx, zero edi |
  | s6 b0 (s5 differ) | 4275 | 69.6% | 95.6% | 91.3% | 238 | retail |
  | wi d1_aq3 (forced) | 4267 | 44.3% | 94.7% | 91.2% | 237 | retail |
  | e02 (`false ||`) | 4268 | 40.9% | 94.5% | 87.5% | 238 | this ebx, zero edi |
  | wi e02_aq3 (forced) | 4279 | 44.7% | 96.2% | 92.7% | 238 | retail |
  | b1 (+ `CBoast&`) | 4275 | 42.0% | 97.7% | 95.1% | 238 | retail |
  | c1 / n11 / final (+ accessors) | 4284 | 100% | 100% | 100% | 238 | retail |
- Odd spellings kept (commented at the site with the retail addresses): `questCardThing == 0 && true` and `false || boasts.empty()`.
  Both fold away; they only set c2's static branch split (TRACE S6.3, S7.2, S7.5: identical bytes at any of four upstream tests;
  no natural spelling without a folded operand gives the split, fmodel.py 114/114 solutions need both).
- Next: nothing for this row. For landing: NAME_FIXES.tsv (SetStaticCast 0x0042A2DB binding, ForceClearGui / PrepareForLoad,
  CTCQuickAccessMenu::Deactivate), the hdr/ opt-ins of session 3 (hdr/README.md "Session 3"; no new ones in sessions 4-7).
