P = "$S/w22/agentP6F/tools/sched/"
exec(open(P + "common.py").read())
TARGET = cfg.get("target", "FindBestStartVertex")
def desc(c):
    sym = u32(c); d = {"cand": c, "id": u32(c + 0x1c)}
    if sym:
        raw = rd(sym, 0x30); d["kind"] = raw[4]; d["sym"] = sym
        if raw[4] == 13:
            node = struct.unpack_from("<I", raw, 0x28)[0]
            if node: d["val"] = u32(node + 0x18)
        else:
            st = struct.unpack_from("<I", raw, 0)[0]
            try: d["name"] = cstr(u32(st + 4)) if st else None
            except Exception: pass
    return d
def on_new():
    if not state["fn"] or TARGET not in state["fn"]: return
    d = desc(reg("eax")); d["site"] = u32(reg("esp")); out["events"].append(["new", d])
def on_free():
    if not state["fn"] or TARGET not in state["fn"]: return
    d = desc(reg("ebx")); d["ret"] = u32(reg("esp") + 8)
    # caller chain: return address of the free routine
    out["events"].append(["free", d])
Hook(0x1077C1DE, on_new)
Hook(0x10784DA6, on_free)
run()
