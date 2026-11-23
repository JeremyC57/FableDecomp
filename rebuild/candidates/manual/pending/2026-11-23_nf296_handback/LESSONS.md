# NF296 lessons (2026-11-23)
- **Every binder you leave needs a `left` line before the table lines are written.** rec.py first deleted the LongToWideString and
  SetNavigation* stand-in lines because their witnesses were the held NF295 rows (0x00646010, 0x0097EC00) that the spec did not list;
  corrected by hand to "no change" / CONDITIONAL rewitness.
- **Owner self rows often define a made-up name, too** (0x00A67770, 0x00CB7D60, 0x006925A0, an access slip at 0x0077CCA0): the link of
  binders + owners shows them as the PDB name staying unresolved; read the owner object's defined symbols before calling a link clean.
- **The donor FableWin.exe answers "does this caller call that function"** in seconds (tools/dcalls.py forward, rcalls.py reverse).
  For tiny targets (4..17 bytes) it is the only cheap guard against an ICF fold; where the donor caller calls a wrapper the retail
  build inlined (CVolume ctor, ~CThreadedFile -> Close, ~CShaderDataBank), follow the chain one level.
