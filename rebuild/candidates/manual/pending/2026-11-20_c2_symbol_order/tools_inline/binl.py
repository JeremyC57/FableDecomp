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
def on_b():            # 0x107284a3: budget path; ebx = callee sym, [esp+0x20] = share
    ebx = reg("ebx")
    try: name = cstr(u32(ebx + 4))
    except Exception: name = "?"
    esp = reg("esp")
    sz = struct.unpack("<H", rd(ebx + 0x6d, 2))[0]
    out["events"].append(["B", name, sz, u32(esp + 0x20), u32(0x10882b5c)])
def on_acc():
    out["events"].append(["A"])
def on_rej():
    out["events"].append(["R"])
def on_h():            # 0x1078cfd5 heuristic call
    out["events"].append(["H"])
def on_pipe():
    fn = reg("eax")
    try: name = cstr(u32(u32(fn) + 4))
    except Exception: name = "?"
    out["events"].append(["pipe", name])
for a, f in ((0x107284a3, on_b), (0x1078d5f5, on_acc), (0x107284cc, on_rej), (0x1078cfd5, on_h), (0x1077C08E, on_pipe)):
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
