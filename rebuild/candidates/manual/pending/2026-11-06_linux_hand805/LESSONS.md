# Chunk 805 -- lessons from the authoring agent (fold into GOTCHAS at landing)

- (chunk 805, 0x00D41D00) Some per-site temporaries have no named object in the obvious spelling. When retail's frame is
  larger by 12 bytes per site and the x87 code spills one product (`fstp [tmp.Y]` ... `fld [tmp.Y]`) while forwarding the
  other, write a named per-site `C3DVector step(D.X*s, D.Y*s, 0.0f);`. Member-wise construction keeps `s` alive past the Y
  product (`fld; fmul st(2)` and a later `fstp st(0)`). `D*s` (operator*) gives the frame but `fxch; fmul mem`; inline
  `(D*s).X` temporaries give neither.
- (chunk 805, 0x00D41D00) x87 compare shapes in VC7.1: `fcomp; fnstsw; test ah,5; jp skip` is `if (x < c)`;
  `test ah,0x41; je to-then` is `if (x > c) {...} else {...}`; `==` is `fld c; fucompp; test ah,0x44; jp`.
- (chunk 805, 0x00D41D00) `xor ebx,ebx; push ebx; push edi; mov [i],ebx` before a call that takes false: the `i=0;` statement comes
  BEFORE the call in source; after it, VC pushes an immediate 0 and zeroes ebx later.
- (chunk 805, 0x00D2D930) `CScriptThing x; if (r==1) x=Get("A"); else if ... else x=Get("D");` where retail expands
  `PImp=other.PImp` (add eax,4; call 0x00704580) at every site and tail-merges them: use a declared-only
  `CCountedPointer<CScriptThing>::operator=` specialization and a `__forceinline CScriptThing::operator=`. Plain inline (the
  FireHeart 0x00D35090 form) expands only the first, most frequent site. IMPLICIT_ASSIGNMENT with the ShareData body visible
  expands none.
- (chunk 805, 0x00EA36B0) A plain-inline `CTimer::CTimer() : TimerIndex(pDSTGame->RegisterTimer()) {}` is still called out of
  line (0x00CD4450), but its visible body lets VC keep TimerIndex in a callee-saved register across calls (retail ebp). With the
  ctor declared-only, every GetTimer / SetTimer reloads it.
- (chunk 805, 0x00D2D930) /d2inlS is a cliff, not a cap: S14 equals the default and S16 already flips unrelated inlines
  (5180v5094). No value reproduces retail's long-Main MovieBase dtor expansions.

Review at landing:
- Every row took more than 4 grades (attempt logs in ATTEMPTS.log and STATUS.tsv notes).
- The chunk table names 0x00D2D930 as CPermanentBarrier::Main; it is CEndGameJackOfBlades::Main (NAME_FIXES.tsv).
- Manifest label 0x00EA3490 'CV_BeardyBaldyScript::WatchForQuestFinished' -> CV_LostTraderScript::WatchForQuestFailure (LABELS.tsv).
- reloc_bindings.tsv binds the compiler literal 0x0122DEDC (__real@00000000) to nine global names (CGSI_Rumble_Zero,
  g_defaultHealth, g_RampZero, ...). Those landed rows spell a float literal as a named global; worth a one-name review.
- differ/00ec9d30.cpp already uses the PDB FadeOutAndKillEntity signature that chunk 804's NAME_FIXES asks the landed callers
  to adopt.
- 0x00704580 is bound under three spellings in reloc_bindings.tsv (CCountedPointer<CScriptThing> by 0x00D36620, <CScriptThingImp>
  by 0x00CF8970, non-template by 0x00CFD6C0). FableWin.pdb has only the CScriptThing one. Respell the other two (NAME_FIXES.tsv).
