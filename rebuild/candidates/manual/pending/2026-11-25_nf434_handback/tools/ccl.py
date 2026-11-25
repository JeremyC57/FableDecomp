#!/usr/bin/env python3
"""ccl.py <root> <src.cpp> <out.obj> [extra cl flags] | ccl.py --link <root> <out.exe> <obj>...: compile (/O2 /Oy /W3 + extras) or link
with verify_and_land's env (FABLE_ROOT=<root>, FABLE_HDR_OVERLAY honoured); prints the tool output."""
import sys, os
args = sys.argv[1:]; link = args[0] == "--link"
if link: args = args[1:]
os.environ["FABLE_ROOT"] = args[0]
sys.path.insert(0, args[0] + "/tools/decomp_pipeline"); import verify_and_land as V
e = V.env()
if link:
    r = V.run_tool(V.VC / "bin" / "link.exe", ["/nologo", "/out:" + args[1]] + args[2:], e)
else:
    r = V.cl(["/nologo", "/c"] + V.BASE_FLAGS + args[3:] + ["/Fo" + args[2], args[1]], e)
print(r.stdout[-4000:], r.stderr[-2000:], "rc", r.returncode)
