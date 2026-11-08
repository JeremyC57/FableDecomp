# Chunk 882 -- lessons from the authoring agent (fold into GOTCHAS at landing)

- (0x0061E6D0) A loop's form moves VC7.1's inline decisions inside AND after it: `unsigned long i = 0; if (v.size() != 0) { for (; i != v.size(); ++i) ... }`
  (the guard merges with the rotation test, no extra code) cools the loop body (push_back sites 2-4 called, 1/5 expanded, as retail)
  and the code after it (the next GetTC's LowerBound called 0x0040F020), and gives retail's prologue registers and the `i = 0` store
  before the `je`. `for (...; ++i)` alone expanded site 2; `for(;;)` with `++i;` in the body / do-while expanded all sites.
- (0x0061E6D0) Proving a whole-function inline budget: 4 extra inline `CCountedPointer<CAction> p(new CAction)` statements before a loop
  turn the LATE sites (CCountedPointer / CAction ctors, ConvertVector) into calls while the earlier ones stay expanded -- the budget
  is spent in source order, so a late "retail calls it" residue means retail spent more budget earlier, not that the late site is
  colder. /d2inlS0..S20 changed nothing on this 12 KB row; /d2inlT moved unrelated sites first; the other c2 knobs
  (/d2inlcrmax, /d2inluserinl, /d2inlnlw, /d2inlfcsa, /d2inlfcsw) had no effect.
- (0x0061E6D0) Making CDefPointer's members plain inline, adding an STLport allocator wrapper level, or using trivial inline accessors
  (GetGoldReward etc.) does not change the budget; plain-inline GetTC makes the LAST GetTCs out-of-line calls (retail never calls GetTC).
- (0x0061E6D0) `#pragma inline_depth(0)` round `C2DVector v = F(member)` also outlines CDefPointer::operator-> and the C2DVector copy ctor
  of the by-value argument (two extra calls); only a statement whose operands are already plain locals is safe to wrap.
- (0x0061E6D0) `p += origin` (inline operator+=) keeps the second product on the x87 stack and stores straight into the member, as retail
  (0x00620FC1..0x00621025); `p = origin + p` builds a temporary and copies it.
- (0x0061E6D0) In an inner `for (j...)` whose begin/count are locals, retail stores `j = 0` before the begin/count stores: declare
  `unsigned long j = 0;` first and write `for (; j != count; ++j)`.
- `#error` in an overlay header makes Wine cl.exe spin until the timeout (like a missing include); use a negative-array typedef
  to prove an overlay is reached.

Review at landing:
- 0x0061E6D0 needed 95 grades (55 source); parked. differ/0061e6d0.cpp carries six `#pragma inline_depth(0)` regions as an
  inline-budget stand-in (device 3) -- not a landing candidate until the budget consumer is found and the register residue goes.
- NAME_FIXES.tsv: 0x00411570 (strcmp_signed -> CBasicString<char>::Compare), 0x007022A0 (IsCore -> IsExclusive), 0x007022E0
  (CanBeHitByMeleeStrike -> CTCQuestCard::IsVignette).
- Landed CloseInventoryPC 0x00621720 calls CTCInventoryBase+0x20 "quest cards"; it is the base's Categories.
- New shared headers engine/CTCInventoryQuests.h and engine/CQuestCardDef.h (hdr/README.md); enumerator names of QuestScreenType and
  EQuestCardPrerequisite are descriptive placeholders (naming queue).
