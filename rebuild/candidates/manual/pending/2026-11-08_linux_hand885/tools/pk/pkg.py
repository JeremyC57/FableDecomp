"""pkg.py -- gdb side (run by c2dbg.probe): VC7.1 c2.dll frame packer for every function.
Events: ["pipe", name]; ["list", [[idx,size,weight,kind,flags,name,tnum,node], ...]] (the sorted frame-item list
at 0x10790c96, i.e. the order the packer processes items); ["first", idx, slot] (first-fit join, 0x107349c0);
["place", idx] (0x10791426 second path entry); ["join2", slot] (0x10791ab7); ["new", slot] (0x1079145e);
optional ["iset", idx, [members]] when cfg iset is set (interference set at [0x108a7894 + 4*idx])."""
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
def bits(hdr):
    r = []
    if not hdr: return r
    c = u32(hdr); n = 0
    while c and n < 10000:
        b = rd(c, 12); base, nxt, w = struct.unpack("<III", b)
        for k in range(32):
            if w >> k & 1: r.append(base + k)
        c = nxt; n += 1
    return r
class Hook(gdb.Breakpoint):
    def __init__(self, addr, fn):
        super().__init__(f"*{addr:#x}", internal=True); self.fn = fn
    def stop(self):
        try: self.fn()
        except Exception: out["events"].append(["err", traceback.format_exc()])
        return False
want = cfg.get("func", "")
on = [False]
def on_pipe():
    fn = reg("eax")
    try: name = cstr(u32(u32(fn) + 4))
    except Exception: name = "?"
    on[0] = (want in (name or ""))
    out["events"].append(["pipe", name])
def on_list():
    if not on[0]: return
    nd = u32(0x108a78a4); L = []; n = 0
    while nd and n < 100000:
        b = rd(nd, 0x48)
        sym = struct.unpack_from("<I", b, 0)[0]
        nm = None; tnum = None; sw = None
        if sym:
            try:
                s = rd(sym, 0x30); sw = list(struct.unpack("<12I", s))
                nm = cstr(sw[1]) if sw[1] else None; tnum = sw[10]
            except Exception: pass
        L.append([struct.unpack_from("<I", b, 0x3c)[0], struct.unpack_from("<i", b, 0x20)[0], struct.unpack_from("<i", b, 0x38)[0],
                  b[4], b[6], nm, tnum, b.hex(), sw])
        nd = struct.unpack_from("<I", b, 0x30)[0]; n += 1
    out["events"].append(["list", L])
    if cfg.get("iset"):
        arr = u32(0x108a7894)
        for it in L:
            try: out["events"].append(["iset", it[0], bits(u32(arr + 4 * it[0]))])
            except Exception: pass
cur = [None]
def on_first():
    if not on[0]: return
    item = u32(reg("esp") + 0x10)
    out["events"].append(["first", u32(item + 0x3c), reg("edi")])
def on_place():
    if not on[0]: return
    item = u32(reg("esp") + 4)
    out["events"].append(["place", u32(item + 0x3c)])
def on_join2():
    if not on[0]: return
    out["events"].append(["join2", reg("edi")])
def on_new():
    if not on[0]: return
    out["events"].append(["new", u32(0x108a788c)])
def dump_slots(tag):
    if not on[0]: return
    n = u32(0x108a788c); base = u32(0x108a7890)
    raw = rd(base, 0x1c * n).hex() if n else ''
    out["events"].append([tag, n, u32(0x108a789c), raw])
Hook(0x107ae0b0, lambda: dump_slots("presort")); Hook(0x10785605, lambda: dump_slots("final")); Hook(0x107ae0bf, lambda: dump_slots("postsort"))
Hook(0x1077C08E, on_pipe); Hook(0x10790c96, on_list); Hook(0x107349c0, on_first); Hook(0x10791426, on_place)
Hook(0x10791ab7, on_join2); Hook(0x1079145e, on_new)
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
