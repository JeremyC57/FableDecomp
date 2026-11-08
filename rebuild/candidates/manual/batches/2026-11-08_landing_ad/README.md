# Landing 2026-11-08ad: chunk 890 (expert lane, instrumented, Opus xhigh), 1 of 3

0x00551EA0 NUISystem::CTable::ConstructSpritesToDraw (3,849 bytes, `ret` + int3 to 0x00552DAF): RELOCATION_MATCH + PASS (VC7.1 and
g++), 12 grades, no flags. Hand-back: `pending/2026-11-08_linux_hand890/` (TRACE.md logs every measurement: inline trace, /FAs frame
table, first-diverging instruction).

- Name: Ego_r.pdb + FableWin.pdb `?ConstructSpritesToDraw@CTable@NUISystem@@MAEXXZ`; CTable vtable 0x0124A224 slot 142.
- What it does: sizes the table's tiled border from its def, adds the four corner sprites as children, then draws the four edges and
  the horizontal / vertical separation lines through virtual slots 143 / 144 (ConstructHorizontalLine / ConstructVerticalLine).
- Source shapes found by instrumentation: one size loop in `while` form (c2's call-site frequency puts the corner code at retail's
  inline split); separation-line calls as if/else pairs (a `?:` argument adds a conditional-temporary flag slot); locals declared
  in retail's order; edge totals written inline (x87 CSE).
- Headers (additive opt-ins, hdrcheck default path unchanged): engine/NUISystem_CTable.h `FABLE_RETAIL_CTABLE_CONSTRUCT` (slots
  142-144), stlport/fable_map.h `FABLE_RETAIL_STLP_TREE_FIND_MEMBER_TEMPLATE` (owner decision D1 for the non-const find).
- Names: labels `ghidra_out/labels_landing_2026-11-08ad.tsv` (also 0x0056AD40 = CTCBoastUI::Activate, manifest renamed from the
  descriptive CQuestManager::UpdateUI_QuestMenu; 0x00498490 DrawRetailDisplay); 2 respellings queued as land0811ad (0x00553520
  placeholder find; the landed caller 0x00499880's DrawProgress spelling).
- Parked, 0 grades (session budget): 0x0056AD40 CTCBoastUI::Activate (decoded in differ/, needs CTCBoastUI / CQuestManager / UI list
  opt-ins) and 0x00498490 CProgressDisplay::DrawRetailDisplay (retail layout derived, first third decoded).
