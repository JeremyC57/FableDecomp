# tsym.py: root-candidate walk over the sym arena (0x1077fcf1: ebp = sym, ebx = chunk). Log every sym visited: address, chunk, kind, name, has-record
P = "/tmp/claude-0/-home-user-FableDecomp/5221bbec-fb18-575f-8649-a32d96bb1c2d/scratchpad/w22/agentP6F/tools/sched/"
exec(open(P + "common.py").read())
TARGET = cfg.get("target", "FindBestStartVertex")
def on_s():
    if not state["fn"] or TARGET not in state["fn"]: return
    s = reg("ebp"); raw = rd(s, 0x60); kind = raw[4]
    name = None
    try:
        if kind == 13:
            node = struct.unpack_from("<I", raw, 0x28)[0]; name = "c" + hex(u32(node + 0x18))
        else:
            st = struct.unpack_from("<I", raw, 0)[0]; name = cstr(u32(st + 4)) if st else None
    except Exception: pass
    rec = struct.unpack_from("<I", raw, 0x34)[0]; webs = 0
    try:
        if rec:
            w = u32(rec + 0xc)
            while w and webs < 20: webs += 1; w = u32(w)
    except Exception: pass
    out["events"].append(["sym", s, reg("ebx"), kind, name, webs, raw[5], struct.unpack_from("<H", raw, 0x10)[0]])
Hook(0x1077fcf1, on_s)
run()
