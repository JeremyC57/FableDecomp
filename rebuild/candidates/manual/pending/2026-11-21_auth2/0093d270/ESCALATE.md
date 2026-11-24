# ESCALATE 0x0093D270 CAIStateGroup_MeleeCombatNearbyBase::IsRunnable (295 B) -- AUTH2, 2026-11-21
Best source unchanged: S/w22/auth1/0093d270/e1.cpp DIFFER(305v295), residue 6 lines (the inline float getter's return-value home
`mov [esp+10h],eax` after GetCombatManager; retail keeps the manager in esi and loads +0x28 after GetPItem). AUTH1's packet
(S/w22/auth1/0093d270/ESCALATE.md) holds the diff, the 11 grades and the minis.

## The advisor's two steps (AUTH2 LOG.md)
1. Ego_r: GetProximityForNavigation appears in no Ego_r dump in the repo (names, signatures, publics, obj functions, fp); Ego_r.pdb's
   type stream is not on this machine. Ego_d (a VC10 build, S_COMPILE3 16.00.40219): public method, LF_MFUNCTION returns float by value;
   member private; no friend. => no PDB source for `const float&`.
2. c2 budget path: forcing the callee onto the budget path in the m2 mini (c2 hook S/w22/auth2/c2/c2force.py, decision logged
   `budget`, accepted) gives byte-identical code: the home store stays. With the callee already compiled, the budget decision path leaves the bytes identical; the compile ORDER (callee after caller) is untested: `start ?GetP` still precedes f1/f2 in both traces.

## What is left
(a) the TLC header differed from FableWin/Ego_d at this inline (only the Ego_r type stream, on Windows, can show it: request its
    LF_MFUNCTION for ?GetProximityForNavigation@CCombatManager and the member's access);
(c) a global build flag we lack (AUTH1's sweep of ~15 flags: none).
Flip (1) of the 2026-11-21 ruling (a landed byte-matching row with this shape showing the deferred load) is not searched yet: it needs a
scan of landed rows that pass a by-value inline float getter of a call-returned object to a call with later call-containing arguments.

## Next steps (no ruling needed)
- Windows request (coordinator): Ego_r / FableWin type-stream LF_MFUNCTION of ?GetProximityForNavigation@CCombatManager and the member's access.
- Linux: the flip-(1) scan the 2026-11-21 ruling already allows; and a compile-order inversion probe (callee compiled after caller) if a natural cycle can be built.
## Original question (kept)
Is the next step the Windows-side Ego_r type-stream read (FableWin/Ego_r LF_MFUNCTION of the getter), or should I run
the flip-(1) scan of landed rows (bounded: grep rebuild/src/compiled for `.Get...()` float getters inside multi-argument call arguments,
then compare retail bytes for the home store)?
