# tu.py: at the constant counting walk (0x1077dd6d, edx = tuple) dump every tuple that has a constant operand: opcode, flag byte +9,
# constants in list +0x2c ("A", not counted) and list +0x28 ("B", counted).
P = "$S/w22/agentP6F/tools/sched/"
exec(open(P + "common.py").read())
TARGET = cfg.get("target", "FindBestStartVertex")
NC = {}
def mn(op):
    if op not in NC:
        try: NC[op] = cstr(u32(0x10874398 + 4 * (op + 4))) if op < 0x240 else hex(op)
        except Exception: NC[op] = hex(op)
    return NC[op]
def consts(lst):
    r = []; n = 0
    while lst and n < 12:
        raw = rd(lst, 0x20); k = raw[8]
        if k == 1:
            sym = struct.unpack_from("<I", raw, 0x18)[0]
            try:
                if rd(sym + 4, 1)[0] == 0xd: r.append(u32(u32(sym + 0x28) + 0x18))
                else: r.append("s")
            except Exception: r.append("?")
        else: r.append("k%d" % k)
        lst = struct.unpack_from("<I", raw, 0)[0]; n += 1
    return r
def on_t():
    if not state["fn"] or TARGET not in state["fn"]: return
    t = reg("edx"); raw = rd(t, 0x30)
    op = struct.unpack_from("<I", raw, 4)[0]
    a = b = []
    if raw[9] & 1:
        try: a = consts(struct.unpack_from("<I", raw, 0x2c)[0]); b = consts(struct.unpack_from("<I", raw, 0x28)[0])
        except Exception: a = b = ["err"]
    out["events"].append(["t", mn(op & 0xffff), raw[9], a, b])
Hook(0x1077dd6d, on_t)
run()
