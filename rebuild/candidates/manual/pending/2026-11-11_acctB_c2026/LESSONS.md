# Chunk 2026 -- lessons from the authoring agent (fold into GOTCHAS at landing)

- A string member assigned one of two literals is `if (c) s = "A"; else s = "B";`: VC7.1 cross-jumps the two operator= calls into
  `push "A" ; jmp` / `push "B" ; call` (0x0092DAC9..0x0092DAE5); `s = c ? "A" : "B";` selects the pointer in a register first.
- Two wake-up paths after a wait loop that retail reaches straight from different loop exits are a local flag
  (`bool disturbed = true; while (...) { if (!A) { disturbed = false; break; } if (!B || C) break; ... }` then
  `if (disturbed && fast_name != "") {fast} else {normal}`): VC7.1 threads the constant flag through the exits (0x0092DA20).
- The same inline template at two sites can be split by the `&&` frequency alone: IS_THING_ALIVE<CIntelligentPointer<CThing> > is called
  out of line (0x004CC340) under `if (a && b)` (0.25) and expanded in its else arm (0.75) (0x0092DF5E v 0x0092E00C).
- An inline constructor in a class header that retail calls out of line (CCreatureAction_GetInBed 0x0092E300, the COMDAT sitting in the
  caller's TU) needed nothing special: VC7.1 kept it out of line by itself at the PerformActionAndWait argument site.
- `X = Y.GetDefinition()` from a `const T* tc = 0; thing->PeekTC(&tc);` gives retail's `xor reg,reg` before the lookup and the const
  LowerBound 0x004365B0; the copied CDefPointer, dereferenced on every path, releases with no null test (0x0092E113).
- An `if / else` whose else arm retail lays out AFTER the shared code that follows (the then-arm falls into the tail, the else arm jumps
  back into it) can be an `else if (cond) {...} else { Terminate(); return; }` chain: written as `else { if (!cond) { Terminate(); return; } ... }`
  VC7.1 keeps source order (0x0094E7B0: idle/boast tail at 0x0094EB25 before the throwing arm at 0x0094EC63).
- A trailing `if (IsStopping()) Terminate();` merges the conditional release into the natural exit and re-pairs every earlier
  `{ Terminate(); return; }` copy; `if (IsStopping()) { Terminate(); return; }` gives retail's two end releases (0x0094EF36 / 0x0094EF51) and
  the whole exit layout (0x0094E7B0: 1973 -> 1980 bytes, RELOCATION_MATCH).
- Before calling a missing expansion an inline-budget residue, run the c2 inline trace: a same-TU helper whose body retail expands
  (Ego_r CAIStateGroup_TrollBase::GroundPound, no retail copy) scored 431 against the 74 threshold at its site (c1 62, f 0.023) -- no flag
  or loop shape plausibly moves that, so the body is written at the call site (0x0094EAA7).
- Layout probes are cheap and need no grade: compile only and print the order of a few call markers (tmp/lay.sh); matching the marker
  addresses predicted the final RELOCATION_MATCH exactly.

Review at landing:
- 0x0094E7B0 writes CAIStateGroup_TrollBase::GroundPound's body inline (unproven construct, NOTES.md); 5 grades in pass 1.
- 0x0092DA20 also defines MinionSleep::IsInSleepTime: it grades RELOCATION_MATCH against 0x0092C520 (landed under the invented
  CAIStateGroup_GoToBed::IsInSleepTimeWithWakeDelay) -- respell that row (NAME_FIXES.tsv).
- Landed rows with wrong spellings (NAME_FIXES.tsv): 0x00733210 is CTCCombat::ResetComboChainPos, not CWADFile::Close; the TrollBase
  rows 0x0094D680 / 0x0094D9C0 / 0x0094DBC0 / 0x0094DC40 use public / invented-class spellings for protected Ego_r members; the
  GetOutOfBed / UnsheatheWeapons ctor rows define pointer-argument ctors where both PDBs have references.
- vland RELOC_NAME warnings for 0x006BB390 / 0x0091F990 / 0x00733210 are catalog labels to respell (NAME_FIXES.tsv).
