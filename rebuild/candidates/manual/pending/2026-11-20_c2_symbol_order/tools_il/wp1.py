# gdb script: hardware write watchpoints on cfg["watch"] addresses (dword). Logs pc, new value, stack return candidates.
import sys
exec(open("/tmp/claude-0/-home-user-FableDecomp/923afea2-3bc0-53db-a7ad-a8bbc78e03e2/scratchpad/w25/hand3c3/tools/sched/common.py").read())
LIM = cfg.get("limit", 400)
class W(gdb.Breakpoint):
    def __init__(self, addr):
        super().__init__(f"*(unsigned int*){addr:#x}", type=gdb.BP_WATCHPOINT, wp_class=gdb.WP_WRITE, internal=True); self.addr = addr
    def stop(self):
        try:
            pc = reg("eip"); esp = reg("esp"); ebp = reg("ebp")
            st = struct.unpack("<96I", rd(esp, 96 * 4))
            ras = [[i * 4, v] for i, v in enumerate(st) if 0x10701000 <= v < 0x10868000]
            out["events"].append(["w", self.addr, pc, u32(self.addr), state["fn"], ras[:40], [reg(r) for r in ("eax","ecx","edx","ebx","esi","edi","ebp")]])
            if len(out["events"]) > LIM: self.enabled = False
        except Exception: out["events"].append(["err", traceback.format_exc()[-800:]])
        return False
for a in cfg["watch"]: W(a)
run()
