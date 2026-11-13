- (agentC3, 0x00BB3E80) Expanded SetRenderState blocks carry TWO record-keyed orders: L = which of the two bookmark loads is issued first
  (scheduler tie, IL order) and O = the test/or operand order. Measure them per expansion (tools/srs2.py) instead of counting fg hunks.
- (agentC3) L depends only on records created BEFORE each expansion (outer named locals, inline-body named locals expanded earlier); O
  depends on the function's total, including records created after the expansions and records of DEAD code. Position probes
  (tools/posprobe.py: a forced-inline helper with N dead named locals called at a chosen point) separate the two in a few grades.
- (agentC3) Per-expansion named locals inside SetRenderState (DESIRED / BOOKMARK / DIRTY) change the SLOPE of L across expansions, not
  only its offset: with them no outer count lines L and O up for this row; with the default text the L-peak recurs every ~31 units.
- (agentC3) Things that do NOT create records: references, unused named parameters, empty inline calls, `(void)expr`, `sizeof`, `__noop`,
  `if (!(c)) {}`, TU flags /Ox /Oi- /GF /Gy /Ob1 /GR /G5 /G6 /Gr /Zi /Z7, a preceding function in the TU, a visible stub callee body.
  Things that DO: named locals (class objects with field traffic weigh several units), and dead blocks `if (0) { ... call("str") ... }`
  (O only).
- (agentC3) Review at landing / project question: retail rows may carry record-only IL from release-mode ASSERTs (FableWin row bodies are
  +80..+200 /Od instructions per assert-bearing helper). 0x00BB3E80 matches with the default setter text plus 27..31 assert-shaped dead
  blocks (diagnostic). If the project adopts a shared release ASSERT form, revisit the FITTED DESIRED/BOOKMARK/DIRTY opt-ins too.
