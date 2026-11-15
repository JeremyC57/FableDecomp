# trst.py: count rotation-pointer resets at pseudo-tuple 0x2d3 (0x107dff94) and list the pseudo-op tuples reaching 0x107dff87
P = "$S/w22/agentP6F/tools/sched/"
exec(open(P + "common.py").read())
TARGET = cfg.get("target", "Transfer@CCreatureDef")
def on_chk():
    if not state["fn"] or TARGET not in state["fn"]: return
    out["events"].append(["chk", u32(reg("esi") + 4)])
def on_rst():
    if not state["fn"] or TARGET not in state["fn"]: return
    out["events"].append(["rst"])
def on_fn():
    if not state["fn"] or TARGET not in state["fn"]: return
    out["events"].append(["fnreset"])
Hook(0x107dff87, on_chk)
Hook(0x107dff94, on_rst)
Hook(0x10775844, on_fn)
run()
