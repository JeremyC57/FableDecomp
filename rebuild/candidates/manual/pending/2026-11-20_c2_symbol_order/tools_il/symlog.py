# gdb script: log every symbol allocation (0x1075a0c5; al = kind) with the symbol number and the return chain.
exec(open("/tmp/claude-0/-home-user-FableDecomp/923afea2-3bc0-53db-a7ad-a8bbc78e03e2/scratchpad/w25/hand3c3/tools/sched/common.py").read())
pend = {}
rets = {}
out["syms"] = []
def on_ret():
    esp = reg("esp"); p = pend.pop(esp - 4, None)
    if p is None: return
    s = reg("eax")
    try: num = u32(s + 0x1c)
    except Exception: num = -1
    nm = None
    try:
        pc = reg("eip")
        if pc == 0x1076f08c: nm = cstr(u32(reg("edi") + 4))
        elif pc == 0x10769fe4:
            b = reg("ebx"); st = u32(b); nm = "field of " + str(cstr(u32(st + 4)) if st else None) + " #%d" % u32(b + 0x1c)
    except Exception: nm = "?"
    out["syms"].append([len(out["events"]), p[0], s, num, p[1], nm])
def on_alloc():
    esp = reg("esp"); kind = reg("eax") & 0xff
    st = struct.unpack("<48I", rd(esp, 48 * 4))
    ra = st[0]
    chain = [v for v in st if 0x10701000 <= v < 0x10868000][:9]
    pend[esp] = (kind, chain)
    if ra not in rets: rets[ra] = Hook(ra, on_ret)
Hook(0x1075a0c5, on_alloc)
run()
