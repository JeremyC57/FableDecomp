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
