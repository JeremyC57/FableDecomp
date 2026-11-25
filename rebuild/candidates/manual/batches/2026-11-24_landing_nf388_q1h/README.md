# Landing nf388 q1h: 7 rows push retail's string literal (ruling q1: static proof)

These rows' tests are self-contained copies of the function, so they pass whatever the literal is. Ruling q1 (advisor decision, owner delegated) lands them on the static proof:
- retail bytes are the primary source;
- the code bytes are unchanged (COMDAT-keyed comparison is literal-only);
- the patched grader (7f84efd57a) shows LITERAL_DIFF going from 65 lines to 0 (NF388, both sides at one path; this landing's log has 0);
- each test's body copy now carries the corrected literal and asserts the retail string.

Linked tests are queued debt. q1 and q1h are one 42-row batch split for landing: q1h holds the 7 rows other T3 lines reserve for ports (005621b0 005ea9b0 0073eac0 0073fdb0 0088f7d0 009574d0 00b3b240), landed with those lines set aside and restored, as in nf386.
- Swapped pairs, decided from structure before landing:
  - 0048f939: the literals were crossed, not the member names. By the Ego_r layout retail fills Button1Text (+0x64 at retail's +0x10 shift) with L"TEXT_GUI_CANCEL" (0x012381E4) and Button2Text with L"TEXT_GUI_CONFIRM" (0x012381C0); the coordinator read both strings in the exe.
  - 00df2380: the arguments were crossed. The PDB signature and the FSE manifest give (defName, position, radius, scriptName); retail passes CREATURE_MINION_WARDOG / SummonerMinion.
  - Evidence: pending/2026-11-25_nf388_handback/SWAP_DECISIONS.txt.
- NF388 gates: HEAD and fixed 42 RELOCATION_MATCH; pick-any checkers identical, 0 reach flips; /FORCE link duplicates 0 -> 0; no includers; 34 of 41 tests also pass natively under g++.
- Coordinator checks: landed 7 of 7 with 0 LITERAL_DIFF; no table or self_symbols change; T3 and claim holds checked; baselines: bindings multi 1, funclets 0 broken, S units 3 OK.
