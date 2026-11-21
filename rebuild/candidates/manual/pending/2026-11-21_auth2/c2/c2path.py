# c2 gdb script: per inline decision (0x1078CF54 path): callee, its +0x69 flags (bit 2 = compiled, 0x10 = own pass), record present,
# the path taken (heur / budget) and the result; budget-path inputs (running total [0x10882B5C] v 0x88B8, callee size +0x6D, limit arg);
# compile starts (0x107AFF88) and pipe events (function whose decisions follow... note: decisions are logged BEFORE their pipe).
exec(open("/home/user/FableDecomp/rebuild/candidates/manual/pending/2026-11-19_authoring_handoff/hand3c/tools/sched/common.py").read())
def nm(s):
    try: return cstr(u32(s + 4))
    except Exception: return None
cur = {}
def on_dec():
    global cur
    b = reg("ebx")
    cur = {"n": nm(b), "fl": u32(b + 0x69), "rec": u32(b + 0x99) != 0}
    out["events"].append(["d", cur])
def on_bud():
    cur["path"] = "budget"; cur["tot"] = u32(0x10882B5C)
    cur["sz"] = struct.unpack("<H", rd(reg("ebx") + 0x6D, 2))[0]; cur["lim"] = u32(reg("esp") + 0x20)
def on_bacc(): cur["r"] = 1
def on_brej():
    if cur.get("path") == "budget" and "r" not in cur: cur["r"] = 0
def on_heur(): cur["path"] = "heur"
def on_hres(): cur["r"] = reg("eax")
def on_start(): out["events"].append(["start", nm(reg("eax"))])
def on_pipe():
    try: out["events"].append(["pipe", cstr(u32(u32(reg("eax")) + 4))])
    except Exception: pass
for a, f in ((0x1078CF8D, on_dec), (0x107284A3, on_bud), (0x1078D5F5, on_bacc), (0x107284CC, on_brej), (0x1078CFD5, on_heur),
             (0x1078CFDA, on_hres), (0x107AFF88, on_start), (0x1077C08E, on_pipe)):
    Hook(a, f)
run()
