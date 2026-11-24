# Gate 0 of the object-order build mode -- B24A6, 2026-11-24: FAIL (no working form yet)
Scratch only ($S/w22/b24a/gate0/), stock cl 13.10.3077, no gdb hook, no patched c2, no caller-growth bump, no forcing device.

What was attempted: one TU of the LANDED retail object-mates of 0x00E93EB0 in retail address order (rebuild/src/compiled/00/e9,
0x00E93D50 .. 0x00E96E10: the 37 landed CV_HauntedHouseScript-family rows incl. TRetVoid::Return's row 0x00E95380, list in
gate0/files.txt) with 0x00E93EB0 v1 in its slot, the Codex pointer-to-member Return REMOVED (gate0/v1_noptm.cpp: the final check is
`if (IsActiveThreadTerminating()) return _Fail_Val.Return();`; v1's inline_depth(0) region left as it was).

Result: the TU cannot be built from landed sources, so the observation and both controls could not run.
- The mates are not TU-composable. 20 of the 37 landed mates define their OWN local model of NScript::CV_HauntedHouseScript (and several
  of CScriptDataBase / CCharString / CScriptThing / CSpawnedFuncBase / CScriptProcessBase); v1 uses the shared engine headers. The first
  three-file merge (0x00E93D50 ctor + 0x00E93D80 Init + v1) already stops: C2011 'NScript::CV_HauntedHouseScript' class redefinition,
  C2011 'CScriptDataBase', C2118 negative subscript (layout static_assert of the shared header against the local model), C1903
  (gate0/q_try1.txt). Merging would mean rewriting landed rows onto the shared headers, which the gate forbids ("report the conflict
  instead of merging").
- TRetVoid::Return (0x00E95380) has no usable landed body: its row E2_E2_00e95380.cpp is a stand-in class `C00e95380` (wrong name, not
  TRetVoid), so even a rebased TU would not hold retail's callee. vector<CScriptThing>::operator[] has no landed row at all.
- Flags / defines: no conflicting `// Flags:` (one row says "none"); FABLE_RETAIL_* define sets differ per row (0..19 each) but were not the
  blocker.
- Reference numbers, same oracle and grader (q.py): v1 with the pointer-to-member removed, alone = DIFFER(5145v5321) (frame `sub esp,0C8h`
  v retail 0CCh), i.e. the final Return is inlined away as B24A5 described.

Plain statement: the approved build mode has no working form yet. Prerequisite for a real Gate 0: the object-mates (at least the
CV_HauntedHouseScript family and TRetVoid::Return 0x00E95380) re-based onto the shared headers with their real names, then rerun this
gate unchanged (observe Return / operator[] calls at 0x00E9520A / 0x00E9524E; controls: permuted order, wrong body).
