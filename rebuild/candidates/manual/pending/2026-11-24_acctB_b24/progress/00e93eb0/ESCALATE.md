# 0x00E93EB0 CV_HauntedHouseScript::Main -- escalation (B24A5, 2026-11-24)
Retail's two out-of-line calls at the end of Main -- `vector<CScriptThing>::operator[]` (0x00E9520A -> 0x00CD1FB1) in the last candle
loop and `TRetVoid::Return` (0x00E9524E -> 0x00E95380, the only caller of 0x00E95380 in Fable.exe) at the final terminate check -- are
c2 BUDGET-PATH refusals: retail's c2 compiled Main before those two callees, and Main's running budget total passed 35000 exactly there.
Evidence: diagnostic trace inl/f1.txt (callees forced down the budget path, no source change): totals 3270 .. 34701 accepted through the
last loop's terminate Return, then 35072 -> operator[] and the final Return rejected; nothing else changes. A DIRECT call to an already-compiled
empty callee disappears even on a "no" decision (v5 with inline_depth(0) round the call; f1 forced-budget run: no call, _Fail_Val
dropped); only the Codex pointer-to-member spelling keeps the call when Return is compiled first. f1 shows the operator[] call in the
last loop exactly as retail (push ebp; push edi; lea ecx,[esp+3Ch]; call). A one-row TU compiles callees first (all heuristic); making Main inline/vtable-reached sends ALL callees down the budget
path (probe pr/p1.cpp, 443 budget decisions), unlike retail. Retail's state needs the unity TU (Ego_r: all scripts in register_scripts.obj).
Options for the owner: (1) accept the Codex pointer-to-member call device for the final Return (current v1, 59 hunks) as an Inline-budget
device; (2) a lane build mode that compiles this row with retail's compile order (TU-composition fact, like 0x00E20600); (3) park.
Even with (1)/(2) one pen unit (~220 tuples of caller growth) is missing at the end (FXBarrier / normal-end first ~CScriptThing sc 73 v 74).

## Second build fact: caller growth (B24A5 diagnostic, inl/g250.txt, q_g250.txt)
v7 = v1 minus the post-cellar `inline_depth(0)` region (pointer-to-member Return kept so the frame stays 0xCC). Trace v7 (inl/v7f.txt):
without the pragma the post-cellar exit's ~vector scores 73 (retail CALLS it: needs 74) and the three ~CScriptThing score 77 (called,
= retail): the pragma is a stand-in for the same missing pen unit as the final-T FXBarrier (sc 73 v 74). Diagnostic object: v7 + FORCE
operator[] down the budget path + caller growth [0x10882B58] raised once by BUMP at Main's first Return decision:
- BUMP 250 (diffed) / 200 (graded by length only): DIFFER(5333v5321) with 0 slot hunks (FXBarrier 0x28 / Forcefield1 0x1C now = retail: the slot swap was the same fact),
  every exit = retail; the only residue is the last candle loop's register choice (retail keeps Game's vtable in ebx and reloads
  [esi+40h]; ours keeps Game in ebx and spills the vtable to [esp+4Ch]), one `xor ebp,ebp` after that loop and one alignment filler --
  plausibly the pointer-to-member call standing in for retail's direct call to an uncompiled Return. BUMP 300: 4981 (too much).
- So the whole residue is two build facts: (1) Main compiled before TRetVoid::Return and vector<CScriptThing>::operator[]; (2) Main's
  caller growth about +170..+290 tuples larger than our TU's at the end (unknown source: IL that leaves no bytes, or TU context).
  Inference (uses f4004's w = statements x 14 + 15 from another row, not measured here): since the budget total must stay <= 35000 at decision 423 (ours 34701), (2) cannot be ~22+ extra statements of Main's own IL at w=14.
