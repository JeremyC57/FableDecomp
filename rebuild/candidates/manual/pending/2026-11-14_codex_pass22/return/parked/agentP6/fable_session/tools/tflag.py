P = "$S/w22/agentP6F/tools/sched/"
exec(open(P + "common.py").read())
seen = [0]
def on_fn():
    if seen[0] > 2: return
    seen[0] += 1
    out["events"].append(["flags", u32(0x108a9138), u32(0x108a913c), u32(0x108a9554), u32(0x108a923c)])
Hook(0x10775844, on_fn)
run()
