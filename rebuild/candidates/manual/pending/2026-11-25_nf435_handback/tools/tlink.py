#!/usr/bin/env python3
"""tlink.py <workdir> <addr>: link <addr>.obj + <addr>.t.obj the way vland does and print the linker output, then run the exe."""
import sys, os, subprocess
sys.path.insert(0, '/home/user/FableDecomp/tools/decomp_pipeline')
os.chdir('/home/user/FableDecomp')
import verify_and_land as V
d, a = sys.argv[1:3]; e = V.env()
exe = f'{d}/{a}.lt.exe'
r = V.run_tool(V.VC / 'bin' / 'link.exe', ['/nologo', f'/out:{exe}', f'{d}/{a}.obj', f'{d}/{a}.t.obj'], e)
print((r.stdout or '') + (r.stderr or ''))
if os.path.exists(exe):
    rn = subprocess.run(([V.WINE] if V.WINE else []) + [exe], capture_output=True, text=True, errors='replace', env=e); print('RUN', rn.returncode, rn.stdout[-300:])
