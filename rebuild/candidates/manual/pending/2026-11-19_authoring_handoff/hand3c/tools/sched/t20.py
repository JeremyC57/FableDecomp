P = "/tmp/claude-0/-home-user-FableDecomp/40effb6c-294b-5111-9a88-ca0c32f095ec/scratchpad/w24/hand2c/tools/sched/"
src = open(P + "t11.py").read().replace("run()", "")
src = src.replace("Hook(0x107b4f22, on_cycle)\nHook(0x107b5244, on_pick)\nHook(0x107b4e25, on_region)", "")
exec(src)
def on_dag():
    if not state["fn"] or TARGET not in state["fn"]:
        hc.enabled = False; hp.enabled = False; return
    hdr = u32(0x108a8054); first = u32(hdr); last = u32(hdr + 4)
    nd = first; res = []; n = 0
    while nd and n < 300:
        res.append(nd)
        if nd == last: break
        nd = u32(nd); n += 1
    full = []
    for nd in res:
        d = node(nd); e = u32(nd + 0xc); succ = []
        while e:
            raw = rd(e, 0x18)
            succ.append([u16(u32(e + 0xc) + 0x36), struct.unpack_from("<H", raw, 0x14)[0], raw[0x16], raw[0x17]])
            e = u32(e)
        d["succ"] = succ; full.append(d)
    out["events"].append(["dag", full])
    hc.enabled = True; hp.enabled = True
Hook(0x107b4e16, on_dag)
hc = Hook(0x107b4f22, on_cycle); hp = Hook(0x107b5244, on_pick)
hc.enabled = False; hp.enabled = False
run()
