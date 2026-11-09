"""cnt.py -- gdb side (run by c2dbg.probe): log the c2 'too big' tuple count (0x1077be49) and each function name (PIPE)."""
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
head = [0]
def on_entry():
    head[0] = reg("eax")
def on_cnt():
    esp = reg("esp")
    ev = ["count", u32(esp + 0xc), u32(esp + 0x10)]
    if u32(esp + 0xc) > cfg.get("walkmin", 5000):
        hist = {}; ops = {}
        t = head[0]; n = 0
        while t and n < 200000:
            b = rd(t, 0x18); n += 1
            if b[9] & 1:
                ln = struct.unpack_from("<H", b, 0x14)[0]
                op = struct.unpack_from("<I", b, 4)[0]
                hist[ln] = hist.get(ln, 0) + 1
                k = "%d:%x" % (ln, op); ops[k] = ops.get(k, 0) + 1
            t = struct.unpack_from("<I", b, 0)[0]
        ev.append(hist); ev.append(ops)
    out["events"].append(ev)
def on_pipe():
    fn = reg("eax")
    try: name = cstr(u32(u32(fn) + 4))
    except Exception: name = "?"
    out["events"].append(["pipe", name])
if cfg.get("nolimit"):
    inf.write_memory(0x1077be4d, b"\xff\xff\xff\x7f")
def on_gate():
    A = reg("ebx"); arr = u32(0x108a7898); names = []
    if cfg.get("pack") and A < 5000:
        for i in range(A):
            try:
                node = u32(arr + 4 * i); sym = u32(node); kind = rd(node + 4, 1)[0]
                p = u32(sym + 4) if sym else 0
                names.append([cstr(p) if p else None, kind, u32(sym + 0x1c) if sym else 0, rd(node, 0x48).hex(), rd(sym, 0x60).hex() if sym else ""])
            except Exception as ex: names.append(["err", 0, 0])
    out["events"].append(["gate", A, reg("esi"), u32(0x108a9220), names])
Hook(0x10785605, on_gate)
def on_live():
    arg = u32(reg("esp") + 4)
    b = u32(u32(arg + 8)); n = 0; nt = 0
    while b and n < 100000:
        n += 1
        b = u32(b)
    out["events"].append(["blocks", n])
Hook(0x107903e3, on_live)
def on_ovf():
    out["events"].append(["ovf", reg("ebp"), u32(0x10882bc0)])
Hook(0x10765b41, on_ovf)
def on_pack():
    a = reg("eax"); nm = None
    try:
        p = u32(a + 4); nm = cstr(p) if p else None
    except Exception: pass
    out["events"].append(["pack", a, reg("ecx"), nm, u32(a + 0x1c)])
def on_list():
    edi = reg("edi"); name = cstr(reg("eax"))
    out["events"].append(["sym", name, edi, struct.unpack("<i", rd(edi + 0x18, 4))[0], u32(edi + 0x1c)])
if cfg.get("pack"):
    Hook(0x10790eb5, on_pack); Hook(0x1082bf47, on_list)
for a, f in ((0x1077bdd8, on_entry), (0x1077be49, on_cnt), (0x1077C08E, on_pipe)):
    Hook(a, f)
for extra in cfg.get("extra", []):
    pass
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
