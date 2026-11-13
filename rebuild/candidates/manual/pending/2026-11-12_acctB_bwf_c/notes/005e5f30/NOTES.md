# 0x005E5F30 CTCQuestCompletionUI::ActivateReal -- RELOCATION_MATCH (13 fg grades + 5 q.py compile-fix runs)
- What made it match: full draft from OUTLINE on the shared headers, then (1) NO /d2inlT60 /d2inlS48 (the GetQuestDescription TU-mate flags
  flip every inline decision here: bar ctors called, erases expanded, CDeletion copy called); flags = /GS only; (2) the two-string picks
  (screen, viewport, container) as if/else statements (VC tail-merges the calls), the viewport pointer zero-initialised and set in the
  found arm; (3) PC_QUEST_ICON looked up in each arm of `Completion != 3`; (4) pmenu held as CComponent* (cast to CList at
  AddNonScrollingChild) -- that alone fixed the pmenu register (ebx in every arm) and merged the case-0/default menu tails and the frame
  size (0xA4 -> 0x8C); (5) FABLE_RETAIL_STLP_LIST_DTOR_OUTOFLINE (~list 0x0042ABCA called after SetDeletion); (6) the model's mesh view
  `CCountedPointer<NUISystem::CMesh> pmesh; pmesh.SetStaticCast(pmodel);` (FableWin ??$SetStaticCast@VCComponent@NUISystem@@@?$CCountedPointer@VCMesh@NUISystem@@@@..).
- Headers / flags: Flags /GS; overlay opt-ins in hdr/README.md (ACTIVATEREAL, MINIMAPMARKERS, FINISHSCREEN, RIGHTHANDEDSET, CTCHEROSTATS_TOTALS,
  CTCHEROEXPERIENCE_TOTALEXPERIENCE). Repo opt-ins defined: see the row's #define list (STLP_MAP, STLP_TREE_ERASE(+_BODY), STLP_LIST_DTOR_OUTOFLINE,
  CTHING_GETTC_EXPANDED, CGAMEEVENT_INLINE, CSOUNDINIT_INLINE_CTOR, CTCHEROSTATS_SERIALISE layout, ...).
- Names: CThing access is CTCBase::GetParentThing() (the header has no `Thing` member). Made-up local names only (pscroll, phelpers, pbar, ptext,
  pdescription, pviewport_movie, cursor_name ...). Landed names proven wrong -> NAME_FIXES.tsv (0x005E8F50, 0x004B2FE0).
