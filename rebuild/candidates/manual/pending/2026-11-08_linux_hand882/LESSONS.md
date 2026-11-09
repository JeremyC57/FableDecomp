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
- (2026-11-09, 0x0061E6D0) Regime 0 (5027 tuples). The regime-0 caller-size penalty is pen ~ sz*(a+g)/18256 and only once a+g > 9000
  (g = the caller's running inline size, 2473 at the first site, ~11000 at the end here); the inline trace (tools/inl) prints a, g,
  pen, f and the score per site, which turns "retail spends more budget" into a number per site.
- (2026-11-09, 0x0061E6D0) The two STLport 4.6 layers of chunk 883 (FABLE_RETAIL_STLP_LIST_CREATE_CONSTRUCT, ..._ALLOC_PROXY_REBIND)
  add ~360 g by the take block of this row too and make the state-origin ConvertVector (0x00620F0A -> 0x0052E580) a call with no
  device (sc 74 at g 9175); they are the natural first budget consumer for any row with CAction list push_backs.
- (2026-11-09, 0x0061E6D0) `new T` puts T's constructor site at half the block frequency (the null test): CAction() at f 11.7 where
  the CCountedPointer<CAction> of the same statement is at f 23.4.
- (2026-11-09, 0x0061E6D0) A budget-only explanation must fit every site at once: retail calls the teleport CAction() (f 11.7 needs
  a+g >= ~12900) but expands pop_action's CCountedPointer ~1000 g earlier (needs a+g < ~9300) -- infeasible, which pointed at the
  site frequency: the take block follows the boast `for` loop and inherits its 5x (375 instead of 75).
- (2026-11-09, 0x0061E6D0) `for (it = c.begin(); it != c.end(); ++it) {...}` -> `it = c.begin(); while (it != c.end()) { ...; ++it; }`
  over a std::list is code-identical in VC7.1 (same rotation) and drops the code after the loop back to the outer frequency: the
  take block 46.9 -> 9.4, which made the zoom entries and ConvertVector #2 calls and left a single consistent budget window
  (Delta g 724..1385 before pop_action's CCountedPointer); 3 dummy inline statements in that window reproduce all six late out-of-line
  sites with no inline_depth device (b1x3).
- (2026-11-09, 0x0061E6D0) An inline_depth(0) region whose statement has inline argument code (`ConvertVector(misc_def->X)`) adds
  calls that also move register allocation: removing it took regs+frame share from 97.03% to 98.05% (b2).
