"""hostobj.py <gdb-script> <src> <out.json> <obj_out> [json-config]: host.py that also keeps the object."""
import sys, os, json
sys.path.insert(0, "/home/user/FableDecomp/tools/decomp_pipeline/linux_lane/lane_tools/c2probe")
import c2dbg
script, src, outp, objo = sys.argv[1:5]
cfg = json.loads(sys.argv[5]) if len(sys.argv) > 5 else {}
flags = cfg.pop('flags', [])
tr, log, run = c2dbg.probe(src, os.path.abspath(script), cfg, flags=flags, timeout=1500, obj_out=objo)
if tr is None: print("ERROR", log[-3000:]); sys.exit(1)
json.dump(tr, open(outp, "w")); print("ok", tr.get("error"), len(tr.get("events", [])))
