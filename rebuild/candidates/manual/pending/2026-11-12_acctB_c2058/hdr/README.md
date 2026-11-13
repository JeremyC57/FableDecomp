# hdr/ overlay (chunk 2058, row 0x00563920 CTCHeroOnlineScoreboard::DisplayScoreboardGUI)

Both changes are new opt-in blocks; the default text of each header is unchanged, so no landed includer changes bytes
(no regrade needed). Grade with `export FABLE_HDR_OVERLAY=/opt/fable_lane/w22/agent2058/hdr`.

## engine/CTCInventory.h -- `FABLE_RETAIL_CTCINVENTORY_MOUSEPOINTER` (inside the FABLE_RETAIL_CTCINVENTORY branch)
- Adds `NUISystem::CComponent* GetMousePointer() { return PMouse_Pointer; }`.
- Evidence: (1) FableWin.pdb `?GetMousePointer@CTCInventory@@QAEPAVCComponent@NUISystem@@XZ` at 0x022E66F0, the COMDAT emitted
  right after `?DisplayScoreboardGUI@CTCHeroOnlineScoreboard@@AAEXXZ` (0x022E4BC0), i.e. this row is its first user;
  (2) retail reads PMouse_Pointer +0x214 twice in place (0x005639BB, 0x005639E6) for ChangeColour / ObserveEvent;
  (3) fwsize: our /Od body 9i/20b == FableWin 9i/20b (SAME).
- Needs FABLE_RETAIL_CTCINVENTORY + FABLE_RETAIL_CTCINVENTORYBASE (+ _OPTIONSMENU for the typed CurrentOpenMenu).

## stlport/fable_algorithm.h -- `FABLE_RETAIL_STLP_FIND_INPUT`
- STLport 4.6 `_algobase.h` `find(first, last, val)` forwarding to `_algobase.c` `__find(first, last, val, const
  input_iterator_tag&)` (`while (first != last && !(*first == val)) ++first;`), both inline templates spelled FABLE_FASTCALL
  (/Gr).
- Evidence: (1) retail list<CClanMember> instances: find 0x00567A40 (landed, `ret 8`, builds both iterators with the user
  copy constructor and the tag temporary, calls 0x00567240) and __find 0x00567240 (landed, `ret 0Ch`); (2) this row calls
  find at the outer site (0x00563F35) and __find directly at the inner-loop site (0x005642FA, find expanded: the tag is
  `lea edx,[esp+27h]`), which the c2 inline trace reproduces from these two bodies (find c1 27, __find c1 80 never expanded);
  (3) Ego_r.pdb `??$_Find@V?$_List_unchecked_iterator@...CClanMember...@@VCWideString@@@std@@YI...` (the Dinkumware twin of the
  same search over list<CClanMember> with a CWideString value).
- Needs FABLE_RETAIL_STLP_FIND_IF_INPUT (stlport/fable_containers.h: the list iterator's category typedefs and iterator_traits).
