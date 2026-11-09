"""fhost.py <src.cpp> <out.json> [flags...]: compile under the c2 copy with fhook.py; dump events."""
import sys, os, json
sys.path.insert(0, "/home/user/FableDecomp/tools/decomp_pipeline/linux_lane/lane_tools/c2probe")
import c2dbg
src, outp = sys.argv[1], sys.argv[2]; flags = sys.argv[3:]
cfg = {"func": os.environ.get("FUNC", "Action@CManager")}
here = os.path.dirname(os.path.abspath(__file__))
tr, log, run = c2dbg.probe(src, os.path.join(here, os.environ.get("HOOK", "fhook.py")), cfg, flags=flags, timeout=1500, obj_out=os.environ.get("OBJ_OUT"))
if tr is None or tr.get("error"):
    print("ERROR", log[-3000:] if log else "", (tr or {}).get("error")); sys.exit(1)
json.dump(tr, open(outp, "w"))
print("events", len(tr.get("events", [])), "rc", tr.get("cl_rc"))
