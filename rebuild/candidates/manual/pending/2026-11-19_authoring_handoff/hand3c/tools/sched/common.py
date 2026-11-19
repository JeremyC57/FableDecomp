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
def u16(a): return struct.unpack("<H", rd(a, 2))[0]
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
        except Exception: out["events"].append(["err", traceback.format_exc()[-800:]])
        return False
state = {"fn": None}
def on_pipe():
    fn = reg("eax")
    try: state["fn"] = cstr(u32(u32(fn) + 4))
    except Exception: state["fn"] = "?"
    out["events"].append(["pipe", state["fn"]])
Hook(0x1077C08E, on_pipe)
def run():
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
