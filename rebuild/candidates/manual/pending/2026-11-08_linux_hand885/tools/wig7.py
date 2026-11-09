"""wig.py -- gdb side: c2 allocator WHAT-IF. cfg 'force': [[leafname, prio], ...] -- every candidate (and split piece) whose
leaf name equals leafname gets its priority (+0x0c) overwritten when it is inserted into the sorted list (0x10782b26);
cfg 'forcekey': same for the tie key +0x44. Logs every choose/chosen of watched names ('watch') and the forced ones."""
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
    for size in (64, 16, 4):
        try: return rd(a, size).split(b"\0")[0].decode("latin1")
        except gdb.MemoryError: continue
names = {}
def leafname(c):
    sym = u32(c)
    if sym in names: return names[sym]
    nm = None
    try:
        raw = rd(sym, 0x30); st = struct.unpack_from("<I", raw, 0)[0]
        if raw[4] == 13:
            node = struct.unpack_from("<I", raw, 0x28)[0]
            nm = 'const %d' % struct.unpack_from("<i", rd(node, 0x20), 0x18)[0] if node else 'const ?'
        elif st: nm = cstr(u32(st + 4))
    except Exception: pass
    names[sym] = nm; return nm
force = {n: p for n, p in cfg.get("force", [])}
forcekey = {n: p for n, p in cfg.get("forcekey", [])}
watch = set(cfg.get("watch", [])) | set(force) | set(forcekey)
on = [False]
class Hook(gdb.Breakpoint):
    def __init__(self, addr, fn):
        super().__init__(f"*{addr:#x}", internal=True); self.fn = fn
    def stop(self):
        try: self.fn()
        except Exception: out["events"].append(["err", traceback.format_exc()])
        return False
def on_pipe():
    fn = reg("eax")
    try: name = cstr(u32(u32(fn) + 4))
    except Exception: name = "?"
    on[0] = cfg.get("func", "RunCutsceneMacro") in (name or ""); names.clear(); ppass[0] = 0
    for h in bhooks: h.enabled = on[0] and not bdone[0]
def on_insert():
    if not on[0]: return
    c = reg("eax"); nm = leafname(c)
    if nm in force:
        inf.write_memory(c + 0xc, struct.pack("<i", force[nm]))
    if nm in forcekey:
        inf.write_memory(c + 0x44, struct.pack("<I", forcekey[nm]))
def on_chosen():
    if not on[0]: return
    c = u32(reg("esp") + 0x20); nm = leafname(c)
    if nm in watch or cfg.get("all"):
        out["events"].append(["chosen", u32(c + 0x1c), nm, struct.unpack("<i", rd(c + 0xc, 4))[0], reg("ebx"),
                              list(struct.unpack("<9i", rd(0x10880680, 36))), list(curc[1]) if curc[0] == c else None])
blkpass = [0]
def chain(h):
    res = []; seen = set()
    while h and h not in seen and len(res) < 5000:
        seen.add(h); raw = rd(h, 0x20)
        res.append([struct.unpack_from("<I", raw, 0x1C)[0], leafname(h), raw[6], struct.unpack_from("<i", raw, 0x18)[0]])
        h = struct.unpack_from("<I", raw, 0x14)[0]
    return res
tblocks = set(); curblk = [None]; dumped = {}
def on_def():
    if not on[0]: return
    c = reg("esi")
    if leafname(c) in watch: tblocks.add(u32(reg("esp") + 0x14))
def on_use():
    if not on[0]: return
    c = reg("ecx")
    if leafname(c) in watch: tblocks.add(u32(reg("esp") + 0x14))
def on_blkstart():
    curblk[0] = reg("ebx")
def on_blkend():
    if not on[0] or curblk[0] not in tblocks or dumped.get(curblk[0], 0) >= 3: return
    refl = chain(reg("ebx")); live = chain(u32(reg("esp") + 0x10))
    if not any(c[1] in watch for c in refl): return
    dumped[curblk[0]] = dumped.get(curblk[0], 0) + 1
    out["events"].append(["blk", reg("eax"), reg("ebp"), refl, live])
    if cfg.get("blocks_once"):   # first watched block of the first priority pass only: drop the hooks (speed)
        bdone[0] = True
        for h in bhooks: h.enabled = False
ppass = [0]
bhooks = []; bdone = [False]
if cfg.get("blocks"):
    bhooks = [Hook(0x107828EB, on_blkend), Hook(0x1078244D, on_blkstart), Hook(0x1077FAE3, on_def), Hook(0x1077FBF7, on_use)]
    for h in bhooks: h.enabled = False
curc = [None, []]
RNUM = {1: 'eax', 2: 'ecx', 3: 'edx', 4: 'ebx', 6: 'ebp', 7: 'esi', 8: 'edi'}
def on_choose():
    if not on[0]: return
    c = reg("ebp"); nm = leafname(c)
    curc[0] = c if (nm in watch or cfg.get("all")) else None; curc[1] = []
def on_pen1():   # 0x10784b57: neighbour edi has only register esi left: COSTS[esi] += 100 * neighbour spill (eax)
    if not on[0] or curc[0] is None: return
    nb = reg("edi")
    curc[1].append(['only', u32(nb + 0x1c), leafname(nb), RNUM.get(reg("esi"), reg("esi")), reg("eax")])
def on_pen2():   # 0x107ac667: COSTS[[esi+4]+0x1c] += [esi+8]
    if not on[0] or curc[0] is None: return
    e = reg("esi")
    curc[1].append(['add', RNUM.get(u32(u32(e + 4) + 0x1c), '?'), struct.unpack("<i", rd(e + 8, 4))[0]])
def on_pref():   # 0x1078c06b: COSTS[[eax+4]+0x1c] -= [eax+8]
    if not on[0] or curc[0] is None: return
    e = reg("eax")
    curc[1].append(['pref', RNUM.get(u32(u32(e + 4) + 0x1c), '?'), struct.unpack("<i", rd(e + 8, 4))[0]])
Hook(0x1077C08E, on_pipe); Hook(0x10782b26, on_insert); Hook(0x107834A4, on_chosen)
if cfg.get("penalties"):
    Hook(0x10782A0D, on_choose); Hook(0x10784B57, on_pen1); Hook(0x107AC667, on_pen2); Hook(0x1078C06B, on_pref)
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
