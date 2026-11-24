# 0x00D0D910 CGate2Guard1::Main -- B24B3 notes
- Baseline regrade: DIFFER(4672v4923) 36 hunks /d2inlS15 (oracle extent wrong: the body ends with `ret` at 0x00D0EB8F (4736 b) followed by the 4-entry jump table
  at 0x00D0EB90 used by `jmp [eax*4+0xD0EB90]` at 0x00D0DBCC = 4752 b, the extent on record; the current cut adds ~171 bytes beyond it), raw 143 lines.
- Same quest TU and the SAME inline conflict as row 0x00D0C410 (see ../00d0c410/NOTES.md and ESCALATE.md): draft's own line 2 says it --
  original timer body + S15 expands all ~CTimer sites; S10..14 keep retail's timer split but call the movies. Measured in 00d0c410 with
  the c2 trace: c1(~MovieBase) 19 v c1(~CTimer) 14, retail = MovieBase always + ~CTimer frequency-split; no threshold fits. The ~CTimer
  text is proven by FableWin hashes ($S/w22/b24b/SHARED_FORM.md), so the lever is the ~MovieBase cost (B24A lane). Not worked further (B24B3).
## B24B4 (2026-11-24): regrade on the corrected oracle (4752): DIFFER(4672v4752) 36 hunks /d2inlS15 (r0.cpp = differ/ unchanged)
- Retail exit map (dis_all): ~CTimer EXPANDED at exactly two blocks: 0x00D0E89D (loop-bottom NewScriptFrame + terminate check, the rotated
  `for(;!false;NewScriptFrame()){ if(T) return;` head; the pre-loop check enters it at 0x00D0E8A1 with edi = TimerIndex) and 0x00D0E96F
  (shared by the first `if(T) return` of BOTH Gate2Open branches, 0x00D0DA57 / 0x00D0E61C). Every other exit, in-movie ones included,
  joins 0x00D0EB73 (called ~CTimer 0x00CD4470 + ~STB). ~MovieBase expanded at every in-movie exit. Draft uses `#pragma auto_inline(off)` round
  ~CTimer (diagnostic, not landable). Same MovieBase-always / ~CTimer-split conflict as 0x00D0C410 (B24A recipe fails: in-movie cold exits).
## B24B6 (2026-11-24): visible-body lever checked on the shared MovieBase cost (probe in ../00dc93f0/v1.cpp): a VISIBLE noinline
CP<Movie> dtor specialization leaves implicit ~MovieBase at sz 25 / c1 19 (same as declared-only), so it cannot change this row's
MovieBase-v-~CTimer conflict. Not worked further by B24B6 (context).
## B24B7 (2026-11-24): full hunk map (fg_b7.log, --lines 400), r0 /d2inlS15 unchanged DIFFER(4672v4752) 36 hunks
- Hunk 1 (0x00D0DA0F..DA39): retail keeps TimerIndex (call [eax+0x15C] result) in edi because its EXPANDED ~CTimer at the pre-loop
  exit 0x00D0E8A1 uses it (`push edi; call [eax+0x160]`); ours calls ~CTimer there, so edi becomes the const 0 (`xor edi,edi`,
  `mov [esp+54h],edi`). Hunks at 0x00D0E89D..0x00D0EB63: the two missing hot ~CTimer blocks plus eax/edx flips after them.
  Every hunk traces to the MovieBase-always / ~CTimer-split conflict; no independent residue found to work.
- B24B7 a1 test (advisor): b7/a1.cpp = r0 without the auto_inline(off) round ~CTimer, STOCK flags (0x00D0C410's a1 form):
  DIFFER(4544v4752) 117 hunks (b7/a1.log). Hunk 1 PERSISTS (`mov edi,0` at 0x00D0DA41 v retail `mov edi,eax` TimerIndex) and new slot
  shifts appear early (0x00D0D927.., MovieBase/STB locals). So the claim "every hunk is the CTimer conflict" is NOT proven: under a1 the
  edi role still differs; whether a1 expands ~CTimer at retail's two hot blocks was not read (count `call [reg+0x160]` in the q listing
  next: retail has exactly 2). Open for the next agent.
## B24B8 (2026-11-24): PROVEN conflict-only (ESCALATE.md). Lessons that apply (top of the row's learning):
- (lane) a c2 decision-override compile (gdb at 0x1078CFDA, `set $eax`) is a cheap exact what-if: it tells whether a residue is ONLY
  inline decisions. Kit: b8/inl/inl_force*.py + OBJ_OUT, b8/objgrade.py.
- ~CTimer c1 14 / implicit ~MovieBase c1 19 (sz 25), pen 0 (g <= 2675 < 9000) in this row: no S/T flag splits them.
- Counts: retail ~CTimer expanded 2 (`call [reg+0x160]`) + called 1 (0x00CD4470 at the common tail 0x00D0EB73); MovieBase expanded 9.
  ours r0 (S15+auto_inline(off)) 0+1 / 9; a1 (stock) 1+1 / called 13; f1 (stock + MOVIEBASE_FORCEINLINE) 2+1 / 9 (+vptr stores).
- RESULT (decision set proven, which side's cost causes it is NOT): stock flags + all 21 ~MovieBase decisions forced = RELOCATION_MATCH;
  S15 + 31 cold ~CTimer decisions forced = RELOCATION_MATCH. Counterexample in the same object: 0x00DC93F0 retail calls ~MovieBase.
  No independent residue exists. differ/ = pragma-free stock form (4544v4752, 117 hunks); r0 kept as b8/differ_prev_r0_autoinline.cpp.
- Tried, no effect: forceinline GROBase dtor (g1), non-inline ~CTimer under S15 (n1). Row waits for the MovieBase-text answer.
## B24B9 (2026-11-24) -- FableWin read (debug build), no decision moves
- FableWin Main 0x036979E0 (listing ../00dc93f0/b9/fw_036979e0.txt): no asserts. Main loop opens with the VC6 for-scope prolog
  (0x03697BB3 `xor ecx,ecx; je; jmp 0x03699C8E`) and has NO condition test: FableWin text = `if (false) {} else for (;; Game.NewScriptFrame())`.
  b9/m1.cpp (that spelling) = same bytes as differ/ (117 hunks, stock); b9/m0.cpp (`for (;;NSF)` without the prolog) = 4720 b, frequencies x2.
  So m1 is the evidenced loop spelling (keep it); the `!false` condition of differ/ is not in FableWin.
- /Od call sequence (fwsize obj, b9/_fwsize; ../00dc93f0/b9/objcalls.py) v FableWin: identical direct-call counts (38 Return / 38 terminate
  checks / 21 ~MovieBase / 36 ~CTimer ...): no missing statement, no assert, no local-static. FableWin text does not explain the 21
  ~MovieBase decisions: they sit at f .0001..0166 (sc 5575+), so no frequency or 1-point cost change can expand them in regime 0
  (g <= 3000, pen 0). FableWin destructor texts = ours (../SHARED_FORM.md B24B9). Row state unchanged (decision-set proof B24B8).
- B24B9 calls + conditional-jump alignment (../00dc93f0/b9/cjalign.py, /Od obj of b9/m1.cpp v FableWin): 473 = 473 events, 97 = 97 jcc, no difference: Main's CFG is FableWin's.
- B24B9 swap check: differ/00d0d910_pre_b9.cpp v b9/m1.cpp (= new differ/) compiled at stock: leaf bytes RAW-EQUAL (4544, b9/old.obj v
  b9/new.obj). The loop spelling is inferred from FableWin (later VC10 build): structure evidence, not proof of retail text.
