# 0x00D0D910 CGate2Guard1::Main -- B24B3 notes
- Baseline regrade: DIFFER(4672v4923) 36 hunks /d2inlS15 (oracle extent wrong: the body ends with `ret` at 0x00D0EB8F (4736 b) followed by the 4-entry jump table
  at 0x00D0EB90 used by `jmp [eax*4+0xD0EB90]` at 0x00D0DBCC = 4752 b, the extent on record; the current cut adds ~171 bytes beyond it), raw 143 lines.
- Same quest TU and the SAME inline conflict as row 0x00D0C410 (see ../00d0c410/NOTES.md and ESCALATE.md): draft's own line 2 says it --
  original timer body + S15 expands all ~CTimer sites; S10..14 keep retail's timer split but call the movies. Measured in 00d0c410 with
  the c2 trace: c1(~MovieBase) 19 v c1(~CTimer) 14, retail = MovieBase always + ~CTimer frequency-split; no threshold fits. The ~CTimer
  text is proven by FableWin hashes ($S/w22/b24b/SHARED_FORM.md), so the lever is the ~MovieBase cost (B24A lane). Not worked further (B24B3).
