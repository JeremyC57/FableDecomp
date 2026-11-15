P = "$S/w22/agentP6/tools/sched/"
exec(open(P + "common.py").read())
TARGET = cfg.get("target", "FindBestStartVertex")
def on_cmp():
    if not state["fn"] or TARGET not in state["fn"]: return
    ent = reg("ebp"); sym = u32(ent + 4); rec = reg("ecx")
    val = None
    try:
        node = u32(sym + 0x28); val = u32(node + 0x18)
    except Exception: pass
    cand = u32(rec + 0xc)
    out["events"].append(["cnt", reg("eax") if False else None, val, u32(rec + 0x14), u32(cand + 0x1c) if cand else None])
Hook(0x1077FF77, on_cmp)
run()
