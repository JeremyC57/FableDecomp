# tbt.py: candidate creations (0x1077C1DE, eax = new candidate) with the return-address chain; cfg "ids": only these ids
P = "/tmp/claude-0/-home-user-FableDecomp/5221bbec-fb18-575f-8649-a32d96bb1c2d/scratchpad/w22/agentP6F/tools/sched/"
exec(open(P + "common.py").read())
TARGET = cfg.get("target", "FindBestStartVertex")
n = {"k": 0}
def nm(c):
    try:
        sym = u32(c); raw = rd(sym, 0x30)
        if raw[4] == 13: return hex(u32(struct.unpack_from("<I", raw, 0x28)[0] + 0x18))
        st = struct.unpack_from("<I", raw, 0)[0]
        return cstr(u32(st + 4)) if st else "k%d" % raw[4]
    except Exception: return "?"
def on_new():
    if not state["fn"] or TARGET not in state["fn"]: return
    n["k"] += 1
    if n["k"] < cfg.get("from", 83) or n["k"] > cfg.get("to", 130): return
    c = reg("eax"); sp = reg("esp"); st = [u32(sp + 4 * i) for i in range(80)]
    out["events"].append(["new", n["k"], u32(c + 0x1c), nm(c), [hex(x) for x in st if 0x10700000 <= x < 0x10880000][:9]])
Hook(0x1077C1DE, on_new)
run()
