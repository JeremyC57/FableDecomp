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
