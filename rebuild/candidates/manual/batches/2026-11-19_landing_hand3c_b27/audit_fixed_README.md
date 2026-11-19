# fixed_b27 (audit of b27, 2026-11-19): 0x00D8A240 NScript::CQ_HerosOldHouseScript::WatchForBooty

One fix over b27: the locals are Ego_d.pdb's. Everything else (flag, overlay header, test) is unchanged from b27.

- Ego_d.pdb HAS a record (egod_locals.py 'NScript::CQ_HerosOldHouseScript::WatchForBooty', debug length 833):
  this, HiddenBooty (CScriptThing 0xBCD4, bprel -28), _Fail_Val (TRetVoid 0xFA51, bprel -13). b27's README/source said
  "no Ego_d.pdb record" (likely looked up without `NScript::`; the short name returns "no complete record"), and the source used `booty` and `TRetVoid()` temporaries.
- Fix: `TRetVoid _Fail_Val;` declared first (highest bprel; same convention as landed 0x00D58490, Ego_d -13 at the top),
  `CScriptThing HiddenBooty = ...`, every terminate exit `return _Fail_Val.Return();` (GOTCHAS 138). Final `return;` kept (v2 with `return _Fail_Val.Return();` there also matches at T57; FableWin.exe was not reachable to count
  TRetVoid::Return calls in 0x037BA020 -> REVIEW).
- Grades (overlay = hdr/): no flag DIFFER(662v610); /d2inlT56 DIFFER(561v610); /d2inlT57..63 RELOCATION_MATCH; /d2inlT64
  DIFFER(655v610). Sweep T0..T70: only 57..63 match. Strict vland (LANDVERIFY_STRICT_HEADERS=1): RELOCATION_MATCH PASS.
- Test (b27's, unchanged; payload test_cpp identical): 00d8a240_TEST PASS (20 targeted, 3000 random) on the /d2inlT57 object.
- Preference 1 on the fixed form, no flag: do{...}while(0) per terminate check DIFFER(639v610); `if(..){return ..;} else {}` DIFFER(662v610).
- The c2 trace numbers in the source header were measured on b27's spelling (booty / TRetVoid() temporaries); the T window is identical on this one.
- Grader: qa.py = copy of $S/tools/q.py reading b27 oracle.tsv (bytes = $S/batch/hand2c_oracle.tsv) with its own work dir; stock q.py not run.
  Overlay control: fixed source with an empty overlay = SRC_FAIL.
- Mutants (mutants.tsv, logs/mutants_audit.log, built with /d2inlT57): 34 graded (+1 LINK_FAIL, no result); 33 KILLED, 1 SURVIVED (final `return;` ->
  `return _Fail_Val.Return();`, equivalent: TRetVoid::Return emits nothing), 1 LINK_FAIL (extra CScriptThing copy: the test
  does not link the copy ctor; not a behaviour result).
- payload.json = b27's with source_cpp replaced by src/00d8a240.cpp; compiler_flags / flags ['/d2inlT57'].
- NAME_FIXES.tsv: none (b27's note).
