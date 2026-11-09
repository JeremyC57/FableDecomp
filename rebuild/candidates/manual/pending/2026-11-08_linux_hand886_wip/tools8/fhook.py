"""fhook.py -- gdb side: log frame allocations (0x1076ed94 symbol allocator, 0x1077479d raw allocator) per function."""
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
    for size in (256, 64, 16, 4):
        try: return rd(a, size).split(b"\0")[0].decode("latin1")
        except gdb.MemoryError: continue
ON = [False]
WANT = cfg.get("func", "Action@CManager")
class Hook(gdb.Breakpoint):
    def __init__(self, addr, fn, always=False):
        super().__init__(f"*{addr:#x}", internal=True); self.fn = fn; self.always = always
    def stop(self):
        if not ON[0] and not self.always: return False
        try: self.fn()
        except Exception: out["events"].append(["err", traceback.format_exc()])
        return False
def symname(s):
    try:
        p = u32(s + 4); return cstr(p) if p else None
    except Exception: return "?"
def on_pipe():
    fn = reg("eax")
    try: name = cstr(u32(u32(fn) + 4))
    except Exception: name = "?"
    ON[0] = bool(name and WANT in name)
    out["events"].append(["pipe", name])
def on_symalloc():
    s = reg("ecx"); esp = reg("esp")
    out["events"].append(["sa", u32(esp), s, symname(s), u32(s + 0x1c), u32(s + 0x20), u32(s + 0x37), struct.unpack("<i", rd(s+0x18,4))[0]])
def on_symalloc_done():
    s = reg("esi")
    out["events"].append(["sd", s, struct.unpack("<i", rd(s + 0x18, 4))[0]])
def on_raw():
    esp = reg("esp")
    out["events"].append(["ra", u32(esp), reg("eax"), reg("ecx"), struct.unpack("<i", rd(0x108a9218, 4))[0]])
Hook(0x1077C08E, on_pipe, always=True)
Hook(0x1076ed94, on_symalloc)
Hook(0x1076ee19, on_symalloc_done)
Hook(0x1077479d, on_raw)
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
