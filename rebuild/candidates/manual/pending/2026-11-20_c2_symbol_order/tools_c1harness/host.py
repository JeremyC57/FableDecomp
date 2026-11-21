"""host.py <gdb-script.py> <src.cpp> <out.json> [json-config] : run a c1dbg probe (cfg 'flags' = extra cl flags)."""
import sys, os, json
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import c1dbg
script, src, outp = sys.argv[1:4]
cfg = json.loads(sys.argv[4]) if len(sys.argv) > 4 else {}
flags = cfg.pop('flags', [])
tr, log, run = c1dbg.probe(src, os.path.abspath(script), cfg, flags=flags, timeout=1500, keep=bool(os.environ.get('KEEP')))
if tr is None: print("ERROR", log[-3000:]); sys.exit(1)
json.dump(tr, open(outp, "w")); print("ok", tr.get("error"), len(tr.get("events", [])), "rc", tr.get("cl_rc"))
