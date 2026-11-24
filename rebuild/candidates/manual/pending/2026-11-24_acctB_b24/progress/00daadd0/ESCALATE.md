# 0x00DAADD0 CQ_NewOakValeIntroScript::Init -- HYPOTHESIS, not a proven escalation (B24A12, 2026-11-24)

Residue: retail expands std::_Construct<CCharString,CCharString> inside the inlined push_back at the first 22 of 42 sites and calls the
/Gr COMDAT 0x0044B110 at the last 20 (push_back itself inline at all 42). Ours (stock /Gr) expands all 42: DIFFER(3406v3306).

Evidence (gdb traces of c2.dll 13.10.3077, scripts and logs in $S/w22/b24e/00daadd0/tr/):
1. Decision 0x1078CF54 takes the heuristic 0x1078D1AA when the caller has global opts AND the callee has its inline record and flag
   bit 2 ([sym+0x69]); otherwise the budget test 0x107284A3 (accept iff size[sym+0x6D] <= 40 or share >= size).
2. Under stock flags every decision in Init is heuristic; _Construct's heuristic adj is 3 (< inlS+5 = 14) at every site, push_back's
   score is 37 at every site (freq 5e8 constant), the 9000 size cap and depth limit (16) are never reached: the heuristic cannot give a
   per-site split. The only per-site state is the budget: R0 = 2 x caller size = 3686, R -= callee size for each accepted callee > 40
   (also in heuristic mode), nested share = (R - size) / remaining sites at that level (traced 85, 84, 84, 83 ...).
3. The TU is heuristic-mode: the landed same-TU sibling 0x00DAC760 (CNOVI_Guard::Main) is RELOCATION_MATCH under its /d2inlT72 and
   DIFFER(6201v5867) under the budget-mode emulation (`#pragma optimize("g", on)` + /Og- /d2vol10).
4. All-budget mode reproduces ours exactly in the simulator (tr/sim.py: 24 full / 9 partial / 9 called at R0 3686 = the a1 grade) and
   no R0 gives retail's 22/20/0 with push_back 80, _Construct 42. Retail's 22/20/0 fits exactly when push_back stays heuristic and
   ONLY _Construct takes the budget test, with R0 in [3524, 3603] (ours 3686).
5. Probe p2 (`#pragma optimize("g", off)` at the TU end) only shows that the instantiations' compile state changes their heuristic
   SIZE: push_back's decision lines in Init still read glob=1, record present, fl69 0x18EA (same path), and it was rejected on score
   (92 > T 74, size 134 instead of 59). Nothing run here puts _Construct on the budget test while push_back stays heuristic.
Not yet shown: a build in which push_back has its heuristic info and _Construct does not; R0 would also have to be <= 3603 (caller
IL <= 1801, ours 1843). So this is NOT proven to be compile order; the per-site split is unexplained. Untested: the FableWin `if (false) {} else for`
form of the WhichBadDeedsPerformed loop; EXTRA_RULES.md was not read this session.
