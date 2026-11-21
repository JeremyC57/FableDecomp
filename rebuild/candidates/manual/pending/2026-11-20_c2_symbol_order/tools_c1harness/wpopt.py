# gdb script: watch option byte(s) cfg["watch"]; on write print strings pointed by regs and stack words (which option set it)
exec(open("/tmp/claude-0/-home-user-FableDecomp/923afea2-3bc0-53db-a7ad-a8bbc78e03e2/scratchpad/w26/p25R/tools/c1/common.py").read())
class WP(gdb.Breakpoint):
    def __init__(self, a):
        super().__init__(f"*(unsigned int*){a:#x}", gdb.BP_WATCHPOINT, gdb.WP_WRITE, internal=True); self.a = a
    def stop(self):
        try:
            rs = {r: reg(r) for r in ("eax", "ebx", "ecx", "edx", "esi", "edi", "ebp")}
            st = struct.unpack("<40I", rd(reg("esp"), 160))
            strs = {}
            for k, v in list(rs.items()) + [("s%d" % i, v) for i, v in enumerate(st)]:
                s = cstr(v, 60) if 0x10000 < v < 0x7fffffff else None
                if s and len(s) >= 2 and all(32 <= ord(c) < 127 for c in s): strs[k] = s
            out["events"].append(["wp", self.a, reg("eip"), u32(self.a), strs])
        except Exception: out["events"].append(["err", traceback.format_exc()])
        return False
for a in cfg["watch"]: WP(a)
run()
