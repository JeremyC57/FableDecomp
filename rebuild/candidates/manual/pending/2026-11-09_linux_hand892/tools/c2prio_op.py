#!/usr/bin/env python3
"""c2prio.py -- read VC7.1 c2.dll's own register-allocation decisions for the functions of one source file.

    python3 c2prio.py <src.cpp>                          # every function in the TU
    python3 c2prio.py <src.cpp> --func Update            # functions whose mangled name contains "Update"
    python3 c2prio.py <src.cpp> --func '?p1_a3_b2@@YAHH@Z' --trace     # + every colouring step
    python3 c2prio.py <src.cpp> --func NAME --blocks     # + each block's share of each priority
    python3 c2prio.py <src.cpp> --func NAME --flags "/Gr /EHsc"        # extra cl flags (BASE_FLAGS /O2 /Oy /W3 always)
    python3 c2prio.py <src.cpp> --json out.json --keep   # raw trace; keep the run directory (gdb.log, obj)

Environment: `source $S/wine/env.sh` first (FABLE_VC71 = the RTM 3077 toolchain, WINEPREFIX...). INCLUDE is set like
verify_and_land.env(): <toolchain>/include, $FABLE_HDR_OVERLAY dirs, rebuild/include. The source gets the lane's
vc71() rewrites (__thiscall -> __fastcall, static_assert lines dropped). Needs gdb with Python and winedbg.

What it prints, per function, in the order c2 coloured the candidates (docs: C2_ALLOCATOR_PROBE.md):

  #  id  candidate        lines      prio  key  spill refs  allowed  register
  0   3  a                5           32    24     8    4  ...sibp  esi

  - prio  = candidate +0x0c, the priority computed by the color.c pass at 0x10782473:
            sum over blocks b that reference it of  w(b) * K(b) * cost_b  minus  w(b) * K(b) for every block it is
            live through without a reference.  w(b) = 1 << loop depth (block +0xa6), K(b) = number of candidates
            referenced in b, cost_b = this candidate's cost in b (2 per variable reference, 0/1 for a constant).
  - key   = candidate +0x44, the tie-break: the number of the last tuple that writes it.  Tuples are numbered block by
            block, each block from its last tuple to its first, so the value DEFINED EARLIER in a block has the LARGER
            key and wins an equal-priority tie (p3/p4 in research861 probe/prio.cpp).
  - spill = candidate +0x40, sum of w(b) * cost_b (no K): a candidate with spill <= 0 is not worth a register.
  - refs  = candidate +0x24, the reference count.
  - allowed: registers still allowed when the sorted list was built, in the allocator's table order
            a=eax c=ecx d=edx s=esi i=edi b=ebx p=ebp (a dot = not allowed; `...sibp` = live across a call).
  - register: the register 0x107833f2 gave it (cheapest allowed register, ties to the EARLIER one in the table order
            eax, ecx, edx, esi, edi, ebx, ebp), or `memory` / `immediate`, or `split: ...` with the pieces' registers.
  - lines: source lines of the tuples that read or write it (absolute when the function's definition line is found in
            the source, else +N relative to the function's first line).

The compile itself is a normal cl.exe run (the object is byte-identical to a plain compile up to the COFF timestamp);
only a COPY of c2.dll is patched to spin at its entry so gdb can attach (tools/c2dbg.py).
"""
import json
import os
import re
import struct
import sys

try:
    import gdb  # this file is also the script gdb runs inside the debugger
except ImportError:
    gdb = None

# --- c2.dll 13.10.3077 (sha256 bcd28f39...) addresses, image base 0x10700000 (loads there; c2dbg checks) --------------
PIPE = 0x1077C08E        # per-function register-allocation pipeline entry; eax = function record (name at [[fn]+4])
NEWCAND = 0x1077C1DE     # ret of the candidate constructor 0x1077c10f: eax = new candidate (0x48 bytes), [esp] = call site
PIECE_SITES = {0x1078B499: "esi", 0x1078BF4B: "ebp"}   # constructor call sites that make split pieces; parent register
DEF = 0x1077FAE3         # 0x10780000 (tuple walk): a tuple writes candidate esi; eax = tuple number; [esp+0x14] = block
USE = 0x1077FBF7         # 0x10780000: a tuple reads candidate ecx; [esp+0x1c] = tuple number; [esp+0x14] = block
CUR_LINE = 0x108A9114    # the current tuple's line (tuple +0x14, relative to the function's first line) while walking
DRIVER = 0x107821A7      # color.c allocator driver entry (per function; loops over the register classes 6..0)
BLKSTART = 0x1078244D    # priority pass 0x10782473: start of a block; ebx = block (+0xa6 loop depth, +0x1c/+0x20 tuples)
BLKEND = 0x107828EB      # priority pass: K counted; eax = w*K, ebp = w, ebx = referenced-not-live list, [esp+0x10] = live list
SORTED = 0x10783B6B      # driver: 0x1078482f built the sorted list at LIST_HEAD for class ebx
LIST_HEAD = 0x108807B4
HASH = 0x108807B8        # 1024 buckets by id & 0x3ff, chained at candidate +0x30
LOWSPILL = 0x107896AE    # driver: head candidate ebp has spill (+0x40) <= 0
NOCOLOR = 0x10722CBB     # driver calls it when the head candidate (ecx) has no allowed register left: stays in memory / split
DROP = 0x10782AB8        # drop pass 0x10782978: candidate eax dropped (fewer than 2 references and spill <= 0)
FORWARD = 0x10782B1D     # drop pass: candidate eax has exactly 2 references: 0x107820b7 checks whether it is a copy to forward
CHOOSE = 0x10782A0D      # driver: call 0x1078368e for the head candidate ebp (interference / costs)
CHOSEN = 0x107834A4      # 0x107833f2: register chosen; ebx = register number, [esp+0x20] = candidate; costs in COSTS
COSTS = 0x10880680       # per-register cost array (by register number 1..8) the chooser compares (strict <, table order)
SPLIT = 0x1078B2AA       # live-range split pass entry: [esp+8] = bit set of the candidate ids to split
RESORT = 0x1078ABAC      # driver: priorities recomputed and the list re-sorted after a split (ebx = class)
ENDS = (0x10756957, 0x10782328)   # the driver's ret sites
CONSTBACK = 0x10785366   # 0x10785300 (after code generation): register descriptor esi, holding only constants whose benefit
                         # (+0x38) is <= 2, is released (0x10795796) and the constants become immediates again
# Expression temporaries (code generation, after the allocator; the regasg.c equivalent): 0x10775acc gives a temporary
# a register. TEMP_PTR: it has fetched the class's rotating pointer into the order table 0x108a9738 (eax; [eax] is the
# register the rotation would try first). TEMP_OTHER: the first-free / best-score path chose edi instead (no rotation).
# TEMP_EXIT: common exit, eax = 3 * register number; the arguments [esp+0x28]/[esp+0x24] (tuple / operand; line word at +0x14).
# ROT_RESET: the pointer is reset to the table start once per function (0x10775782, code generation init).
TEMP_PTR = 0x10775B43
TEMP_OTHER = 0x10714CD4
TEMP_EXIT = 0x10775BE8
ROT_PTR = 0x108A7D0C
ROT_TABLE = 0x108A9738
ROT_RESETS = (0x10775844, 0x107DFF94)
REGDESC = 0x108A97A4     # register descriptors, 0x60 bytes each, indexed by register number (+0x1c = the number)
ORDER_TABLE = 0x108691A0  # [1, 2, 3, 7, 8, 4, 6, 0]: eax, ecx, edx, esi, edi, ebx, ebp

REG_NAMES = {0: "noreg", 1: "eax", 2: "ecx", 3: "edx", 4: "ebx", 5: "esp", 6: "ebp", 7: "esi", 8: "edi"}
ORDER = [1, 2, 3, 7, 8, 4, 6]
LETTERS = {1: "a", 2: "c", 3: "d", 7: "s", 8: "i", 4: "b", 6: "p"}
KINDS = {3: "temp", 4: "local", 5: "param", 7: "global", 13: "const"}
CLASS_NAMES = {0: "integer", 1: "x87", 5: "mmx", 6: "sse"}


# ======================================================================================================================
# gdb side
# ======================================================================================================================
def tracer():
    import time
    import traceback
    sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
    import c2dbg

    t0 = time.time()
    cfg = json.load(open(os.environ["C2PRIO_CONFIG"]))
    out = {"functions": [], "error": None}

    def finish():
        out["seconds"] = round(time.time() - t0, 2)
        with open(cfg["out"], "w") as fh:
            json.dump(out, fh)

    try:
        _gdb, inf, cfg = c2dbg.gdb_setup()
    except Exception:
        out["error"] = traceback.format_exc()
        finish()
        return

    def rd(a, n):
        return bytes(inf.read_memory(a, n))

    def u32(a):
        return struct.unpack("<I", rd(a, 4))[0]

    def s32(a):
        return struct.unpack("<i", rd(a, 4))[0]

    def reg(n):
        return int(gdb.selected_frame().read_register(n)) & 0xFFFFFFFF

    def cstr(a):
        if not a:
            return None
        for size in (256, 64, 16, 4):
            try:
                return rd(a, size).split(b"\0")[0].decode("latin1")
            except gdb.MemoryError:
                continue
        return None

    def bitset(addr):
        """c2's sparse bit set: header -> chunks {base, next, 32 bits}."""
        if not addr:
            return None
        bits, chunk, n = [], u32(addr), 0
        while chunk and n < 4096:
            base, nxt, word = struct.unpack("<III", rd(chunk, 12))
            bits += [base + i for i in range(32) if word >> i & 1]
            chunk, n = nxt, n + 1
        return bits

    leaves = {}

    def leaf(sym):
        """A candidate's leaf (+0): +4 kind (3 temp, 4 local, 5 param, 7 global, 0xd const), +0x10 type word
        (class = type >> 12: 1 int, 4 x87), +0 storage record whose +4 is the name; a constant's node at +0x28."""
        if sym in leaves:
            return leaves[sym]
        info = {}
        if sym:
            raw = rd(sym, 0x30)
            st = struct.unpack_from("<I", raw, 0)[0]
            info = {"kind": raw[4], "type": struct.unpack_from("<H", raw, 0x10)[0]}
            if st and raw[4] != 13:
                try:
                    info["name"] = cstr(u32(st + 4))
                except gdb.MemoryError:
                    pass
            if raw[4] == 13:
                node = struct.unpack_from("<I", raw, 0x28)[0]
                if node:
                    try:
                        nb = rd(node, 0x20)
                        info["node"] = [struct.unpack_from("<I", nb, i)[0] for i in range(0, 0x20, 4)]
                    except gdb.MemoryError:
                        pass
        leaves[sym] = info
        return info

    def regnum(p):
        if not p:
            return 0
        if REGDESC <= p < REGDESC + 0x60 * 64 and (p - REGDESC) % 0x60 == 0:
            return (p - REGDESC) // 0x60
        return -1

    def cand_snapshot(c, full=True):
        raw = rd(c, 0x48)
        f = lambda o: struct.unpack_from("<I", raw, o)[0]
        s = lambda o: struct.unpack_from("<i", raw, o)[0]
        d = {"id": f(0x1C), "prio": s(0xC), "key": f(0x44), "spill": s(0x40), "refs": f(0x24), "f5": raw[5], "f6": raw[6],
             "reg": regnum(f(0x10))}
        if full:
            d["allowed"] = bitset(f(0x20))
        return d

    def walk(head, full=True):
        res, seen = [], set()
        while head and head not in seen and len(res) < 100000:
            seen.add(head)
            res.append(cand_snapshot(head, full))
            head = u32(head + 0x14)
        return res

    def hashed():
        """[(id, candidate address)] for every candidate in the hash table."""
        res, table = [], rd(HASH, 4096)
        for b in range(1024):
            c, seen = struct.unpack_from("<I", table, 4 * b)[0], set()
            while c and c not in seen:
                seen.add(c)
                res.append((u32(c + 0x1C), c))
                c = u32(c + 0x30)
        return res

    def cand_list(head):
        """[id, flags +6, cost +0x18, regnum] along the +0x14 chain (the priority pass's block lists)."""
        res, seen = [], set()
        while head and head not in seen and len(res) < 100000:
            seen.add(head)
            raw = rd(head, 0x20)
            res.append([struct.unpack_from("<I", raw, 0x1C)[0], raw[6], struct.unpack_from("<i", raw, 0x18)[0],
                        regnum(struct.unpack_from("<I", raw, 0x10)[0])])
            head = struct.unpack_from("<I", raw, 0x14)[0]
        return res

    kind, names = cfg["match"]

    def wanted(name):
        if name is None:
            return False
        if kind == "all":
            return True
        if kind == "exact":
            return name in names
        return any(n in name for n in names)

    state = {"cur": None, "blk": None, "phase": None, "post": None, "rot": None}

    def ev(e, **kw):
        state["cur"]["events"].append(dict(e=e, **kw))

    def on_pipe():
        fn = reg("eax")
        name = None
        try:
            name = cstr(u32(u32(fn) + 4))
        except gdb.MemoryError:
            pass
        leaves.clear()
        state["post"] = None
        if wanted(name):
            state["cur"] = {"name": name, "fn": fn, "events": []}
            state["phase"] = "build"
            out["functions"].append(state["cur"])
        else:
            state["cur"] = None

    def on_new():
        c = reg("eax")
        site = u32(reg("esp"))
        parent = None
        if site in PIECE_SITES:
            parent = u32(reg(PIECE_SITES[site]) + 0x1C)
        sym = u32(c)
        ev("new", id=u32(c + 0x1C), parent=parent, leaf=leaf(sym), addr=c, cls=rd(c + 4, 1)[0])

    def on_def():
        c = reg("esi")
        ev("ref", id=u32(c + 0x1C), rw="w", n=reg("eax"), line=u32(CUR_LINE), blk=u32(reg("esp") + 0x14))

    def on_use():
        c = reg("ecx")
        esp = reg("esp")
        ev("ref", id=u32(c + 0x1C), rw="r", n=u32(esp + 0x1C), line=u32(CUR_LINE), blk=u32(esp + 0x14))

    def on_driver():
        state["phase"] = "alloc"
        ev("driver")

    def on_blkstart():
        b = reg("ebx")
        state["blk"] = b
        depth = rd(b + 0xA6, 1)[0]
        # lines of the block's tuples (tuple +0x14 word), first to last
        lines, t, end, n = [], u32(b + 0x1C), u32(b + 0x20), 0
        while t and t != end and n < 5000:
            ln = struct.unpack("<H", rd(t + 0x14, 2))[0]
            if ln:
                lines.append(ln)
            t, n = u32(t), n + 1
        ev("blk", blk=b, depth=depth, lines=sorted(set(lines)))

    def on_blkend():
        wk, w = reg("eax"), reg("ebp")
        ev("blkend", blk=state["blk"], w=w, wK=wk, refl=cand_list(reg("ebx")), live=cand_list(u32(reg("esp") + 0x10)))

    def on_sorted():
        cls = reg("ebx")
        allc = [(cid, cand_snapshot(c, False)) for cid, c in hashed()]
        ev("sorted", cls=cls, list=walk(u32(LIST_HEAD)), all=[dict(s, id=cid) for cid, s in allc])

    def on_lowspill():
        c = reg("ebp")
        ev("lowspill", id=u32(c + 0x1C), spill=s32(c + 0x40))

    def on_nocolor():
        c = reg("ecx")
        ev("nocolor", id=u32(c + 0x1C), prio=s32(c + 0xC))

    def on_drop():
        c = reg("eax")
        ev("drop", id=u32(c + 0x1C), refs=u32(c + 0x24), spill=s32(c + 0x40))

    def on_forward():
        c = reg("eax")
        ev("forward", id=u32(c + 0x1C))

    def on_choose():
        c = reg("ebp")
        ev("choose", id=u32(c + 0x1C), prio=s32(c + 0xC), allowed=bitset(u32(c + 0x20)))

    def on_chosen():
        c = u32(reg("esp") + 0x20)
        costs = list(struct.unpack("<9i", rd(COSTS, 36)))
        ev("chosen", id=u32(c + 0x1C), reg=reg("ebx"), costs=costs)

    def on_split():
        ev("split", ids=bitset(u32(reg("esp") + 8)) or [])

    def on_resort():
        ev("resort", cls=reg("ebx"), list=walk(u32(LIST_HEAD), False))

    def on_end():
        ev("end")
        state["phase"] = "done"
        state["post"] = state["cur"]      # code generation follows: --rotation events go to this function

    def pev(e, **kw):
        if state["post"] is not None:
            state["post"]["events"].append(dict(e=e, **kw))

    def on_temp_ptr():
        p = reg("eax")
        state["rot"] = {"ptr": u32(p) if p else 0, "route": "rotation"}

    def on_temp_other():
        if state["rot"] is None:
            state["rot"] = {"ptr": None, "route": "first free / score"}
        else:
            state["rot"]["route"] = "first free / score"

    def on_temp_exit():
        r = reg("eax") // 3
        esp = reg("esp")
        line = None
        for off in (0x28, 0x24):          # the two arguments (ret 8): the tuple carries its line word at +0x14
            try:
                a = u32(esp + off)
                ln = struct.unpack("<H", rd(a + 0x14, 2))[0] if a else 0
                if 0 < ln < 20000:
                    line = ln
                    try:
                        opc = u32(a + 4) & 0xffff
                        nm = rd(u32(0x10874398 + 4 * (opc + 4)), 12).split(b"\0")[0].decode("latin-1") if opc < 0x240 else hex(opc)
                    except Exception:
                        nm = "?"
                    state["lastop"] = nm
                    break
            except gdb.MemoryError:
                pass
        rot = state["rot"] or {"ptr": None, "route": "hint / other"}
        state["rot"] = None
        pp = u32(ROT_PTR)
        pev("temp", reg=r, line=line, ptr=rot["ptr"], route=rot["route"], after=u32(pp) if pp else None, op=state.get("lastop"))
        state["lastop"] = None

    def on_rot_reset():
        pev("rotreset")

    def on_constback():
        d = reg("esi")
        ev("constback", reg=regnum(d), benefit=s32(d + 0x38))

    handlers = {NEWCAND: on_new, DEF: on_def, USE: on_use, DRIVER: on_driver, SORTED: on_sorted,
                LOWSPILL: on_lowspill, NOCOLOR: on_nocolor, DROP: on_drop, FORWARD: on_forward,
                CHOOSE: on_choose, CHOSEN: on_chosen, SPLIT: on_split, RESORT: on_resort}
    if cfg.get("blocks"):
        handlers[BLKSTART] = on_blkstart
        handlers[BLKEND] = on_blkend
    for e in ENDS:
        handlers[e] = on_end
    handlers[CONSTBACK] = on_constback
    failed = []

    class Hook(gdb.Breakpoint):
        def __init__(self, addr, fn, always=False):
            super().__init__(f"*{addr:#x}", internal=True)
            self.fn, self.always = fn, always

        def stop(self):
            if state["cur"] is None and not self.always:
                return False
            try:
                self.fn()
                return False
            except Exception:
                failed.append(traceback.format_exc())
                return True

    Hook(PIPE, on_pipe, always=True)
    hooks = [Hook(a, f) for a, f in handlers.items()]
    if cfg.get("rotation"):
        for a, f in ((TEMP_PTR, on_temp_ptr), (TEMP_OTHER, on_temp_other), (TEMP_EXIT, on_temp_exit),
                     *((r, on_rot_reset) for r in ROT_RESETS)):
            hooks.append(Hook(a, f, always=True))
    exited = []
    gdb.events.exited.connect(exited.append)
    try:
        while not exited and not failed:
            try:
                gdb.execute("continue", to_string=True)
            except gdb.error:
                if exited or not inf.threads():
                    break
                raise
    except Exception:
        failed.append(traceback.format_exc())
    if failed:
        out["error"] = failed[0]
        try:
            for h in hooks:
                h.enabled = False
            gdb.execute("detach", to_string=True)
        except Exception:
            pass
    finish()


# ======================================================================================================================
# host side
# ======================================================================================================================
def host_main():
    import argparse
    from pathlib import Path
    sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
    import c2dbg

    ap = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument("source", type=Path)
    ap.add_argument("--func", action="append", default=[],
                    help="substring of the mangled name of the function(s) to show (repeatable); default: all")
    ap.add_argument("--exact", action="store_true", help="--func names are exact mangled names")
    ap.add_argument("--flags", default="", help="extra cl flags, e.g. \"/Gr /EHsc\" (BASE_FLAGS /O2 /Oy /W3 always)")
    ap.add_argument("--trace", action="store_true", help="also print every colouring step (costs, splits, re-sorts)")
    ap.add_argument("--blocks", nargs="?", const="", metavar="NAMES",
                    help="also print each block's share of each priority (or only of NAMES, comma-separated)")
    ap.add_argument("--rotation", action="store_true",
                    help="also print the expression temporaries' registers in code-generation order (the eax/ecx/edx "
                         "rotation of 0x10775acc, with the rotating pointer before each one)")
    ap.add_argument("--json", type=Path, help="write the raw trace to this file")
    ap.add_argument("--keep", action="store_true", help="keep the run directory (gdb.log, winedbg.log, the .obj)")
    ap.add_argument("--obj", type=Path, help="copy the compiled object here")
    args = ap.parse_args()

    if not args.source.exists():
        sys.exit(f"{args.source}: no such file")
    text = args.source.read_text(errors="replace")
    m = re.search(r"^// Leaf: *(\S+)", text, re.M)
    if not args.func and m and m.group(1).startswith("?"):
        args.func = [m.group(1)]          # a lane row: its mangled leaf name
        print(f"(--func from the row's // Leaf: line: {m.group(1)})")
    m = re.search(r"^// Flags: *(.*)$", text, re.M)
    if not args.flags and m:
        args.flags = " ".join(x for x in m.group(1).split() if x != "none")
        if args.flags:
            print(f"(--flags from the row's // Flags: line: {args.flags})")
    match = ["all", []] if not args.func else ["exact" if args.exact else "substr", args.func]
    cfg = {"match": match, "blocks": args.blocks is not None, "rotation": args.rotation}
    trace, log, run = c2dbg.probe(args.source, os.path.abspath(__file__), cfg, flags=args.flags.split(),
                                  keep=args.keep, obj_out=args.obj)
    if trace is None:
        sys.exit(log)
    if trace.get("error"):
        print(log)
        sys.exit(f"the trace failed:\n{trace['error']}")
    if args.json:
        args.json.write_text(json.dumps(trace, indent=1))
    if trace.get("cl_rc"):
        print(log)
        sys.exit(f"cl failed (rc {trace['cl_rc']})")
    fns = trace["functions"]
    print(f"{args.source}  ({len(fns)} function(s) traced, {trace.get('seconds')} s in gdb; cl rc {trace.get('cl_rc')})")
    if args.keep:
        print(f"run directory kept: {run}")
    if not fns:
        print("no function matched" if args.func else "c2 ran its allocator on no function (empty TU?)")
    for fn in fns:
        report(fn, text, args.trace, args.blocks)
        if args.rotation:
            print_rotation(fn, def_line(text, fn["name"]))


def def_line(text, mangled):
    """Line of the function's definition in the source (its first line), from the mangled name: the plain
    identifier, qualified by its class for members. None when not found."""
    m = re.match(r"\?(\?\d|[\w$]+)@((?:[\w$]+@)*)@", mangled or "")
    if not m:
        return None
    name, scope = m.group(1), [s for s in m.group(2).split("@") if s]
    if name.startswith("?"):
        special = {"?0": scope[0] if scope else None, "?1": "~" + scope[0] if scope else None}.get(name)
        if special is None:
            return None
        name = special
    pats = []
    if scope:
        pats.append(re.compile(r"\b" + re.escape(scope[0]) + r"\s*::\s*" + re.escape(name) + r"\s*\("))
    pats.append(re.compile(r"(?<![\w:~])" + re.escape(name) + r"\s*\("))
    lines = text.splitlines()
    for pat in pats:
        for i, l in enumerate(lines):
            s = l.split("//")[0]
            if pat.search(s) and not re.search(r";\s*$", s):
                # a definition: a '{' follows before a ';' at paren depth 0
                rest = "\n".join(lines[i:i + 12])
                depth = 0
                for ch in rest[pat.search(s).start():]:
                    if ch == "(":
                        depth += 1
                    elif ch == ")":
                        depth -= 1
                    elif ch == "{" and depth == 0:
                        return i + 1
                    elif ch == ";" and depth == 0:
                        break
    return None


class Cand:
    def __init__(self, cid, leaf, parent=None):
        self.id, self.leaf, self.parent = cid, leaf, parent
        self.root = parent.root if parent else self
        self.pieces = []
        self.refs = []          # (line, rw, tuple number)
        self.reg = None
        self.listed = False
        self.late = False
        self.dropped = None
        self.twouse = False
        self.constback = None
        self.lowspill = False
        self.nocolor = False
        self.split = False
        self.cls = None


def model(fn):
    cur, roots, phase = {}, [], "build"
    for e in fn["events"]:
        k = e["e"]
        if k == "driver":
            phase = "alloc"
        elif k == "new":
            parent = cur.get(e["parent"]) if e.get("parent") is not None else None
            c = Cand(e["id"], e["leaf"], parent)
            c.cls = e.get("cls")
            if parent:
                parent.split = True
                c.root.pieces.append(c)
            else:
                c.late = phase != "build"
                roots.append(c)
            cur[e["id"]] = c
            e["cand"] = c
        elif k == "ref":
            if e["id"] in cur:
                cur[e["id"]].refs.append((e["line"], e["rw"], e["n"]))
        elif k in ("sorted", "resort"):
            e["cands"] = [cur.get(s["id"]) for s in e["list"]]
            if k == "sorted":
                for c in e["cands"]:
                    if c:
                        c.listed = True
        elif "id" in e:
            c = e["cand"] = cur.get(e["id"])
            if c is None:
                continue
            if k == "chosen":
                c.reg = e["reg"]
            elif k == "drop":
                c.dropped = "dropped"
            elif k == "forward":
                c.twouse = True
            elif k == "lowspill":
                c.lowspill = True
            elif k == "nocolor":
                c.nocolor = True
        elif k == "split":
            for cid in e["ids"]:
                if cid in cur:
                    cur[cid].split = True
        elif k == "constback":
            for c in list(cur.values()):
                if c.reg == e["reg"] and (c.leaf or {}).get("kind") == 13:
                    c.constback = e["benefit"]
    return roots, cur


def compress(nums):
    nums = sorted(set(nums))
    parts, i = [], 0
    while i < len(nums):
        j = i
        while j + 1 < len(nums) and nums[j + 1] == nums[j] + 1:
            j += 1
        parts.append(str(nums[i]) if i == j else f"{nums[i]}-{nums[j]}")
        i = j + 1
    s = ",".join(parts)
    return s if len(s) <= 14 else f"{nums[0]}..{nums[-1]}"


def describe(c):
    if c is None:
        return "?"
    leaf = c.leaf or {}
    kind, name = leaf.get("kind"), leaf.get("name")
    t = leaf.get("type", 0)
    width = {0x1001: " (8-bit)", 0x1002: " (16-bit)", 0x1008: " (64-bit)"}.get(t, "")
    if kind == 13:
        node = leaf.get("node") or []
        val = const_value(node)
        s = "const ?" if val is None else f"const {val if -10 < val < 10 else hex(val & 0xFFFFFFFF)}"
    elif name:
        s = (name[1:] if name.startswith("_") else name) + width
    else:
        s = KINDS.get(kind, f"kind {kind}") + width
    return s if c.root is c else f"{s}, piece of #{c.root.id}"


def const_value(node):
    """A constant leaf's value node (leaf +0x28): dwords +0x18 / +0x1c hold the value's low / high dword (found by
    probing constants 1234, -77 and 0x12345678); +4 is the node opcode 0x24b."""
    if not node or len(node) < 7:
        return None
    return struct.unpack("<i", struct.pack("<I", node[6]))[0]


def allowed_str(bits):
    if bits is None:
        return "-"
    return "".join(LETTERS[r] if r in bits else "." for r in ORDER)


def regname(r):
    return REG_NAMES.get(r, f"reg{r}")


def result(c):
    if c is None:
        return "?"
    none = "immediate" if (c.leaf or {}).get("kind") == 13 else "memory"
    if not c.split:
        if c.reg and c.constback is not None:
            return f"{regname(c.reg)} -> immediates after codegen (constants only, benefit {c.constback} <= 2)"
        if c.reg:
            return regname(c.reg)
        return none + (" (no register left)" if c.nocolor else "")
    rest = [p for p in c.root.pieces if not p.split]
    got = [f"{regname(p.reg)} #{p.id}" for p in rest if p.reg]
    left = len(rest) - len(got)
    if not got:
        return f"split, {none}"
    return "split: " + ", ".join(got) + (f", {left} piece{'s' * (left > 1)} {none}" if left else "")


def report(fn, text, trace, blocks):
    roots, byid = model(fn)
    events = fn["events"]
    base = def_line(text, fn["name"])

    def lines(c):
        if not c or not c.refs:
            return ""
        ls = [r[0] for r in c.refs if r[0]]
        if not ls:
            return ""
        if base is None:
            return "+" + compress(ls)
        return compress([base - 1 + d for d in ls])

    print(f"\n{'=' * 110}\n{fn['name']}" + (f"  (defined at line {base})" if base else "  (definition line not found; lines are +N)"))
    sorts = [e for e in events if e["e"] == "sorted"]
    for s in sorts:
        cname = CLASS_NAMES.get(s["cls"], f"class {s['cls']}")
        if not s["list"]:
            if s["cls"] == 0:
                print("\nno integer register candidates reached the sorted list")
            continue
        print(f"\n{cname} candidates in c2's colouring order (0x1078482f: priority desc, then key desc; equal -> later id first).")
        print("allowed: a=eax c=ecx d=edx s=esi i=edi b=ebx p=ebp (table order; a value live across a call shows ...sibp)")
        print(f"{'#':>3} {'id':>4}  {'candidate':<24} {'lines':<14} {'prio':>6} {'key':>5} {'spill':>5} {'refs':>4}  {'allowed':<7}  register")
        for i, (snap, c) in enumerate(zip(s["list"], s["cands"])):
            print(f"{i:>3} {snap['id']:>4}  {describe(c):<24.24} {lines(c):<14} {snap['prio']:>6} {snap['key']:>5} "
                  f"{snap['spill']:>5} {snap['refs']:>4}  {allowed_str(snap.get('allowed')):<7}  {result(c)}")
    if not sorts:
        print("\nc2's allocator built no sorted list for this function (no candidates).")
    unlisted = [c for c in roots if not c.listed and not c.late]
    if unlisted:
        print("\nnot in the list (dropped before the sort: fewer than 2 references and spill <= 0, a 2-reference copy "
              "forwarded into its use,\nor not an integer candidate):")
        for c in unlisted:
            why = c.dropped or ("forwarded (2-reference copy)" if c.twouse else "") or \
                ("x87" if (c.leaf or {}).get("type", 0) >> 12 == 4 else "")
            print(f"    {c.id:>4}  {describe(c):<24.24} {lines(c):<14} refs {len(c.refs):<3} {why}")
    if trace:
        print_trace(events, byid, lines)
    if blocks is not None:
        print_blocks(events, byid, blocks, lines, base)


def print_trace(events, byid, lines):
    print("\ncolouring steps (the driver 0x107821a7 takes the list head each time):")
    for e in events:
        k = e["e"]
        c = e.get("cand")
        if k == "choose":
            print(f"  choose  #{e['id']:<4} {describe(c):<24.24} prio {e['prio']:>5}  allowed {allowed_str(e['allowed'])}")
        elif k == "chosen":
            costs = {regname(r): v for r, v in enumerate(e["costs"]) if v and r in REG_NAMES}
            print(f"  -> {regname(e['reg']):<4}  #{e['id']:<4} {describe(c):<24.24}" +
                  (f"  costs: {costs}" if costs else "  (all costs 0: first allowed register in table order)"))
        elif k == "lowspill":
            print(f"  skip    #{e['id']:<4} {describe(c):<24.24} spill {e['spill']} <= 0: not worth a register")
        elif k == "nocolor":
            print(f"  no reg  #{e['id']:<4} {describe(c):<24.24} prio {e['prio']}: every allowed register taken -> memory / split")
        elif k == "split":
            print(f"  split pass over ids {e['ids']}")
        elif k == "new" and e.get("parent") is not None:
            print(f"  piece   #{e['id']:<4} of #{e['parent']}")
        elif k == "resort":
            print("  re-sort: " + ", ".join(f"#{s['id']}({s['prio']})" for s in e["list"]))
        elif k == "drop":
            print(f"  drop    #{e['id']:<4} {describe(c):<24.24} refs {e['refs']} spill {e['spill']}")
        elif k == "forward":
            print(f"  2-ref   #{e['id']:<4} {describe(c):<24.24} checked for copy forwarding" +
                  (" -> forwarded, not listed" if c is not None and not c.listed else ""))
        elif k == "end":
            print("  end of allocation")
        elif k == "constback":
            print(f"  after codegen: {regname(e['reg'])} held only constants with benefit {e['benefit']} <= 2 -> given back, immediates")


def print_rotation(fn, base):
    temps = [e for e in fn["events"] if e["e"] in ("temp", "rotreset")]
    if not temps:
        print("\nno expression temporaries were given registers for this function (or --rotation hooks saw none)")
        return
    print("\nexpression temporaries in code-generation order (0x10775acc): the rotating pointer's register before the")
    print("choice, the register given, and the rule: rotation = next free of eax/ecx/edx from the pointer (the pointer")
    print("then moves past it); first free / score = no scratch register free, table order; hint / other.")
    print(f"  {'line':>6}  {'ptr at':<7} {'got':<4} rule")
    for e in temps:
        if e["e"] == "rotreset":
            print("          (pointer reset to eax: new function)")
            continue
        ln = e["line"]
        ls = "" if not ln else (str(base - 1 + ln) if base else f"+{ln}")
        print(f"  {ls:>6}  {regname(e['ptr']) if e['ptr'] else '-':<7} {regname(e['reg']):<4} {e['route']}"
              + (f"  (pointer now at {regname(e['after'])})" if e.get("after") else "") + f"  op={e.get('op')}")


def print_blocks(events, byid, names, lines, base=None):
    first = next((i for i, e in enumerate(events) if e["e"] == "sorted"), None)
    if first is None:
        return
    blks = [e for e in events[:first] if e["e"] in ("blk", "blkend")]
    sort = events[first]
    label = {s["id"]: describe(c) for s, c in zip(sort["list"], sort["cands"])}
    want = None
    if names:
        want = set()
        for tok in (t.strip() for t in names.split(",") if t.strip()):
            for cid, txt in label.items():
                if tok in (f"#{cid}", str(cid), txt):
                    want.add(cid)
        if not want:
            print(f"\n--blocks: no listed candidate is called {names!r}")
            return
    print("\nwhere each priority comes from (priority pass 0x10782473, first run): per block, w = 1 << loop depth,")
    print("K = candidates referenced in the block; a referenced candidate gains w*K*cost, one live through without a")
    print("reference loses w*K.  cost: 2 per variable reference in the block, 0/1 for a constant.")
    shares = {}
    nblk = 0
    cur = None
    for e in blks:
        if e["e"] == "blk":
            cur = e
            continue
        w, wk = e["w"], e["wK"]
        K = wk // w if w else 0
        nblk += 1
        refl_ids = [x[0] for x in e["refl"]]
        live_ids = [x[0] for x in e["live"]]
        bl = [base - 1 + d for d in cur["lines"]] if (cur and cur["lines"] and base) else (cur["lines"] if cur else [])
        bls = ("" if base else "+") + compress(bl) if bl else "-"
        lab = f"block {nblk} (depth {cur['depth'] if cur else '?'}, lines {bls}): w {w} K {K}"
        print(f"  {lab}")
        for cid, f6, cost, rn in e["refl"]:
            if rn == 0:
                shares.setdefault(cid, []).append((nblk, wk * cost))
                if want is None or cid in want:
                    print(f"      #{cid:<4} {label.get(cid, '?'):<22.22} referenced, cost {cost:>3}: {wk * cost:+d}")
        for cid, f6, cost, rn in e["live"]:
            if f6 & 0x10:
                shares.setdefault(cid, []).append((nblk, wk * cost))
                if want is None or cid in want:
                    print(f"      #{cid:<4} {label.get(cid, '?'):<22.22} referenced, cost {cost:>3}: {wk * cost:+d}")
            elif wk:
                shares.setdefault(cid, []).append((nblk, -wk))
                if want is None or cid in want:
                    print(f"      #{cid:<4} {label.get(cid, '?'):<22.22} live through, no reference: {-wk:+d}")
    print("  totals:")
    for s in sort["list"]:
        cid = s["id"]
        if want is not None and cid not in want:
            continue
        tot = sum(v for _, v in shares.get(cid, []))
        flag = "" if tot == s["prio"] else f"   (table says {s['prio']}: a later pass changed it)"
        print(f"      #{cid:<4} {label.get(cid, '?'):<22.22} {tot:>6}{flag}")


if gdb is not None:
    tracer()
elif __name__ == "__main__":
    host_main()
