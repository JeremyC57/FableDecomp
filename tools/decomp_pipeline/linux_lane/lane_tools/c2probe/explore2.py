import sys, os, json, struct, traceback
sys.path.insert(0, os.environ['S'] + '/w22/c2probe/tools')
import c2dbg
out = {"log": [], "error": None}
cfg = json.load(open(os.environ["C2PRIO_CONFIG"]))
def finish(): json.dump(out, open(cfg["out"], "w"))
try:
    gdb, inf, cfg = c2dbg.gdb_setup()
except Exception:
    out["error"] = traceback.format_exc(); finish(); raise
L = out["log"].append
def rd(a, n):
    try: return bytes(inf.read_memory(a, n))
    except Exception: return None
def u32(a):
    b = rd(a, 4); return struct.unpack("<I", b)[0] if b else None
def s32(a):
    b = rd(a, 4); return struct.unpack("<i", b)[0] if b else None
def reg(n): return int(gdb.selected_frame().read_register(n)) & 0xffffffff
def cstr(a, n=64):
    b = rd(a, n)
    if not b: return None
    return b.split(b"\0")[0].decode('latin1')
def candname(c):
    leaf = u32(c)
    if not leaf: return '?'
    kind = rd(leaf, 8)[4]
    if kind == 0xd:
        node = u32(leaf + 0x28)
        nb = rd(node, 0x20) if node else None
        return f"const node={node:#x} " + (" ".join(f"{struct.unpack_from('<I',nb,i)[0]:08x}" for i in range(0,0x20,4)) if nb else "")
    st = u32(leaf)
    return f"{cstr(u32(st+4)) if st else '?'} (kind {kind:#x})"
WANT = ("p1_a3_b2", "p7_loop")
cur = {"on": False, "blk": None}
class H(gdb.Breakpoint):
    def __init__(self, addr, fn, always=False):
        super().__init__(f"*{addr:#x}", internal=True); self.fn = fn; self.always = always
    def stop(self):
        if not cur["on"] and not self.always: return False
        try: self.fn()
        except Exception: out["error"] = traceback.format_exc(); return True
        return False
def on_driver():
    fn = u32(reg("esp") + 4); name = cstr(u32(u32(fn) + 4))
    cur["on"] = any(w in name for w in WANT)
    if cur["on"]: L(f"=== DRIVER {name}  fn={fn:#x} baseline(fn+0x38)={u32(fn+0x38)}")
def on_pipeline_entry():  # 0x1077c367 entry: the per-function candidate build pipeline? record fn name if arg looks like fn
    pass
def on_def():
    c = reg("esi"); L(f"  DEF  cand id {u32(c+0x1c)} {candname(c)} tnum={reg('eax')} line={u32(0x108a9114)} blk={u32(reg('esp')+0x14):#x}")
def on_use():
    c = reg("ecx"); L(f"  USE  cand id {u32(c+0x1c)} {candname(c)} tnum={u32(reg('esp')+0x1c)} line={u32(0x108a9114)} blk={u32(reg('esp')+0x14):#x}")
def on_blk_start():
    b = reg("ebx"); cur["blk"] = b
    L(f"  BLOCK {b:#x} depth={rd(b+0xa6,1)[0]} first={u32(b+0x1c):#x} end={u32(b+0x20):#x}")
def lst(head):
    res = []; n = 0
    while head and n < 200:
        b = rd(head, 0x20); res.append(f"#{struct.unpack_from('<I',b,0x1c)[0]}(f6={b[6]:#x},cost={struct.unpack_from('<i',b,0x18)[0]},reg={struct.unpack_from('<I',b,0x10)[0]:#x})"); head = struct.unpack_from('<I',b,0x14)[0]; n += 1
    return " ".join(res)
def on_blk_end():
    wk = reg("eax"); w = reg("ebp"); esp = reg("esp")
    L(f"  BLKEND blk={cur['blk']:#x} w={w} wK={wk} K={wk//w if w else '?'} refl=[{lst(reg('ebx'))}] live=[{lst(u32(esp+0x10))}]  stack10..44=" + " ".join(f"{u32(esp+o):08x}" for o in range(0x10,0x48,4)))
def on_chosen():
    c = u32(reg("esp") + 0x20); r = reg("ebx")
    costs = [s32(0x10880680 + 4*i) for i in range(10)]
    L(f"  CHOSEN cand id {u32(c+0x1c)} {candname(c)} -> reg {r}  costs[0..9]={costs}")
def on_newcand():
    c = reg("eax"); L(f"  NEW cand {c:#x} id {u32(c+0x1c)} leaf={u32(c):#x} {candname(c)} ret={u32(reg('esp')):#x}")
def on_sorted():
    head = u32(0x108807b4); L(f"  SORTED head={head:#x}")
    c = head; n = 0
    while c and n < 50:
        b = rd(c, 0x48); f = lambda o: struct.unpack_from("<I", b, o)[0]
        L(f"    #{f(0x1c)} {candname(c)} prio={struct.unpack_from('<i',b,0xc)[0]} k44={f(0x44)} spill={struct.unpack_from('<i',b,0x40)[0]} refs={f(0x24)} +28={f(0x28):#x} +2c={f(0x2c):#x} +34={f(0x34)} f5={b[5]:#x} f6={b[6]:#x}")
        c = f(0x14); n += 1
def on_resort():
    L("  RESORT"); on_sorted()
def on_split():
    L(f"  SPLIT call: args [esp+4..c]={u32(reg('esp')+4):#x} {u32(reg('esp')+8):#x} {u32(reg('esp')+0xc):#x}")
H(0x107821a7, on_driver, True); H(0x1077fae3, on_def); H(0x1077fbf7, on_use); H(0x1078244d, on_blk_start); H(0x107828eb, on_blk_end)
H(0x107834a4, on_chosen); H(0x1077c1de, on_newcand); H(0x10783b6b, on_sorted); H(0x1078abac, on_resort); H(0x1078b2aa, on_split)
exited = []
gdb.events.exited.connect(exited.append)
try:
    while not exited and not out["error"]:
        try: gdb.execute("continue", to_string=True)
        except gdb.error as e:
            if exited or not inf.threads(): break
            out["error"] = str(e); break
except Exception:
    out["error"] = traceback.format_exc()
finish()
