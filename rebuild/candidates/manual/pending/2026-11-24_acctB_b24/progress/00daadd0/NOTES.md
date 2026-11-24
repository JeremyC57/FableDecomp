# 0x00DAADD0 NScript::CQ_NewOakValeIntroScript::Init -- B24A12 (2026-11-24)

## Lessons that apply (read first)
- Inline-budget residue (retail: 22 sites expand std::_Construct<CCharString>, 20 call the __fastcall COMDAT 0x0044B110; push_back
  inlined at all 42). Old knob sweeps (/d2inl*) are all-or-nothing (hand862/865 ATTEMPTS.log).
- GOTCHAS landing fw: c2 has two inliners (heuristic with inline-info record, budget path otherwise); budget = max(1000, 2*caller size).
- FableWin 0x03803D30 (Init, /Od): 26 member stores, the `if (false) {} else for` loop over WhichBadDeedsPerformed, then 42
  x (CCharString(lit,-1) temp, push_back, ~CCharString); nothing else (no asserts, no helper).

## c2 mechanism measured this session (gdb traces, $W/tr/: inl_gdb.py, run.py, sim.py; traces *.trace.txt)
- Site walker 0x1078CEE4: [esp+0x18] = remaining candidate sites at this level (Init top level: 43 = CTimer op= + 42 push_back;
  CCharString ctor/dtor are not candidates), [esp+0x28] = budget R. Decision 0x1078CF54: heuristic 0x1078D1AA if global opts
  [0x108A91EC] && callee record [sym+0x99] && fl[sym+0x69]&2 && !(fl&0x10); else budget test 0x107284A3: accept iff
  size[sym+0x6D] <= 40 or share >= size. Accepted callee > 40: R -= size (0x1078D4C2) -- this happens in heuristic mode too.
- Nested budget for an accepted callee = (R - size) / remaining-sites-at-this-level. Init: R0 = 3686 (= 2 x 1843),
  push_back size 80, _Construct 42, operator new 16, __false_type ctor 16, _M_insert_overflow 185.
- Stock /Gr (heuristic mode): _Construct adj 3 < 14 -> always inline (all 42). push_back score 37 < T 74.
- Budget mode (`#pragma optimize("g", on)` + /Og- /d2vol10): DIFFER(3000v3306): 24 full sites, 9 push_back-inline/_Construct-call,
  9 push_back calls (sim.py reproduces 24/9/9 exactly). /Og- alone (vol 0..5) 1757 (push_back outlined everywhere).
- sim.py: NO R0 gives retail's 22 full + 20 partial in all-budget mode with sizes 80/42. Feasible only if (a) push_back is never
  budget-rejected and only _Construct takes the budget test: need 3524 <= R0 < ~3603 (ours 3686), or (b) P ~ C, or (c) >= 17 more
  counted sites after the push_backs.
- Probe p1 (`#pragma optimize("g", off)` round _Construct's definition): no effect -- the trace shows _Construct's record and flags
  unchanged, so the probe tested nothing.
- Discriminator: sibling 0x00DAC760 matches under its /d2inlT72, DIFFER(6201v5867) under budget-mode emulation -> TU is heuristic.
- Probe p2: `#pragma optimize("g", off)` at TU END -> all instantiations compiled without global opts, push_back heuristic size 134
  (59 normally) -> every push_back called (1757). p3/p4 (explicit push_back instantiation after / before Init + that pragma): same.
- Stock-flag traces: no decision in Init reaches the budget test; heuristic per-callee constants (adj 3, score 37), cap/depth unreached.
- State: OPEN. ESCALATE.md holds a hypothesis only (mixed heuristic/budget path), not a proof. Best source = differ/ (stock /Gr, 3406v3306). Next idea if reopened: find a build in
  which push_back has heuristic info and _Construct not (flag bit 2 of [sym+0x69] clear / no record [sym+0x99]); writer of bit 2 /
  record at c2 0x107FF746..0x107FF768. R0 would also need <= 3603.
- Untested: FableWin's `if (false) {} else for` loop form; EXTRA_RULES.md not read this session.
## B24S2 (2026-11-24): 2 grades, no change
- s2/f1.cpp: FableWin's `if (false) {} else for (long i = 0; i < 5; ++i)` loop form = DIFFER(3406v3306) 35 hunks (unchanged; the split
  is a budget question, not a frequency one, as the trace said: push_back score 37 << 74 everywhere).
- Probe s2/p1.cpp (s2/ph overlay, diagnostic): `_Construct` as a template WITHOUT the `inline` keyword (hypothesis: flag bit 2 = declared
  inline, which would send only _Construct to the budget test) = unchanged 3406v3306. So the keyword is not the bit.
- Still open; ESCALATE.md hypothesis stands. Next: identify what writes [sym+0x69] bit 2 / [sym+0x99] at c2 0x107FF746..768 (B24A12).
