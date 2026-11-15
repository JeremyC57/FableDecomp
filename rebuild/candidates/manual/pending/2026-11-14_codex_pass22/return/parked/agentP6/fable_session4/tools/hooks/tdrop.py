# tdrop.py: at the candidate free 0x10784d3b called from 0x107891e4 (ret 0x107891e9): dump candidate fields and return chain
P = "/tmp/claude-0/-home-user-FableDecomp/5221bbec-fb18-575f-8649-a32d96bb1c2d/scratchpad/w22/agentP6F/tools/sched/"
exec(open(P + "common.py").read())
TARGET = cfg.get("target", "FindBestStartVertex")
def on_free():
    if not state["fn"] or TARGET not in state["fn"]: return
    sp = reg("esp")
    if u32(sp) != 0x107891e9: return
    c = reg("ebx"); st = [u32(sp + 4 * i) for i in range(60)]
    out["events"].append(["drop", u32(c + 0x1c), [u32(c + 4 * i) for i in range(16)], [x for x in st if 0x10700000 <= x < 0x10880000][:8]])
Hook(0x10784d3b, on_free)
run()
