P = "/tmp/claude-0/-home-user-FableDecomp/5221bbec-fb18-575f-8649-a32d96bb1c2d/scratchpad/w22/agent892/tools/sched/"
src = open(P + "t11.py").read().replace("run()", "")
src = src.replace("Hook(0x107b4f22, on_cycle)\nHook(0x107b5244, on_pick)\nHook(0x107b4e25, on_region)", "")
exec(src)
def on_find():
    # region-end finder entry (0x107b4e2f): eax = first tuple of the next region; walk the raw tuple list
    if not state["fn"] or TARGET not in state["fn"]: return
    t = reg("eax"); res = []; n = 0
    while t and n < 140:
        op = u32(t + 4) & 0xffff; kind = rd(t + 8, 1)[0]
        res.append([mn(op), kind])
        if op == 0x2c9: break
        t = u32(t); n += 1
    out["events"].append(["tuples", res])
Hook(0x107b4e2f, on_find)
run()
