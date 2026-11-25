#!/usr/bin/env python3
"""vtlink.py <addr> <row.obj>: relink the test object vt.py left in $S/wine/vt/<addr> with the row object and print the linker errors."""
import os,sys
sys.path.insert(0,'tools/decomp_pipeline'); import verify_and_land as V
from pathlib import Path
a=sys.argv[1]; w=Path(os.environ['S'])/'wine'/'vt'/a; e=V.env()
lk=V.run_tool(V.VC/"bin"/"link.exe",["/nologo",f"/out:{w/(a+'.x.exe')}",sys.argv[2],str(w/(a+'.t.obj'))],e)
print('\n'.join(l for l in (lk.stdout or '').splitlines() if 'error' in l or 'warning' in l)[:3000])
