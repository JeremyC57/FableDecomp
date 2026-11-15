# tw.py: watch the use-count field (+0x14) of constant records for the values in cfg["vals"]; log the pc of each write.
P = "$S/w22/agentP6F/tools/sched/"
exec(open(P + "common.py").read())
TARGET = cfg.get("target", "FindBestStartVertex")
VALS = [v & 0xFFFFFFFF for v in cfg.get("vals", [-4, -1, 0x2aaaaaab])]
class W(gdb.Breakpoint):
    def __init__(self, addr, val):
        super().__init__("*(int*)%#x" % addr, gdb.BP_WATCHPOINT, gdb.WP_WRITE, internal=True); self.a = addr; self.v = val
    def stop(self):
        try:
            sp = reg("esp"); st = [u32(sp + 4 * i) for i in range(24)]
            out["events"].append(["w", self.v, u32(self.a), reg("eip"), [x for x in st if 0x10700000 <= x < 0x10880000]])
        except Exception: out["events"].append(["err", traceback.format_exc()[-400:]])
        return False
ws = []
def on_rec():
    if not state["fn"] or TARGET not in state["fn"]: return
    val = u32(reg("ebp") + 0x18)
    out["events"].append(["rec", val, reg("eax")])
    if val in VALS and len(ws) < 4:
        ws.append(W(reg("eax") + 0x14, val))
def on_pipe2():
    # a new function: drop the watchpoints
    for w in ws:
        try: w.delete()
        except Exception: pass
    del ws[:]
Hook(0x1077e223, on_rec)
Hook(0x1077C08E, on_pipe2)
run()
