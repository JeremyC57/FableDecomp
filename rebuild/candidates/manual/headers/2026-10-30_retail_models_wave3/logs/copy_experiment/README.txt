Wave 3 def-chain copy-member experiment (LESSONS.md item 1). chain.h / chainH.h include the worktree headers with absolute
paths into $S/mig3/work/x; chain.h needs the temporary `#ifndef FABLE_DEF_IMPLICIT_COPY` guard around the CDefClassBase /
CParentDefClassBase copy declarations (removed after the experiment). Grade with scripts/q.py <addr> <src> <leaf>.
t_<addr>_O2 / _Os: standalone implicit members. p2: PDCB-first TU. fe: implicit CFrontEndDef (0x00432E60). h1/h3: explicit
out-of-line members defined earlier in the TU. h4: explicit derived op= over the implicit chain. i1/i2: explicit inline.
v_*: pragma variants. st_impl*: CSoundThemeDef 0x0045C54E (/Os). os_rows/ + os.log: 41 landed callers rewritten onto
chain.h (39 /Os RELOCATION_MATCH; 0x00434370 / 0x00625350 /O2 DIFFER; 0x0045c2e8 leaf-name WARN, graded the largest).
