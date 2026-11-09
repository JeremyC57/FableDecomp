# Landing 2026-11-09d: CTCBoastUI::Activate (chunk 890 session 7) + a header-merge repair

0x0056AD40 CTCBoastUI::Activate (4,284 bytes): RELOCATION_MATCH + PASS (VC7.1, g++ -std=c++03 -pedantic), no flag, no inline
device. Catalog name was the descriptive CQuestManager::UpdateUI_QuestMenu; `?Activate@CTCBoastUI@@QAEXXZ` from both PDBs.
- What it does: when the boast UI activates, closes the quick-access menu and GUI, then builds the boast menu: one entry per
  active quest with boasts (quest card name, sub list of boasts with cost / reward, detail list and backdrop components).
- How it matched (TRACE.md S5-S7 in pending/2026-11-08_linux_hand890/): c2's inline choice follows static branch frequency, not
  callee cost -- a `while` quest loop, `if (false || boasts.empty()) continue;` and `questCardThing == 0 && true` give the 0.75 /
  0.25 splits retail's call / expand pattern needs (both folded operands commented with the retail addresses; a 34-site
  constraint model, s6/fmodel.py); a `const CBoast& boast = *it` loop and the FableWin.pdb inline CBoast accessors fixed the
  remaining colouring, the frame (0x208) and slot order.
- RELOC_CONFLICTs (warnings): SetStaticCast<CList> 0x0042A2DB (queued land0809c for 0x0042A2CA) and three CCountedPointer
  destructors bound at two addresses each (0x00429B72 / 0x00429160, 0x00429C53 / 0x004291DE, 0x00429D8E / 0x00429288): unfolded
  duplicate COMDAT copies; one-address-one-name audit needed.

Header-merge repair (same commit): the 2026-11-09c landing (CManager::Action) copied its overlay over five headers and dropped
opt-ins that landings b69f839e22 / 4995f43480 had added -- FABLE_RETAIL_STLP_TREE_FIND_MEMBER_TEMPLATE (fable_map.h),
FABLE_RETAIL_CCHARSTRING_ENDSWITH + _GETASCHARARRAY_NULLRETURN (fable_string.h), FABLE_RETAIL_CMAINGAMECOMPONENT_TEXTBANK_INITIALISED,
FABLE_RETAIL_CACTIONPARAMSTRING_CTOR (NUISystem_CClickable.h), FABLE_RETAIL_CMENUENTRY_ACTIONPARAMS -- so 0x00498490 and 0x00551EA0
no longer compiled. Restored by a three-way merge (additions only); 0x00498490, 0x00551EA0 and 0x0041E6D3 regrade RELOCATION_MATCH.
The hdrcheck single-macro test missed it (the blocks were nested); overlay merges now also report LOSES_OPTINS.
