"""cnt_host.py <src.cpp> [flags...] : compile under the c2 copy and print (count, name) per function."""
import sys, os, json
sys.path.insert(0, "/home/user/FableDecomp/tools/decomp_pipeline/linux_lane/lane_tools/c2probe")
import c2dbg
src = sys.argv[1]; flags = sys.argv[2:]
cfg = {"pack": bool(os.environ.get("PACK")), "nolimit": bool(os.environ.get("NOLIMIT")), "walkmin": int(os.environ.get("WALKMIN", "5000"))}
tr, log, run = c2dbg.probe(src, os.path.join(os.path.dirname(os.path.abspath(__file__)), "cnt.py"), cfg, flags=flags, timeout=1500, obj_out=os.environ.get("OBJ_OUT"))
if tr is None or tr.get("error"):
    print("ERROR", log[-2000:] if log else "", (tr or {}).get("error")); sys.exit(1)
ev = tr["events"]; pend = []
json.dump(ev, open(os.environ.get("CNT_JSON", "/dev/null"), "w"))
for e in ev:
    if e[0] == "count": pend.append(e)
    elif e[0] == "pipe":
        for c in pend: print("count %6d  aux %5d  -> %s" % (c[1], c[2], e[1])) if c[1] > 50 else None
        if not pend: print("   (no count) -> %s" % e[1])
        pend = []
    elif e[0] in ("pack", "sym"): pass
    elif e[0] == "ovf": print("   ovf %d limit %d" % (e[1], e[2]))
    elif e[0] == "blocks": print("   blocks %d" % e[1])
    elif e[0] == "gate": print("   gate A=%d B=%d g=%d" % (e[1], e[2], e[3]))
    else: print(e)
for c in pend: print("count %6d aux %5d -> (no pipe)" % (c[1], c[2]))
