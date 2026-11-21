# c2 gdb script (sched/host.py): log every OR into a symbol's +0x69 flags: site, symbol name, value OR'd, flags before.
exec(open("/tmp/claude-0/-home-user-FableDecomp/923afea2-3bc0-53db-a7ad-a8bbc78e03e2/scratchpad/w25/hand3c3/tools/sched/common.py").read())
SITES = {0x107707a2: ("eax", "edx"), 0x107aff88: ("eax", "ebx"), 0x107c0409: ("eax", "ebx"), 0x107d7326: ("eax", "edx"),
         0x10709c7a: ("esi", 0x20), 0x107c0879: ("ebx", 0x20), 0x107dc7cf: ("esi", 0x20), 0x107ff67c: ("edi", 0x20),
         0x1084c526: ("eax", 0x20), 0x10772476: ("eax", 0x10), 0x10708c59: ("ebx", "ecx")}
def mk(a, sr, vr):
    def f():
        s = reg(sr); v = reg(vr) if isinstance(vr, str) else vr
        try: nm = cstr(u32(s + 4))
        except Exception: nm = None
        try: before = u32(s + 0x69)
        except Exception: before = None
        out["events"].append(["or", a, nm, v, before])
    return f
for a, (sr, vr) in SITES.items(): Hook(a, mk(a, sr, vr))
run()
