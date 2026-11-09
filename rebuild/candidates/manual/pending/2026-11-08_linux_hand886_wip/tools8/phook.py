"""phook.py -- gdb side: c2 frame packer of one function: the sorted node list at packer entry (0x10790ad6), every
weight update of the list insert 0x10788642 (node edx, increment ebx, caller), and every node's offset at exit."""
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
def s32(a): return struct.unpack("<i", rd(a, 4))[0]
def reg(n): return int(gdb.selected_frame().read_register(n)) & 0xFFFFFFFF
def cstr(a):
    if not a: return None
    for size in (256, 64, 16, 4):
        try: return rd(a, size).split(b"\0")[0].decode("latin1")
        except gdb.MemoryError: continue
ON = [False]
WANT = cfg.get("func", "Action@CManager")
TRACEW = cfg.get("tracew", True)
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
nodeids = {}
def nid(nd):
    if nd not in nodeids: nodeids[nd] = len(nodeids)
    return nodeids[nd]
def node_info(nd):
    b = rd(nd, 0x48); sym = struct.unpack_from("<I", b, 0)[0]
    return [nid(nd), symname(sym) if sym else None, b[4], b[5], b[6], struct.unpack_from("<i", b, 0x20)[0],
            struct.unpack_from("<i", b, 0x38)[0], struct.unpack_from("<i", b, 0x3c)[0], sym,
            u32(sym + 0x1c) if sym else 0, s32(sym + 0x18) if sym else 0, u32(sym + 0x20) if sym else 0,
            b.hex(), rd(sym, 0x64).hex() if sym else ""]
def on_ins():
    nd = reg("edx")
    if TRACEW:
        out["events"].append(["w", nid(nd), reg("ebx"), u32(reg("esp") + 0), symname(u32(nd)) if u32(nd) else None, u32(nd + 0x20)])
def walk_list():
    res = []; nd = u32(0x108a78a4); n = 0
    while nd and n < 100000:
        res.append(node_info(nd)); nd = u32(nd + 0x30); n += 1
    return res
def bitset(addr):
    if not addr: return None
    bits, chunk, n = [], u32(addr), 0
    while chunk and n < 4096:
        base, nxt, word = struct.unpack("<III", rd(chunk, 12))
        bits += [base + i for i in range(32) if word >> i & 1]
        chunk, n = nxt, n + 1
    return bits
def on_pack():
    A = u32(0x108a78a0); nodes = u32(0x108a7898); lv = u32(0x108a7894); inter = []
    for i in range(A):
        nd = u32(nodes + 4 * i)
        try: inter.append([nid(nd), bitset(u32(lv + 4 * i))])
        except Exception: inter.append([nid(nd), None])
    out["events"].append(["pack", A, u32(0x108a789c), walk_list(), inter])
def dump_slots():
    cnt = u32(0x108a788c); arr = u32(0x108a7890); slots = []
    for i in range(cnt):
        b = rd(arr + 0x1c * i, 0x1c); w = struct.unpack("<7i", b)
        slots.append([i, bitset(w[0] & 0xffffffff), w[2], w[3], w[4], w[5], w[6]])
    return slots
def on_sort():
    out["events"].append(["presort", u32(0x108a788c), u32(0x108a789c), reg("ebp"), dump_slots()])
def on_packend():
    cnt = u32(0x108a788c); arr = u32(0x108a7890); slots = []
    for i in range(cnt):
        b = rd(arr + 0x1c * i, 0x1c); w = struct.unpack("<7i", b)
        slots.append([i, bitset(w[0] & 0xffffffff), w[2], w[3], w[4], w[5], w[6]])
    nodes = u32(0x108a7898); idx = []
    for i in range(u32(0x108a78a0)):
        nd = u32(nodes + 4 * i); idx.append(nid(nd))
    out["events"].append(["packend", cnt, u32(0x108a789c), walk_list(), slots, idx])
def on_slot_new():  # 0x10790f2a: eax = new offset, slot = (esi+ebp) region
    out["events"].append(["slot", s32(0x108a9218), reg("eax"), (reg("esi")) // 0x1c])
Hook(0x1077C08E, on_pipe, always=True)
Hook(0x10788642, on_ins)
Hook(0x10790ad6, on_pack)
Hook(0x10790f31, on_packend)
Hook(0x107ae0b0, on_sort)
Hook(0x10790f2a, on_slot_new)
def on_item():
    out["events"].append(["item", nid(u32(reg("esp") + 0x10))])
def on_newslot():
    out["events"].append(["newslot", nid(reg("eax")), u32(0x108a788c)])
def on_join():
    out["events"].append(["join", nid(u32(reg("esp") + 0x10)), reg("edi")])
if cfg.get("order", True):
    Hook(0x10790cd6, on_item); Hook(0x10790cba, on_newslot); Hook(0x107349c0, on_join)
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
