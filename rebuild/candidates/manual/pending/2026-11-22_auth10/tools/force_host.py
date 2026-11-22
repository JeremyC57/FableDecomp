"""force_host.py <src.cpp> <out.json> <force.json> [flags...]: compile under c2 with inl_force.py; OBJ_OUT env keeps the obj."""
import sys, os, json
sys.path.insert(0, "/home/user/FableDecomp/tools/decomp_pipeline/linux_lane/lane_tools/c2probe")
import c2dbg
src, outp, fj = sys.argv[1:4]; flags = sys.argv[4:]
tr, log, run = c2dbg.probe(src, os.path.join(os.path.dirname(os.path.abspath(__file__)), "inl_force.py"), json.load(open(fj)) if fj.endswith(".cfg.json") else {"force": json.load(open(fj))}, flags=flags, timeout=1500, obj_out=os.environ.get("OBJ_OUT"))
if tr is None or tr.get("error"):
    print("ERROR", log[-3000:] if log else "", (tr or {}).get("error")); sys.exit(1)
json.dump(tr["events"], open(outp, "w")); print("events", len(tr["events"]))
