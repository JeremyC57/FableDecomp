# gdb script: log every IL tuple the lowering loop (0x10773138, ecx = tuple) visits, with operands (raw) and symbol names.
exec(open("/tmp/claude-0/-home-user-FableDecomp/923afea2-3bc0-53db-a7ad-a8bbc78e03e2/scratchpad/w25/hand3c3/tools/sched/common.py").read())
syms = {}
out["syms"] = syms
def sym(s):
    k = "%x" % s
    if k in syms or not (0x10000 < s < 0x10700000): return
    try:
        raw = rd(s, 0x30); st = struct.unpack_from("<I", raw, 0)[0]; nm = None
        if st and raw[4] != 13:
            try: nm = cstr(u32(st + 4))
            except Exception: nm = None
        syms[k] = [raw[4], struct.unpack_from("<H", raw, 0x10)[0], nm, raw.hex()]
    except Exception: syms[k] = None
def oplist(o):
    res = []; k = 0
    while o and k < 12:
        try: raw = rd(o, 0x30)
        except Exception: break
        res.append([o, raw.hex()]); sym(struct.unpack_from("<I", raw, 0x18)[0]); o = struct.unpack_from("<I", raw, 0)[0]; k += 1
    return res
def on_tup():
    t = reg("ecx")
    raw = rd(t, 0x30)
    a, b = struct.unpack_from("<II", raw, 0x28)
    opc = struct.unpack_from("<I", raw, 4)[0]
    if opc in (0x2c4, 0x2c3, 0x2c5, 0x2c8, 0x2c6): out["events"].append(["t", t, raw.hex(), [], []])
    else: out["events"].append(["t", t, raw.hex(), oplist(a), oplist(b)])
Hook(cfg.get("hook", 0x10773138), on_tup)
run()
