"""lk_gdb.py -- the gdb side of lk.py (run by c2dbg.probe inside gdb; never run it by hand).

Hooks in c2.dll 13.10.3077 (sha256 bcd28f39..., base 0x10700000):
  PIPE   0x1077C08E  per-function register-allocation pipeline entry; eax = function record, name = [[eax]+4].
                     Everything logged since the previous PIPE belongs to this function (c2 optimises one function
                     at a time and allocates it right after).
  SORTA  0x1075CD0C  `call 0x1076A2F3` (merge sort of node edi's operand list [edi+0x28], comparator 0x1076A394)
                     inside 0x1075CC68 (the recursive tree canonicaliser: child keys first, then sort, then own key).
  SORTB  0x1075EEA5  the same sort inside 0x1075D6AD (the CSE pass that makes the CSE temps through 0x1075C327).
  RETA / RETB 0x1075CD11 / 0x1075EEAA: eax = new head of the sorted list.
  CMP    0x1076A394  the comparator itself: ecx = a, edx = b; returns 1 if key(a) < key(b) (unsigned [+0x10]),
                     0 if equal, -1 if greater, so the list ends up in DESCENDING key order.
  SYM    0x1075A0C5  (cfg "syms": true) symbol allocation, al = kind; the return site names the allocator
                     (0x1075C331 = CSE temp 0x1075C327, 0x1076F08C = inline-expansion local/param, 0x10769FE4 = field ...).

Node layout read here (0x38 bytes): +0 next sibling, +4 op (dword), +8 kind byte, +0x10 key (need:8 count:8 hash:16),
+0x14 dword (low 16 = source line, unproven), +0x18 symbol (leaf kinds 0..3) / constant low word (kind 7),
+0x28 first operand (interior kinds >= 13), +0x30 / +0x34 (kinds 5 / 6).  Symbol: +4 kind byte, +0x14 value node
(CSE forwarding), +0x1c number, [+0] -> +4 name.
Events (compact lists, written to cfg["out"] as JSON):
  ["P", name, n_kept]            function boundary
  ["S", pass, parent_desc, [child_desc...]]     before a sort (pass "a" = canonicaliser, "b" = CSE pass)
  ["R", pass, parent_ptr, [child_ptr...]]       after it (new order)
  ["C", a_ptr, b_ptr]                           one comparator call
  ["Y", kind, sym_ptr, number, alloc_site]      a symbol allocation (only with cfg "syms")
desc = [ptr, op, kind, key, w14, leaf, sub]: leaf = [sym_ptr, number, sym_kind, fwd_ptr, name] for kinds 0..3,
[w18, w1c] for kind 7, [w30, w34] for kinds 5 / 6, else None; sub = operand descs (interior; depth-limited) or the
desc of a forwarded value node.
"""
import json, os, struct, sys, traceback
sys.dont_write_bytecode = True
sys.path.insert(0, os.environ.get("LK_C2PROBE", "/home/user/FableDecomp/tools/decomp_pipeline/linux_lane/lane_tools/c2probe"))
import gdb  # noqa
import c2dbg

out = {"events": [], "error": None, "dropped": 0, "functions": [], "exited": False}
cfg = json.load(open(os.environ["C2PRIO_CONFIG"]))


def fin():
    json.dump(out, open(cfg["out"], "w"), separators=(",", ":"))


try:
    _g, inf, cfg = c2dbg.gdb_setup()
except Exception:
    out["error"] = traceback.format_exc()
    fin()
    raise SystemExit

FUNC = cfg.get("func")            # substring of the function name to keep (None = keep every function)
DEPTH = int(cfg.get("depth", 4))  # operand-tree depth logged per sort child
SYMS = bool(cfg.get("syms", False))


def rd(a, n): return bytes(inf.read_memory(a, n))
def u32(a): return struct.unpack("<I", rd(a, 4))[0]
def reg(n): return int(gdb.selected_frame().read_register(n)) & 0xFFFFFFFF


def cstr(a):
    if not a or not (0x10000 <= a < 0x7FFF0000):
        return None
    for size in (128, 32, 8):
        try:
            s = rd(a, size).split(b"\0")[0]
            return s.decode("latin1") if s and all(32 <= c < 127 for c in s) else None
        except gdb.MemoryError:
            continue
    return None


names = {}
window = []


def symname(s):
    if s in names:
        return names[s]
    nm = None
    try:
        st = u32(s)
        nm = cstr(u32(st + 4)) if st else None
    except Exception:
        nm = None
    names[s] = nm
    return nm


def desc(p, depth):
    try:
        raw = rd(p, 0x38)
    except Exception:
        return [p, None]
    w = struct.unpack("<14I", raw)
    kind = raw[8]
    leaf = None
    sub = None
    if kind <= 3:
        s = w[6]
        try:
            sraw = rd(s, 0x20)
            sk = sraw[4]
            fwd = struct.unpack_from("<I", sraw, 0x14)[0]
            num = struct.unpack_from("<I", sraw, 0x1C)[0]
        except Exception:
            sk, fwd, num = None, 0, None
        leaf = [s, num, sk, fwd, symname(s)]
        if fwd and kind in (1, 2) and depth > 0:
            sub = [desc(fwd, depth - 1)]
    elif kind == 7:
        leaf = [w[6], w[7]]
    elif kind == 4:
        try:
            leaf = [w[6], u32(w[6] + 0x28)]
        except Exception:
            leaf = [w[6], None]
    elif kind == 8:
        try:
            leaf = [w[6], u32(u32(w[6] + 0x24))]
        except Exception:
            leaf = [w[6], None]
    elif kind == 9:
        leaf = [w[6], struct.unpack_from("<I", raw, 0x16)[0]]
    elif kind in (5, 6):
        leaf = [w[12], w[13]]
        if w[13] and depth > 0:
            sub = [desc(w[13], depth - 1)]
    elif kind >= 13 and depth > 0:
        sub = []
        c = w[10]
        n = 0
        while c and n < 16:
            sub.append(desc(c, depth - 1))
            try:
                c = u32(c)
            except Exception:
                break
            n += 1
    return [p, w[1], kind, w[4], w[5], leaf, sub]


def children(head):
    res = []
    n = 0
    while head and n < 64:
        res.append(head)
        head = u32(head)
        n += 1
    return res


class Hook(gdb.Breakpoint):
    def __init__(self, addr, fn):
        super().__init__(f"*{addr:#x}", internal=True)
        self.fn = fn

    def stop(self):
        try:
            self.fn()
        except Exception:
            window.append(["E", traceback.format_exc()[-600:]])
        return False


def on_pipe():
    fn = reg("eax")
    try:
        nm = cstr(u32(u32(fn) + 4)) or "?"
    except Exception:
        nm = "?"
    keep = FUNC is None or FUNC in nm
    out["functions"].append([nm, len(window), keep])
    if keep:
        out["events"].extend(window)
        out["events"].append(["P", nm, len(window)])
    else:
        out["dropped"] += len(window)
    del window[:]
    names.clear()


def sort_call(tag):
    def f():
        par = reg("edi")
        head = reg("eax")
        kids = children(head)
        window.append(["S", tag, desc(par, 0), [desc(k, DEPTH) for k in kids]])
    return f


def sort_ret(tag):
    def f():
        window.append(["R", tag, reg("edi"), children(reg("eax"))])
    return f


def on_cmp():
    window.append(["C", reg("ecx"), reg("edx")])


pend = {}
rets = {}


def on_alloc():
    esp = reg("esp")
    pend[esp] = (reg("eax") & 0xFF, u32(esp))
    ra = u32(esp)
    if ra not in rets:
        rets[ra] = Hook(ra, on_alloc_ret)


def on_alloc_ret():
    p = pend.pop(reg("esp") - 4, None)
    if p is None:
        return
    s = reg("eax")
    try:
        num = u32(s + 0x1C)
    except Exception:
        num = None
    window.append(["Y", p[0], s, num, p[1]])


Hook(0x1077C08E, on_pipe)
Hook(0x1075CD0C, sort_call("a"))
Hook(0x1075CD11, sort_ret("a"))
Hook(0x1075EEA5, sort_call("b"))
Hook(0x1075EEAA, sort_ret("b"))
Hook(0x1076A394, on_cmp)
if SYMS:
    Hook(0x1075A0C5, on_alloc)

exited = []
gdb.events.exited.connect(exited.append)
try:
    while not exited:
        try:
            gdb.execute("continue", to_string=True)
        except gdb.error:
            if exited or not inf.threads():
                break
            raise
except Exception:
    out["error"] = traceback.format_exc()
out["exited"] = bool(exited)
out["tail_window"] = len(window)
fin()
