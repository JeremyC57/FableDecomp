"""inl.py -- gdb side: log every plain-inline decision (0x1078d1aa) with callee name, size, penalty, savings, cost, freq, score, result."""
import json, os, struct, sys, traceback
sys.path.insert(0, "/home/user/FableDecomp/tools/decomp_pipeline/linux_lane/lane_tools/c2probe")
import gdb
import c2dbg
cfg = json.load(open(os.environ["C2PRIO_CONFIG"]))
out = {"events": [], "error": None}
def fin():
    json.dump(out, open(cfg["out"], "w"))
try:
    _g, inf, cfg = c2dbg.gdb_setup()
except Exception:
    out["error"] = traceback.format_exc(); fin(); raise SystemExit
def rd(a, n): return bytes(inf.read_memory(a, n))
def u32(a): return struct.unpack("<I", rd(a, 4))[0]
def reg(n): return int(gdb.selected_frame().read_register(n)) & 0xFFFFFFFF
def cstr(a):
    if not a: return None
    for size in (256, 64, 16, 4):
        try: return rd(a, size).split(b"\0")[0].decode("latin1")
        except gdb.MemoryError: continue
class Hook(gdb.Breakpoint):
    def __init__(self, addr, fn):
        super().__init__(f"*{addr:#x}", internal=True); self.fn = fn
    def stop(self):
        try: self.fn()
        except Exception: out["events"].append(["err", traceback.format_exc()])
        return False
cur = {}
def on_pre():          # 0x1078cfd5: call decision; ebx = callee sym
    global cur
    ebx = reg("ebx")
    try: name = cstr(u32(ebx + 4))
    except Exception: name = "?"
    esp = reg("esp")
    arg1 = reg("esi"); site = reg("edi")
    cur = {"n": name, "sz": u32(u32(arg1 + 4) + 0x81), "a": u32(arg1 + 0x1c), "g": u32(0x10882b58),
           "flo": u32(site + 0x10), "fhi": u32(site + 0x14), "raw": rd(site, 0x30).hex(),
           "T": u32(0x108819a4), "S": u32(0x108819a8), "k": u32(0x10881994), "d50": u32(0x10881950)}
def on_pen():          # 0x1078d1fb: ebx = penalty
    cur["pen"] = reg("ebx")
def on_sav():          # 0x1078d209: eax -> savings struct
    eax = reg("eax"); cur["sav"] = list(struct.unpack("<7i", rd(eax, 28)))
def on_cost():         # 0x1078d248: esi = cost before weighted savings
    cur["c0"] = struct.unpack("<i", struct.pack("<I", reg("esi")))[0]
def on_cost2():        # 0x1078d290: esi = cost after weighted savings
    cur["c1"] = struct.unpack("<i", struct.pack("<I", reg("esi")))[0]
def on_score():        # 0x1078d354: eax = score
    cur["sc"] = reg("eax")
def on_c22c():       # 0x1078d22c: esi = cost after the callee-flag-8 adjustment
    cur["c22c"] = struct.unpack("<i", struct.pack("<I", reg("esi")))[0]
def on_fin():        # 0x107537ed: final regime-1 compare: esi cost, ecx = S
    cur["cf"] = struct.unpack("<i", struct.pack("<I", reg("esi")))[0]; cur["S"] = reg("ecx")
    cur["fl"] = u32(u32(reg("esp") + 0x10) + 0x69) if False else None
def on_res():          # 0x1078cfda: eax = result
    cur["r"] = reg("eax"); out["events"].append(["d", cur])
def on_pipe():
    fn = reg("eax")
    try: name = cstr(u32(u32(fn) + 4))
    except Exception: name = "?"
    out["events"].append(["pipe", name])
for a, f in ((0x1078cfd5, on_pre), (0x1078d1fb, on_pen), (0x1078d209, on_sav), (0x1078d248, on_cost),
             (0x1078d290, on_cost2), (0x1078d354, on_score), (0x1078cfda, on_res), (0x1078d22c, on_c22c), (0x107537ed, on_fin), (0x1077C08E, on_pipe)):
    Hook(a, f)
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
