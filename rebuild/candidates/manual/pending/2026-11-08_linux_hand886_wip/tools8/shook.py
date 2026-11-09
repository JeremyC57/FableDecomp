"""shook.py -- gdb side, light: the sorted colouring list (top 40, class 0 run 1) and the first 8 register choices of one
function. ~1 compile time (no per-tuple hooks). Use: HOOK=shook.py t8/fr/run.sh <src> <tag>; t8/fr/sp.py <tag>."""
import json, os, struct, sys, traceback
sys.path.insert(0, "/home/user/FableDecomp/tools/decomp_pipeline/linux_lane/lane_tools/c2probe")
import gdb
import c2dbg
cfg = json.load(open(os.environ["C2PRIO_CONFIG"]))
out = {"events": [], "error": None}
def fin(): json.dump(out, open(cfg["out"], "w"))
try:
    _g, inf, cfg = c2dbg.gdb_setup()
except Exception:
    out["error"] = traceback.format_exc(); fin(); raise SystemExit
def rd(a, n): return bytes(inf.read_memory(a, n))
def u32(a): return struct.unpack("<I", rd(a, 4))[0]
def reg(n): return int(gdb.selected_frame().read_register(n)) & 0xFFFFFFFF
def cstr(a):
    if not a: return None
    for size in (128, 32, 8):
        try: return rd(a, size).split(b"\0")[0].decode("latin1")
        except gdb.MemoryError: continue
ON = [False]; WANT = cfg.get("func", "Action@CManager"); NCH = [0]; NSORT = [0]
REGDESC = 0x108A97A4
def regnum(p):
    return (p - REGDESC) // 0x60 if p and REGDESC <= p < REGDESC + 0x60 * 64 else 0
class Hook(gdb.Breakpoint):
    def __init__(self, addr, fn, always=False):
        super().__init__(f"*{addr:#x}", internal=True); self.fn = fn; self.always = always
    def stop(self):
        if not ON[0] and not self.always: return False
        try: self.fn()
        except Exception: out["events"].append(["err", traceback.format_exc()])
        return False
def on_pipe():
    fn = reg("eax")
    try: name = cstr(u32(u32(fn) + 4))
    except Exception: name = "?"
    ON[0] = bool(name and WANT in name); NCH[0] = 0; NSORT[0] = 0
def cand(c):
    raw = rd(c, 0x48); f = lambda o: struct.unpack_from("<I", raw, o)[0]; s = lambda o: struct.unpack_from("<i", raw, o)[0]
    lf = f(0); kind = name = None
    if lf:
        lr = rd(lf, 0x30); kind = lr[4]; st = struct.unpack_from("<I", lr, 0)[0]
        if st and kind != 13:
            try: name = cstr(u32(st + 4))
            except Exception: pass
        if kind == 13:
            node = struct.unpack_from("<I", lr, 0x28)[0]
            try: name = 'const %#x' % struct.unpack_from("<I", rd(node, 0x20), 0x18)[0]
            except Exception: pass
    return [f(0x1C), s(0xC), f(0x44), f(0x24), kind, name, regnum(f(0x10))]
def on_sorted():
    NSORT[0] += 1
    if NSORT[0] > 1: return
    res = []; h = u32(0x108807B4); n = 0
    while h and n < 40: res.append(cand(h)); h = u32(h + 0x14); n += 1
    out["events"].append(["sorted", reg("ebx"), res])
def on_chosen():
    NCH[0] += 1
    if NCH[0] > 8: return
    c = u32(reg("esp") + 0x20)
    costs = list(struct.unpack("<9i", rd(0x10880680, 36)))
    out["events"].append(["chosen", cand(c), reg("ebx"), costs])
Hook(0x1077C08E, on_pipe, always=True)
Hook(0x10783B6B, on_sorted)
Hook(0x107834A4, on_chosen)
exited = []
gdb.events.exited.connect(exited.append)
try:
    while not exited:
        try: gdb.execute("continue", to_string=True)
        except gdb.error:
            if exited or not inf.threads(): break
            raise
except Exception:
    out["error"] = traceback.format_exc()
fin()
