# Chunk 2025 -- lessons from the authoring agent (fold into GOTCHAS at landing)

- (chunk 2025, 0x00600B60) CTCInventoryMap's region builders share one body shape: the landed ConstructRegionDescriptionShops
  0x006001E0 source carries over 1:1 to BuyableBuildings (same opt-ins, same-TU GetDef / map::operator[] bodies under
  auto_inline(off)); only the TC id, the counted kinds and the text-bank declaration point change. Matched at the first grade.
- (chunk 2025, 0x00600B60) A list walk whose building pointer is re-read from the node after a call (`mov ecx,[ebp+8]` after
  IsForSale) while the first uses share a register is `(*it)->...` written at each use, not a named `building` local.
- (chunk 2025, 0x00605570) Two complete copies of the same search loop in retail (0x006055F7 / 0x00605660, the second after
  `mov byte [esp+28h],1`) are two source loops in the arms of an if/else: a single loop after `if (a == b) flag = true;` compiles to
  one loop (and `bool flag = a == b;` to sete). The arm laid out first is the arm written first (`!=` first here).
- (chunk 2025, 0x00605570) STLport find() over a vector<long> homes its random_access_iterator_tag temporary in a LIVE argument slot
  (0x00605ABE `lea eax,[esp+74h]`, region) only when the same-TU __find body (STLport 4.6 _algobase.c, under auto_inline(off)) is
  visible; declaration-only the tag takes a frame byte and every register after it shifts (EXTRA_RULES "tag on a LIVE argument").
- (chunk 2025, 0x00605570) c2 regime-0 inline score for f >= 1 fits score = c1*(1/f + 2)/3 (GetDef c1 106: f 1 -> 106, f 7.5 -> 75;
  Reset c1 95: f 3.75 -> 71); for f < 1 the points fit sqrt(f) in place of f (Reset f 0.75 -> 99, f 0.5 -> 108). Some callees add a
  constant on top (LowerBound c1 52 at f 3.75 -> 66), so read scores from the trace, do not compute them.
- (chunk 2025, 0x00605570) A `for` search loop with `break` leaves the code after it at the loop body's frequency (7.5 here); the same
  loop as `unsigned long i = 0; while (i < n) { ...; ++i; }` drops it back to the entry's (1.0), same loop code.
- (chunk 2025, 0x00605570) Inline-budget proof pattern: when a callee retail expands (GetDef, c1 106) sits at the join of an if/else
  whose arms hold a callee retail calls (Reset, c1 95), at T 74 the expansion needs F > 10.6 and the call needs F/2 <= 2.97, so no
  control flow fits; a single per-row /d2inlT works only where score(Reset at F/2) >= T > score(GetDef at F) -- here T 107 with the
  `while` loops. The same TU's Shops / BuyableBuildings match at the default T.

Review at landing:
- 0x00605570 lands with `// Flags: /d2inlT107` and an `// Inline-budget:` line (owner decision 2026-11-08, preference 2: per-row flag,
  source untouched). The proof in NOTES.md shows no control-flow spelling fits at T 74 with the measured costs; retail's c2 must have
  seen a different GetDef / Reset cost in this function (same TU as the device-free Shops / BuyableBuildings rows). If the cause is
  found (header body difference or budget consumer), drop the flag and regrade.
- New shared header engine/CRegionDef.h (overlay; retail offsets from the landed CRegionDef::Transfer 0x004715AB model).
- Landed rows with wrong spellings (NAME_FIXES.tsv): CTCInventoryMap_ConstructRegionDescriptionMapKeys_005fb7e0.cpp (QAE + CRegion*,
  plus local stand-in CDefPointer / FindDef_s7r_0 / ReleaseDef_s7 models), global_Find0050e540_0050e540.cpp (STLport `__find`),
  std_vectorinsert40byte_00618420.cpp (vector<CUIRegionInfo>::_M_insert_overflow).
- The two wide literals in 0x00605570 (L"" 0x0122D70C, L"\n" 0x01231728) are bound in reloc_bindings.tsv to stand-in data names from
  other rows (g_emptyWideString_0122d70c, _s_newLine); here they are plain literals (rel.py DIFF on those two only, vland PASS).
