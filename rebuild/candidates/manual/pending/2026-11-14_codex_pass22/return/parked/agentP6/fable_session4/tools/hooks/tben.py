# tben.py: at 0x10782aa7 (eax = candidate being tested for drop: [eax+0x40] <= 0 drops) dump id, +0x40 and other fields
P = "/tmp/claude-0/-home-user-FableDecomp/5221bbec-fb18-575f-8649-a32d96bb1c2d/scratchpad/w22/agentP6F/tools/sched/"
exec(open(P + "common.py").read())
TARGET = cfg.get("target", "FindBestStartVertex")
def nm(c):
    try:
        sym = u32(c)
        raw = rd(sym, 0x30)
        if raw[4] == 13:
            node = struct.unpack_from("<I", raw, 0x28)[0]
            return hex(u32(node + 0x18))
        st = struct.unpack_from("<I", raw, 0)[0]
        return cstr(u32(st + 4)) if st else "k%d" % raw[4]
    except Exception: return "?"
def on_t():
    if not state["fn"] or TARGET not in state["fn"]: return
    c = reg("eax")
    f = [u32(c + 4 * i) for i in range(18)]
    out["events"].append(["ben", f[7], nm(c), struct.unpack("<i", rd(c + 0x40, 4))[0], struct.unpack("<i", rd(c + 0xc, 4))[0], [hex(x) for x in f]])
Hook(0x10782aa7, on_t)
run()
