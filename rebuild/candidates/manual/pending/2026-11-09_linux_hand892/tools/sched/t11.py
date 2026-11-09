P = "/tmp/claude-0/-home-user-FableDecomp/5221bbec-fb18-575f-8649-a32d96bb1c2d/scratchpad/w22/agent892/tools/sched/"
exec(open(P + "common.py").read())
TARGET = cfg.get("target", "k1")
NC = {}
def mn(op):
    if op not in NC:
        try: NC[op] = cstr(u32(0x10874398 + 4 * (op + 4))) if op < 0x240 else hex(op)
        except Exception: NC[op] = hex(op)
    return NC[op]
def node(nd):
    t = u32(nd + 0x1c)
    raw = rd(nd, 0x40)
    return dict(n=nd, op=mn(u32(t + 4) & 0xffff) if t else None, t=t, prio=struct.unpack_from("<I", raw, 0x28)[0],
                prio2=struct.unpack_from("<I", raw, 0x2c)[0], early=struct.unpack_from("<I", raw, 0x30)[0],
                depth=struct.unpack_from("<H", raw, 0x34)[0], idx=struct.unpack_from("<H", raw, 0x36)[0],
                c20=struct.unpack_from("<H", raw, 0x20)[0], c22=struct.unpack_from("<H", raw, 0x22)[0], c24=struct.unpack_from("<H", raw, 0x24)[0],
                fl=raw[0x3c], unit=struct.unpack_from("<H", raw, 0x3a)[0])
def on_cycle():
    if not state["fn"] or TARGET not in state["fn"]: return
    cyc = reg("esi"); r = []; nd = u32(0x108a8044); n = 0
    while nd and nd != u32(0x108a8024) and n < 60:
        r.append(node(nd)); nd = u32(nd + 0x10); n += 1
    busy = [struct.unpack("<i", rd(0x108a7d90 + 12 * k, 4))[0] for k in range(8)]
    out["events"].append(["cyc", cyc, busy, r])
def on_pick():
    if not state["fn"] or TARGET not in state["fn"]: return
    out["events"].append(["pick", u32(reg("esp") + 0x10 + 0), node(reg("edi")), reg("ebp"), reg("ebx")])
def on_region():
    if not state["fn"] or TARGET not in state["fn"]: return
    out["events"].append(["region"])
def on_slot_ret():
    pass
Hook(0x107b4f22, on_cycle)
Hook(0x107b5244, on_pick)
Hook(0x107b4e25, on_region)
run()
