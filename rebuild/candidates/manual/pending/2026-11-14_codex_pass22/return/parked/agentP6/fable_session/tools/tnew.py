# tnew.py: constant sym creation (hash insert 0x1077e238): value + return-address chain
P = "$S/w22/agentP6F/tools/sched/"
exec(open(P + "common.py").read())
TARGET = cfg.get("target", "FindBestStartVertex")
def on_ins():
    if not state["fn"] or TARGET not in state["fn"]: return
    sp = reg("esp"); st = [u32(sp + 4 * i) for i in range(40)]
    out["events"].append(["ins", u32(reg("ebp") + 0x18), [x for x in st if 0x10700000 <= x < 0x10880000][:10]])
Hook(0x1077e238, on_ins)
run()
