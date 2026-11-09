P = "/tmp/claude-0/-home-user-FableDecomp/5221bbec-fb18-575f-8649-a32d96bb1c2d/scratchpad/w22/agent890/work/s4/re/probe/"
src = open(P + "t11.py").read().replace("run()", "")
src = src.replace("Hook(0x107b4f22, on_cycle)\nHook(0x107b5244, on_pick)\nHook(0x107b4e25, on_region)", "")
exec(src)
PAT = cfg.get("pat", ["fsubr", "fsub", "fstp", "lea", "push", "mov", "call"])
watch = {"on": False}
def has_pat(ops):
    for i in range(len(ops) - len(PAT) + 1):
        if ops[i:i + len(PAT)] == PAT: return True
    return False
def on_dag():
    if not state["fn"] or TARGET not in state["fn"]: 
        hc.enabled = False; hp.enabled = False; return
    hdr = u32(0x108a8054); first = u32(hdr); last = u32(hdr + 4)
    nd = first; res = []; n = 0
    while nd and n < 200:
        res.append(nd)
        if nd == last: break
        nd = u32(nd); n += 1
    ops = []
    for nd in res:
        t = u32(nd + 0x1c); ops.append(mn(u32(t + 4) & 0xffff) if t else None)
    on = has_pat(ops)
    out["events"].append(["region", len(res), ops if on else ops[:3] + ['..'] + ops[-3:]])
    if on:
        full = []
        for nd in res:
            d = node(nd); e = u32(nd + 0xc); succ = []
            while e:
                raw = rd(e, 0x18)
                succ.append([u16(u32(e + 0xc) + 0x36), struct.unpack_from("<H", raw, 0x14)[0], raw[0x16], raw[0x17]])
                e = u32(e)
            d["succ"] = succ; full.append(d)
        out["events"].append(["dag", full])
    hc.enabled = on; hp.enabled = on
Hook(0x107b4e16, on_dag)
hc = Hook(0x107b4f22, on_cycle); hp = Hook(0x107b5244, on_pick)
hc.enabled = False; hp.enabled = False
run()
