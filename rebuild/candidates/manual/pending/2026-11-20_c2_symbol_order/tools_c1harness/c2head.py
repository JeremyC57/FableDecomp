# c2 gdb script: who writes the compile-list head [0x10881d68] (pc + chain) and the list at that moment (after the write)
exec(open("/tmp/claude-0/-home-user-FableDecomp/923afea2-3bc0-53db-a7ad-a8bbc78e03e2/scratchpad/w25/hand3c3/tools/sched/common.py").read())
def chain():
    st = struct.unpack("<48I", rd(reg("esp"), 192)); return [v for v in st if 0x10701000 <= v < 0x10868000][:10]
pend = []
def lst():
    p = u32(0x10881d68); r = []; k = 0
    while p and k < 300:
        try: r.append(cstr(u32(p + 4))); p = u32(p + 0x91)
        except Exception: break
        k += 1
    return r
def mk(a, nxt):
    def f(): out["events"].append(["h", a, chain(), lst()])
    return f
for a in (0x1070c987, 0x107c0520, 0x107ff6b0, 0x10812fbd, 0x10813015):
    Hook(a, mk(a, None))
run()
