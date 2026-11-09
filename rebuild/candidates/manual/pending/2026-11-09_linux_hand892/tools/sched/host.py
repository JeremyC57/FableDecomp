"""host.py <gdb-script.py> <src.cpp> <out.json> [json-config] : run a c2dbg probe with an arbitrary gdb script."""
import sys, os, json
sys.path.insert(0, "/home/user/FableDecomp/tools/decomp_pipeline/linux_lane/lane_tools/c2probe")
import c2dbg
script, src, outp = sys.argv[1:4]
cfg = json.loads(sys.argv[4]) if len(sys.argv) > 4 else {}
flags = cfg.pop('flags', [])
tr, log, run = c2dbg.probe(src, os.path.abspath(script), cfg, flags=flags, timeout=1500)
if tr is None:
    print("ERROR", log[-3000:]); sys.exit(1)
json.dump(tr, open(outp, "w"))
print("ok", tr.get("error"), len(tr.get("events", [])))
