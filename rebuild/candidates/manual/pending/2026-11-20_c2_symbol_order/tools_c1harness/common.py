# gdb-side common helpers for c1dbg scripts (exec'd by each script)
import json, os, struct, sys, traceback
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)) if '__file__' in dir() else "/tmp/claude-0/-home-user-FableDecomp/923afea2-3bc0-53db-a7ad-a8bbc78e03e2/scratchpad/w26/p25R/tools/c1")
sys.path.insert(0, "/tmp/claude-0/-home-user-FableDecomp/923afea2-3bc0-53db-a7ad-a8bbc78e03e2/scratchpad/w26/p25R/tools/c1")
import gdb, c1dbg
out = {"events": [], "error": None}
_g, inf, cfg = c1dbg.gdb_setup()
def fin(): json.dump(out, open(cfg["out"], "w"))
def rd(a, n): return bytes(inf.read_memory(a, n))
def u32(a): return struct.unpack("<I", rd(a, 4))[0]
def reg(n): return int(gdb.selected_frame().read_register(n)) & 0xFFFFFFFF
def cstr(a, n=200):
    try: return rd(a, n).split(b"\0")[0].decode("latin1")
    except Exception: return None
class Hook(gdb.Breakpoint):
    def __init__(self, addr, fn):
        super().__init__(f"*{addr:#x}", internal=True); self.fn = fn
    def stop(self):
        try: return bool(self.fn())
        except Exception: out["events"].append(["err", traceback.format_exc()]); return False
def stack(n=24):
    esp = reg("esp"); st = struct.unpack("<%dI" % n, rd(esp, 4 * n))
    return [v for v in st if 0x10401000 <= v < 0x10560000]
def run():
    ex = []
    gdb.events.exited.connect(ex.append)
    try:
        while not ex:
            try: gdb.execute("continue", to_string=True)
            except gdb.error:
                if ex or not inf.threads(): break
                raise
    except Exception: out["error"] = traceback.format_exc()
    fin()
