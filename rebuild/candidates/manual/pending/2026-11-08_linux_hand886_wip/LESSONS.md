# Chunk 886 -- lessons from the authoring agent (fold into GOTCHAS at landing)
- Wine lane: a FATAL cl.exe error (C1189 from a header `#error "... define FABLE_RETAIL_X"`, or C1075/C1004 from an unclosed
  brace/namespace) makes cl.exe spin with no output until q.py's 600 s timeout; plain C2xxx errors return at once. Run
  `g++ -E -P -x c++ -I<overlay> -Irebuild/include src.cpp` first: it reports the #error in 0.02 s.
- Overlaying stlport/fable_containers.h needs copies of rebuild/include/fable_string.h AND stlport/fable_string.h in the overlay
  too: stlport/fable_string.h includes "fable_containers.h" by its own directory first, so the repo copy otherwise wins.
- `mov [ebp-6xx],hdr` (a dead home store of a list header) then `[hdr+4]`, reload `[hdr+4]` after a call, `[x+4]` again =
  `std::list<T>::reverse_iterator it = l.rbegin(); *it ... *++it` (0x0041F9AA, 0x00420C03, 0x00422023); `l.end()` with `*--it`
  leaves no home store (0x0041EFAB, 0x004226D8).
- VC7.1 compiles a by-reference `CCountedPointer<CComponent>` argument from a named local exactly like the temporary
  (ctor 0x429C15 / call / dtor 0x4291DE): write the named local (portable; no non-const ref to a temporary).
- In a 35 KB size-optimised function, same-TU callees defined before it (setters 0x41CA31.., list begin 0x429A85) are inlined in
  early cases and called in late ones (0x0042247E inline vs 0x00422547 call of the same setter): the caller inline budget, not
  the callee; out-of-TU header-inline accessors (CTCQuestCompletionUI::IsActive) stay inline throughout.
Review at landing:
- Row needed > 4 grades and is not matched (parked; WORKLOG.md has the resume plan).
- Local models next to existing engine headers: CTCInventory, CTCInventoryStats, CTCInventoryMap, CTCInventoryTrade,
  CTCInventoryClothing, CTCInventoryExperience, CTCInventoryAbilities, CTCQuickAccessMenu, CTCInGameMenu, CPlayerGui, CWorld,
  CQuestManager, CTCBoastUI (see WORKLOG.md open problem 4).
- CUserProfileManager overlay accessors SetNewProfileToUseWASD / WereVideoChangesAccepted read opaque bytes (+0x78 / +0x13C);
  type the members when the layout opt-ins are merged.
