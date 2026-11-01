# Chunk 653 -- lessons from the authoring agent (fold into GOTCHAS at landing)

- The relocation mask hides a swapped float constant. 0x00712340's prior matched every byte except the `__real@` addresses,
  but its switch scaled the wrong cases (1.75 and 0.75 swapped, the unscaled case misplaced). Check every `__real@`
  reloc of a candidate against the retail .rdata value at the same offset (`$S/w22/agent653/cc.py <addr> <obj> <leaf>`).
  The correct mapping then matched, because VC7.1 cross-jumps the shared `fxch; fadd [esi]; fstp [esi]` tail only for the
  real case order.
- A same-TU noinline body of the const `CVectorMap<ETCInterfaceType,...>::LowerBound` (0x004365B0) tells VC7.1 that the
  lookup writes no memory. A CScriptThing's PImp.Data then stays in a callee-saved register across the TC lookup
  (0x0089F300). It had no effect at 0x00984F60 or 0x005EBFA0.
- OpenAL-style wrappers as inline header members (lazy resolve through
  `UseALGetProcAddress ? GetProcAddress(name) : ::GetProcAddress(Module, name)`) reproduce retail's split between inlined
  early wrappers and later called ones, and the ebp-cached import, purely through the per-function inline budget
  (0x00C4DC40).
- For a while loop whose normal exit returns, put the post-loop work inside the loop under the break condition, ending in
  `return`. Retail's rotated loop exit then goes straight to the scope-end destructors (0x00E37E40, 65 diff lines from 80).
- VC7.1 ignores source block order for a no-exit branch: if/else, a negated condition and `goto` gave byte-identical output
  at 0x00E4AA00. The layout follows the destructor inline-site choice, not the source.
- Constant-dividend divisions without /Op: `d * -1.0f / s` keeps a plain fdiv (no reciprocal CSE). `-1.0f / s` in any
  temporary becomes a reciprocal reused by the other divisions (0x00C5A9D0).

Review at landing:
- 0x00712340 had a semantic error in its pending prior (rebuild/candidates/manual/pending/2026-10-23_linux_hand313/differ/00712340.cpp)
  that the grader could not see. Sweep other pending/landed rows with switch- or branch-dependent float constants using
  the `__real@` check.
- 0x00E37E40's pending prior (2026-10-23_linux_hand319) negated the second wait loop.
- Rows that needed more than 4 grades: 0x0089F300 (10 probes) and every parked row (12-20 probes each, logged in probe/<addr>.log).
- Local base models next to existing engine headers: 0x00984F60 (CAIStateGroup_OpinionReactionRoot.h), 0x00666310
  (CThingCreatureBase.h, bitfields flattened), and 0x00E37E40/0x00E4AA00 (CScriptThing/CGameScriptInterface VPAD models).
- Landed local models spell CTCPhysicsBase slot 70 as GetFacing/GetForward. The Ego_r name is GetFacingVector
  (0x006B1410); see NAME_FIXES.tsv.
- `D:\Documents\FableTLC\rebuild\build\landverify` empty directories appeared in the agent directory from a tool default
  path (removed).
