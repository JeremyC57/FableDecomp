# c2 gdb script: who sets +0x69 bit 0x20 (sites 0x107c0409 / 0x107c0879 / others) with c2 return chains; plus the compile list order at the driver.
exec(open("/tmp/claude-0/-home-user-FableDecomp/923afea2-3bc0-53db-a7ad-a8bbc78e03e2/scratchpad/w25/hand3c3/tools/sched/common.py").read())
SITES = {0x107c0409: "eax", 0x107c0879: "ebx", 0x10709c7a: "esi", 0x107dc7cf: "esi", 0x107ff67c: "edi", 0x1084c526: "eax", 0x107aff88: "eax"}
def chain():
    st = struct.unpack("<48I", rd(reg("esp"), 192)); return [v for v in st if 0x10701000 <= v < 0x10868000][:10]
def mk(a, sr):
    def f():
        s = reg(sr)
        try: nm = cstr(u32(s + 4))
        except Exception: nm = None
        ev = ["s", a, nm, chain()]
        if a == 0x107aff88:   # list order from here
            lst = []; p = s; k = 0
            while p and k < 200:
                try: lst.append([cstr(u32(p + 4)), u32(p + 0x69)]); p = u32(p + 0x91)
                except Exception: break
                k += 1
            ev.append(lst)
        out["events"].append(ev)
    return f
for a, sr in SITES.items(): Hook(a, mk(a, sr))
run()
