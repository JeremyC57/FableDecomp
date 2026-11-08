import sys, os, json, struct, traceback
sys.path.insert(0, os.environ['S'] + '/w22/c2probe/tools')
import c2dbg
out = {"log": [], "error": None}
def finish():
    json.dump(out, open(cfg["out"], "w"))
try:
    gdb, inf, cfg = c2dbg.gdb_setup()
except Exception as e:
    out["error"] = traceback.format_exc(); cfg = json.load(open(os.environ["C2PRIO_CONFIG"])); finish(); raise
L = out["log"].append
def rd(a, n):
    try: return bytes(inf.read_memory(a, n))
    except Exception: return None
def u32(a):
    b = rd(a, 4); return struct.unpack("<I", b)[0] if b else None
def reg(n): return int(gdb.selected_frame().read_register(n)) & 0xffffffff
def isptr(v): return v is not None and 0x00010000 <= v < 0x7fff0000
def cstr(a, n=64):
    b = rd(a, n)
    if not b: return None
    s = b.split(b"\0")[0]
    if len(s) >= 2 and all(0x20 <= c < 0x7f for c in s): return s.decode()
    return None
def hexdump(a, n):
    b = rd(a, n)
    if b is None: return ["<unreadable>"]
    return [f"{a+i:08x}: " + " ".join(f"{struct.unpack_from('<I',b,i+j)[0]:08x}" for j in range(0, min(16, n-i), 4)) for i in range(0, n, 16)]
def strings_near(a, n, depth, pref):
    b = rd(a, n)
    if not b: return
    for i in range(0, n - 3, 4):
        v = struct.unpack_from("<I", b, i)[0]
        if isptr(v):
            s = cstr(v)
            if s: L(f"{pref}+{i:#x} -> str {s!r}")
            elif depth > 0:
                v2 = u32(v)
                if isptr(v2):
                    s2 = cstr(v2)
                    if s2: L(f"{pref}+{i:#x} -> ptr -> +0 str {s2!r}")
                for k in (0x4, 0x8, 0xc, 0x10, 0x14, 0x18, 0x1c, 0x20):
                    v3 = u32(v + k)
                    if isptr(v3):
                        s3 = cstr(v3)
                        if s3: L(f"{pref}+{i:#x} -> ptr -> +{k:#x} str {s3!r}")
nfn = 0
class H(gdb.Breakpoint):
    def __init__(self, addr, fn):
        super().__init__(f"*{addr:#x}", internal=True); self.fn = fn
    def stop(self):
        try: self.fn()
        except Exception: out["error"] = traceback.format_exc(); return True
        return False
def on_driver():
    global nfn
    nfn += 1
    fn = u32(reg("esp") + 4)
    L(f"=== DRIVER entry #{nfn}: fn={fn:#x}")
    if nfn <= 2:
        for l in hexdump(fn, 0xa0): L("  " + l)
        strings_near(fn, 0xa0, 1, "  fn")
def on_sorted():
    head = u32(0x108807b4)
    L(f"--- SORTED: head={head:#x}")
    c = head; n = 0
    while c and n < 40:
        b = rd(c, 0x48)
        f = lambda o: struct.unpack_from("<I", b, o)[0]
        leaf = f(0)
        L(f"  cand {c:#x}: id {f(0x1c)} prio {struct.unpack_from('<i',b,0xc)[0]} k44 {f(0x44)} spill {struct.unpack_from('<i',b,0x40)[0]} refs {f(0x24)} reg {f(0x10):#x} allowed {f(0x20):#x} leaf {leaf:#x} b4 {b[4]:#x} b5 {b[5]:#x} b6 {b[6]:#x} +28 {f(0x28):#x} +2c {f(0x2c):#x} +34 {f(0x34):#x} +38 {f(0x38):#x} +3c {f(0x3c):#x}")
        if n < 6:
            for l in hexdump(c, 0x48): L("    " + l)
            lb = rd(leaf, 0x40)
            if lb:
                L(f"    leaf kind byte +4 = {lb[4]:#x}, word +0x10 = {struct.unpack_from('<H',lb,0x10)[0]:#x}")
                for l in hexdump(leaf, 0x40): L("    L " + l)
                strings_near(leaf, 0x40, 1, "    leaf")
            al = f(0x20)
            if al:
                ch = u32(al); bits = []
                k = 0
                while ch and k < 8:
                    base, nxt, word = struct.unpack("<III", rd(ch, 12)); bits += [base + i for i in range(32) if word >> i & 1]; ch = nxt; k += 1
                L(f"    allowed bits: {bits}")
        c = f(0x14); n += 1
def on_chosen():
    ebp = reg("ebp"); r = u32(ebp + 0x10)
    rn = (r - 0x108a97a4) // 0x60 if r and r >= 0x108a97a4 else None
    L(f"  CHOSEN cand {ebp:#x} id {u32(ebp+0x1c)} regptr {r:#x} -> regnum {rn}  eax={reg('eax'):#x}")
H(0x107821a7, on_driver); H(0x10783b6b, on_sorted); H(0x10782a1d, on_chosen)
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
