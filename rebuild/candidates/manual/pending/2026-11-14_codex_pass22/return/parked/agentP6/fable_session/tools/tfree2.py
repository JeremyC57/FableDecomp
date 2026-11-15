# tfree2.py: candidate frees (entry 0x10784d3b, ebx = candidate: id, kind, name / constant value, caller) and creations (0x1077C1DE)
P = "/tmp/claude-0/-home-user-FableDecomp/5221bbec-fb18-575f-8649-a32d96bb1c2d/scratchpad/w22/agentP6F/tools/sched/"
exec(open(P + "common.py").read())
TARGET = cfg.get("target", "FindBestStartVertex")
def desc(c):
    d = {"id": u32(c + 0x1c)}
    try:
        sym = u32(c)
        if sym:
            raw = rd(sym, 0x30); d["kind"] = raw[4]
            if raw[4] == 13:
                node = struct.unpack_from("<I", raw, 0x28)[0]
                if node: d["val"] = u32(node + 0x18)
            else:
                st = struct.unpack_from("<I", raw, 0)[0]
                d["name"] = cstr(u32(st + 4)) if st else None
    except Exception: pass
    return d
def on_new():
    if not state["fn"] or TARGET not in state["fn"]: return
    d = desc(reg("eax")); d["site"] = u32(reg("esp")); out["events"].append(["new", d])
def on_free():
    if not state["fn"] or TARGET not in state["fn"]: return
    d = desc(reg("ebx")); d["ret"] = u32(reg("esp")); out["events"].append(["free", d])
Hook(0x1077C1DE, on_new)
Hook(0x10784d3b, on_free)
run()
