# Hand chunk 890 (NUISystem::CTable .. CProgressDisplay): 1 of 3 byte-exact

## What matched (1)
- NUISystem::CTable
  - 0x00551EA0 ConstructSpritesToDraw (3,849 B, RELOCATION_MATCH, 12 grades): sizes the tiled border from the def's ExpansionType
    (TOP_MIDDLE width / MIDDLE_LEFT height, separations rescaled to tiles), adds the four corner sprites as children through
    CManager::CreateComponent, then draws the edges and separation lines through the CTable virtuals ConstructHorizontalLine /
    ConstructVerticalLine (slots 143 / 144). Shape lessons, each found with a probe (TRACE.md E1-E8): the line calls are if/else
    pairs (a `?:` argument adds a conditional-temporary flag, /FAs frame table); the inline split (tree find called at three sites,
    CCountedPointer(T*) expanded once) follows c2's static call-site frequency -- after a `for` loop the frequency stays at the loop
    body's, after `while (i < n) { ...; ++i; }` it does not -- so exactly one of the two size loops is the while form; the four
    position vectors are declared before the integers (immediate zero stores) and the integers in retail's order (imul destination);
    the edge totals are written inline (fst + CSE, not a named float); FABLE_RETAIL_STLP_MAP schedules the state-0 key store;
    `position.X = bottomLeftPosition.X + horizontalSize` picks the load source; D1 member-template find gives the 0x00553520 symbol.

## Parked for Fable 5.1 (2)
- 0x0056AD40 CTCBoastUI::Activate RELOCATION_MATCH (session 7): src/0056ad40.cpp + test/0056ad40.cpp PASS; no device, no flag; two folded condition operands (card `&& true`, `false || boasts.empty()`) set c2 static branch splits; `const CBoast& boast = *it` + FableWin.pdb CBoast accessors give retail colouring and frame (WORKLOG "Session 7", TRACE S7.1-S7.6). Extent: 0x0056AD40..0x0056BDFB `ret` (4,284).
- 0x00498490 CProgressDisplay::DrawRetailDisplay DIFFER(-v5101): not graded -- retail layout derived, first third decoded (differ/00498490.cpp) | tried: identification + layout + partial decode, 0 source attempts (session budget) | next: engine/CProgressDisplay.h overlay (layout in the differ header), finish the decode from 0x00498972, grade; expect x87-order and C2DBoxF copy residues

## Skipped
- none beyond the two parked rows above (both stopped by the session budget, not by a lane gap).

## Label corrections
- 0x0056AD40: catalog / manifest "CQuestManager::UpdateUI_QuestMenu" (forge-autoname3) is CTCBoastUI::Activate
  (?Activate@CTCBoastUI@@QAEXXZ: FableWin.pdb + Ego_r.pdb; the landed caller 0x0056BE00 already binds that name).
- 0x00498490: the landed caller 0x00499880 binds ?DrawProgress@CProgressDisplay@@QAEXABVCCharString@@M_N@Z; the PDBs have no
  DrawProgress -- it is ?DrawRetailDisplay@CProgressDisplay@@AAEXABVCCharString@@M_N@Z (NAME_FIXES.tsv).
- 0x00553520: landed as the placeholder `?find@?$_Rb_tree@UCUnprovenKey_00553520@@...`; it is the STLport member-template find on
  map<ETableSprites, CCountedPointer<CComponent> > (NAME_FIXES.tsv, D1).
- Manifest labels inside 0x0056AD40 seen to be wrong (hints for the next session, not proven here): 0x00429C15 is
  CCountedPointer<NUISystem::CComponent>(CComponent*) (label CCountedPointer_00428b10), 0x0042AA29 is CCountedPointer<NUISystem::CAction>(CAction*)
  (reloc binding ??0CActionPointer@NUISystem@@...: no such class; CComponent::AddAction takes CCountedPointer<CAction>&),
  0x004B87D0 is a list<CQuestManager::CBoast>::size() (label CGuiControlTreePane::SortTreeRecursively), 0x004B8C00 the list clear.
