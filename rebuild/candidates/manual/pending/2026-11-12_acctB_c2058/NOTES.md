# Hand chunk 2058 (CTCHeroOnlineScoreboard): 1 of 1 byte-exact

## What matched (1)
- 0x00563920 CTCHeroOnlineScoreboard::DisplayScoreboardGUI (3143 bytes) -- RELOCATION_MATCH (src/00563920.cpp), behaviour test
  PASS under g++ and VC7.1, read-only vland RELOCATION_MATCH PASS. 7 candidate grades + 8 full-row diagnostic probes (grades.log),
  plus c2 inline traces (bin/inl, not grades).
  - Builds the PC clan scoreboard: PictureCache.clear(), ScoreboardState = 0, CManager::GetInstance; GetTC(TCI_INVENTORY) /
    CurrentOpenMenu 8 / PScoreboardScreen early returns; mouse pointer ChangeColour(white, 1.0, false) + ObserveEvent(MOUSE_MOVEMENT);
    PConnectingMessage1->ChangeState(INACTIVE); IsScoreboardEmpty -> TEXT_GUI_MENU_CLAN_EMPTY + DisplayErrorMessage; else the
    PC_SUB_LIST with the clan name title, a DELETE action on inactive, added to PClanMenuEntry, 39 offsets; per activity 0..38 with
    a non-empty downloaded vector: menu entry (activity name), best score (GetScoreAsString), holder, button, subtitle, the activity
    description when the holder is a clan member, and a sub list of every record (score entry, holder, subtitle, the member's
    description when found, the member's picture from GetCachedImage(L"MemberData", MemberID)); then ProcessEvent(SELECT) on
    PClanMenuEntry, PClanMenuEntry = 0, ScoreboardState = 0, TaskState = 100.
  - Model: the same-TU landed ChooseAClan 0x0055FEC0 header set (shared CThing / CTCBase / CCountedPointer / fable_map / NUISystem
    CComponent / CList / CSprite / CManager / CAction / engine/CText.h, CMainGameComponent text bank) + shared engine/CTCInventory.h
    with the new FABLE_RETAIL_CTCINVENTORY_MOUSEPOINTER opt-in, STLport find / __find via the new FABLE_RETAIL_STLP_FIND_INPUT
    opt-in (hdr/README.md). CTCHeroOnlineScoreboard has no shared header: the class is a local model of the Ego_r.pdb class with
    retail STLport sizes, exactly as the landed ChooseAClan (header debt: an engine/CTCHeroOnlineScoreboard.h for the TU).
  - The first draft had every instruction family right but expanded 17 ~CCountedPointer, the pair copy and the outer find that
    retail calls. The c2 inline trace (bin/inl.sh, copy of agent2003's) showed one cause for all of them: the static frequency
    ahead of the activity loop is half of retail's (regime 0: expand iff c1*(1/f+6)/7 < 74). A diagnostic extra early exit
    reproduced retail's call/expand split exactly (inner-loop dtors f 0.195 -> score 74 = refused, after-loop dtors 0.156 -> 83,
    outer find 121, pair copy 94, inner find 42 -> expanded) and with it every frame slot (the instruction and frame residues
    were all downstream of the inline split).
  - FITTED v PROVEN:
    - `if (frame) { for (...) {...} }` -- FITTED. The halving must come from a caller-level branch that emits no code. VC7.1
      folds a null test of a pointer that was already dereferenced (frame->AddPositionChild(title) precedes it), so the test
      leaves no byte but its 0.5 split stays in the estimate. Equivalent alternatives graded RELOCATION_MATCH: `if (list)`,
      a repeated `PScoreboardScreen == 0` / menu test, `|| inventory == 0` after the menu dereference, `this == 0 ||`. A test that
      is not folded (inventory == 0 before its dereference, HasTC + tested GetTC) emits code (3151v3143 / 3159v3143).
    - FABLE_RETAIL_CCOUNTEDPOINTER_RELEASEDATA (`~CCountedPointer() { ReleaseData(); }`, Data zeroed before Info) -- PROVEN:
      retail dtor COMDATs 0x00429C53 / 0x00429B72 are `jmp ReleaseData`, FableWin dtors are 8 instructions, 0x0041BC90 zeroes
      Data first. The row also matches without it (the dtor cost is 47 either way).
    - `size() > 0` guard -- PROVEN by the bytes (`sar ecx,3 ; test ecx,ecx ; jbe`; `== 0` gives `test ecx,0FFFFFFF8h`).
    - Outer `std::list<CClanMember>::iterator holder_member = std::find(...)` named -- PROVEN by the bytes (the compare reads the
      result slot against a reloaded end(); the unnamed `find(...) != end()` keeps end() in ebp and spills the component).
    - `std::pair<float, CWideString> record = ...[0];` then `record = ...[j];` in the inner loop -- PROVEN (one pair at +0x34,
      copy ctor 0x00564B20 once, the implicit assignment fld/fstp + operator= in the loop).
    - GetMousePointer() accessor -- PROVEN (FableWin COMDAT after this row, body SAME under fwsize); CurrentOpenMenu read directly
      (the landed ChooseAClan spelling; GetCurrentOpenMenu not tried).
  - Names: all callees in their Ego_r / FableWin spellings (header comment). Descriptive: none in the row; the enumerator count
    39 (EOnlineScoreboardActivity has no enumerator names in the name sets) and CTCInventory menu 8 stay literals as in ChooseAClan.
  - vland warnings: RELOC_NAME for ActivityEnumToCharString / find / the three dtors (catalog labels are wrong, NAME_FIXES.tsv);
    RELOC_CONFLICT for ~CCountedPointer<CComponent> (0x00429C53 here, 0x004291DE in landed 0x00570332) and ~CCountedPointer<CAction>
    (0x00429B72 here, 0x00429160 in landed 0x0056F615): 0x004291DE / 0x00429160 are the ReleaseData bodies the dtor thunks jump to,
    so the landed rows' bindings are the wrong ones (NAME_FIXES.tsv), not this row's.

## Parked for Fable 5.1 (0)

## Skipped
None. Start 0x00563920 (`push ebp` after the previous function's padding) and end 0x00564566 (`ret`, next function 0x00564570)
confirmed; length 3143 is right.

## Label corrections
See NAME_FIXES.tsv: 0x0055E7C0 (GetEntryName -> ActivityEnumToCharString), 0x0055E6B0 (nested non-const CActivity),
0x00567A40 / 0x00567240 (find / __find over list<CClanMember>), 0x00566FE0 / 0x0056A680 (the DownloadedClanScoreboard tree),
0x00564B20 (pair<float,CWideString> copy), 0x00429C53 / 0x00429B72 / 0x0041BC90 (CCountedPointer destructors) and
0x004291DE / 0x00429160 (ReleaseData, mis-bound as destructors by two landed rows).
