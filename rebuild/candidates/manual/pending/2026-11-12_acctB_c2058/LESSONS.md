# Chunk 2058 -- lessons from the authoring agent (fold into GOTCHAS at landing)

- (0x00563920) A large row whose first draft calls everything right but EXPANDS the destructors / small helpers retail calls
  (or the reverse) is often ONE static-frequency difference: read the c2 inline trace (bin/inl.sh, ~30 s) and solve
  c1*(1/f+6)/7 >= 74 for every site at once before touching helper texts. Here 17 dtors, a pair copy and a find all flipped
  with one extra halving ahead of the loop; once the split was right the whole frame (0x120, ~60 slot offsets) came out exact.
- (0x00563920) VC7.1 folds a null test of a pointer already dereferenced in the function (`frame->AddPositionChild(title); ...
  if (frame) {...}`) and a repeated member test with no call in between, but the static-frequency estimator still counts the
  branch: an `if (p)` around a loop halves every inline score in it with no byte of code. A test that is not provably redundant
  (null test before the first dereference, a second HasTC) emits code.
- (0x00563920) `container.size() > 0` on a vector of 8-byte elements is retail's `sar ecx,3 ; test ecx,ecx ; jbe`; `size() == 0`
  becomes `test ecx,0FFFFFFF8h ; je`.
- (0x00563920) `std::find(...)` assigned to a named iterator compares the hidden-result slot with a reloaded end(); the unnamed
  `find(...) != c.end()` evaluates end() into a callee-saved register first (also r3019).
- (0x00563920) STLport find over a list: the first (colder) site calls the /Gr `find` COMDAT, the inner-loop site expands find
  and calls `__find` directly (tag temporary `lea edx,[esp+27h]`): same template, two inline scores (c1 27 at f 0.039 / 0.195).
- (0x00563920) CCountedPointer destructor COMDATs that are a bare `jmp ReleaseData` (0x00429C53, 0x00429B72) mean the original
  destructor is `{ ReleaseData(); }` (FABLE_RETAIL_CCOUNTEDPOINTER_RELEASEDATA; FableWin dtor 8 instructions).

Review at landing:
- The row needs FABLE_HDR_OVERLAY with the two new opt-ins (hdr/README.md): engine/CTCInventory.h FABLE_RETAIL_CTCINVENTORY_MOUSEPOINTER
  and stlport/fable_algorithm.h FABLE_RETAIL_STLP_FIND_INPUT (default text unchanged; no landed includer affected).
- `if (frame)` around the activity loop is a FITTED, code-free branch (NOTES.md lists the equivalent spellings graded).
- CTCHeroOnlineScoreboard has no engine header; this row, ChooseAClan 0x0055FEC0 and the other TU rows carry local models of it.
- Landed bindings to fix (NAME_FIXES.tsv): 0x00570332 binds ??1?$CCountedPointer@VCComponent...@ to 0x004291DE and 0x0056F615
  binds ??1?$CCountedPointer@VCAction...@ to 0x00429160 -- both addresses are ReleaseData; the destructors are 0x00429C53 / 0x00429B72.
- Landed names to respell: 0x0055E7C0 (CNameTable_0055e7c0::GetEntryName), 0x0055E6B0 (global const CActivity), 0x00567A40 /
  0x00567240 (local list<CWideString> / FindInListRange models), 0x00566FE0 (CUnproven placeholders), 0x00564B20 (SPairLongCSecond).
