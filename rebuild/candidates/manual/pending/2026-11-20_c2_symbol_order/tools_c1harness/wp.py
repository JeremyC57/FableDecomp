# gdb script: hardware write watchpoints on cfg["watch"] (list of addresses); log pc, new dword, c1 return chain.
exec(open("/tmp/claude-0/-home-user-FableDecomp/923afea2-3bc0-53db-a7ad-a8bbc78e03e2/scratchpad/w26/p25R/tools/c1/common.py").read())
LIM = cfg.get("limit", 300)
class WP(gdb.Breakpoint):
    def __init__(self, a):
        super().__init__(f"*(unsigned int*){a:#x}", gdb.BP_WATCHPOINT, gdb.WP_WRITE, internal=True); self.a = a
    def stop(self):
        try:
            if len(out["events"]) < LIM:
                out["events"].append(["wp", self.a, reg("eip"), u32(self.a), stack(32),
                                      {r: reg(r) for r in ("eax", "ebx", "ecx", "edx", "esi", "edi", "ebp")}])
        except Exception: out["events"].append(["err", traceback.format_exc()])
        return False
for a in cfg["watch"]: WP(a)
run()
