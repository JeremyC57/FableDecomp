#!/usr/bin/env python3
"""Recursive-descent extent scanner for retail Fable.exe (VC7.1, ImageBase 0x400000).

    python3 extent.py <addr> [<addr> ...]        -> print extent + evidence per address

Library use: E = Exe(); r = E.scan(va) -> dict(extent, code_end, tables, insns, flags ...)

From the start: follow fallthrough, jcc targets, internal jmp targets, `jmp [reg*4+T]` jump
tables (entry count from the preceding `cmp idx, N / ja`, incl. VC7.1's two-level
`movzx r, byte [x+T2]` index table), stopping at ret / tail-jmp / int3 / hlt and at calls to
noreturn targets.  Noreturn targets = noreturn IAT imports + data-driven: call targets whose
call sites are (mostly) followed by int3 padding or a known function start.  A call followed
directly by 0xCC also ends the path (VC keeps the int3 after a noreturn call).
Extent = end of the furthest reached instruction or inline table.
"""
import bisect, csv, os, re, struct, sys
from collections import defaultdict
import numpy as np
from capstone import Cs, CS_ARCH_X86, CS_MODE_32

ROOT = "/home/user/FableDecomp"
EXE = os.environ.get("FABLE_EXE") or (os.environ["S"] + "/dl/game/Fable The Lost Chapters/Fable.exe")
NORET_IMPORTS = {"ExitProcess", "ExitThread", "FatalAppExitA", "FatalAppExitW", "FatalExit",
                 "_exit", "exit", "abort", "_amsg_exit", "_CxxThrowException", "terminate",
                 "RtlExitUserThread", "_invalid_parameter_noinfo_noreturn", "longjmp", "_longjmp"}
FILLER = {0xCC, 0x90}
FILLSEQ = [bytes.fromhex(h) for h in ("8da42400000000", "8d9b00000000", "8da42400", "8d642400", "8d4900", "8bff", "90", "cc")]


def is_filler(b):
    """int3/nop padding or VC7.1's multi-byte alignment fillers (mov edi,edi / lea r,[r])."""
    i = 0
    while i < len(b):
        for f in FILLSEQ:
            if b.startswith(f, i): i += len(f); break
        else:
            return False
    return True
JMPTAB = re.compile(r"dword ptr \[(?:(\w+) \+ )?(\w+)\*4 \+ (0x[0-9a-f]+)\]$")
MOVZXB = re.compile(r"^(\w+), byte ptr \[(\w+) \+ (0x[0-9a-f]+)\]$")
CMPIMM = re.compile(r"^(\w+), (0x[0-9a-f]+|\d+)$")
IATCALL = re.compile(r"^dword ptr \[(0x[0-9a-f]+)\]$")

R32 = {"al": "eax", "ax": "eax", "bl": "ebx", "bx": "ebx", "cl": "ecx", "cx": "ecx", "dl": "edx", "dx": "edx",
       "si": "esi", "di": "edi", "bp": "ebp"}
def r32(r): return R32.get(r, r)


def load_starts():
    st = set()
    for r in csv.DictReader(open(ROOT + "/rebuild/manifest/functions.tsv", encoding="utf-8-sig"), delimiter="\t"):
        try: st.add(int(r["address"], 16))
        except Exception: pass
    return st


class Exe:
    def __init__(self, path=EXE, starts=None):
        d = open(path, "rb").read(); self.d = d
        pe = struct.unpack_from("<I", d, 0x3c)[0]
        n = struct.unpack_from("<H", d, pe + 6)[0]; opt = struct.unpack_from("<H", d, pe + 20)[0]
        self.base = struct.unpack_from("<I", d, pe + 24 + 28)[0]
        self.secs = []
        for i in range(n):
            o = pe + 24 + opt + 40 * i
            name = d[o:o + 8].rstrip(b"\0").decode()
            vs, va, rs, raw = struct.unpack_from("<IIII", d, o + 8)
            self.secs.append((name, self.base + va, vs, raw, rs))
        t = [s for s in self.secs if s[0] == ".text"][0]
        self.tlo, self.thi = t[1], t[1] + min(t[2], t[4])
        self.text = d[t[3]:t[3] + min(t[2], t[4])]
        self.md = Cs(CS_ARCH_X86, CS_MODE_32)
        self.iat = self._imports(pe)
        self.starts = starts if starts is not None else load_starts()
        self.sstarts = sorted(self.starts)
        self._xrefs()
        self._noreturn()
        self.cache = {}

    # ---- PE helpers
    def off(self, va):
        for name, v, vs, raw, rs in self.secs:
            if v <= va < v + max(vs, rs):
                return raw + va - v if va - v < rs else None
        return None

    def read(self, va, n):
        o = self.off(va)
        return self.d[o:o + n] if o is not None else b""

    def dword(self, va):
        b = self.read(va, 4)
        return struct.unpack("<I", b)[0] if len(b) == 4 else None

    def _imports(self, pe):
        d = self.d
        imp = struct.unpack_from("<I", d, pe + 24 + 104)[0]
        names = {}
        o = self.off(self.base + imp)
        while o is not None:
            oft, _, _, nm, ft = struct.unpack_from("<IIIII", d, o)
            if ft == 0: break
            t = self.off(self.base + (oft or ft)); a = ft
            while True:
                e = struct.unpack_from("<I", d, t)[0]
                if e == 0: break
                if not e & 0x80000000:
                    so = self.off(self.base + e)
                    names[self.base + a] = d[so + 2:so + 80].split(b"\0")[0].decode("latin1")
                t += 4; a += 4
            o += 20
        return names

    def _xrefs(self):
        """rel32 call/jmp targets inside .text and absolute dword refs (data + text imm)."""
        t = np.frombuffer(self.text, dtype=np.uint8)
        n = len(t) - 5
        rel = (t[1:n + 1].astype(np.int64) | (t[2:n + 2].astype(np.int64) << 8) |
               (t[3:n + 3].astype(np.int64) << 16) | (t[4:n + 4].astype(np.int64) << 24))
        rel = np.where(rel >= 1 << 31, rel - (1 << 32), rel)
        src = np.arange(n, dtype=np.int64) + self.tlo
        tgt = src + 5 + rel
        ok = (tgt >= self.tlo) & (tgt < self.thi)
        self.call_src = defaultdict(list); self.jmp_src = defaultdict(list)
        starr = np.array(self.sstarts, dtype=np.int64)
        isstart = np.isin(tgt, starr)
        for op, dst in ((0xE8, self.call_src), (0xE9, self.jmp_src)):
            m = ok & (t[:n] == op)
            for s, g in zip(src[m].tolist(), tgt[m].tolist()):
                dst[g].append(s)
        # absolute refs anywhere in the file to a .text VA
        self.abs_src = defaultdict(list)   # target -> [(section, src_va)]
        for name, v, vs, raw, rs in self.secs:
            b = np.frombuffer(self.d[raw:raw + rs], dtype=np.uint8)
            if len(b) < 4: continue
            m = len(b) - 3
            w = (b[:m].astype(np.uint32) | (b[1:m + 1].astype(np.uint32) << 8) |
                 (b[2:m + 2].astype(np.uint32) << 16) | (b[3:m + 3].astype(np.uint32) << 24))
            sel = (w >= self.tlo) & (w < self.thi)
            if name != ".text":
                sel &= (np.arange(m) % 4 == 0)   # data refs (vtables, EH tables) are dword aligned
            for i, g in zip(np.nonzero(sel)[0].tolist(), w[sel].tolist()):
                self.abs_src[g].append((name, v + i))
        self.isstart_arr = None

    def _noreturn(self):
        self.noret = set(a for a, n in self.iat.items() if n in NORET_IMPORTS or n.lstrip("_") in NORET_IMPORTS)
        self.noret_fn = set()
        S = self.starts
        for g, srcs in self.call_src.items():
            if g not in S or len(srcs) < 2: continue
            k = 0
            for s in srcs:
                nx = s + 5
                b = self.text[nx - self.tlo] if nx < self.thi else 0xCC
                if b == 0xCC or (nx in S): k += 1
            if k >= 2 and k >= 0.75 * len(srcs):
                self.noret_fn.add(g)
        # thunks `jmp [IAT noreturn]`
        for g in list(S):
            b = self.read(g, 6)
            if len(b) == 6 and b[0] == 0xFF and b[1] == 0x25 and struct.unpack("<I", b[2:])[0] in self.noret:
                self.noret_fn.add(g)

    def is_start(self, a): return a in self.starts

    def solid_start(self, t):
        """a jmp to t is a tail call when t is a real entry: it is called, or referenced from
        data / a push imm (vtable, callback), or is not a manifest start but is called."""
        if self.call_src.get(t): return True
        if t not in self.starts: return False
        for sec, s in self.abs_src.get(t, ()):
            if sec != ".text": return True
            if self.text[s - 1 - self.tlo] == 0x68: return True
        return False

    def after_terminator(self, t):
        """the bytes just before t end in int3 padding, ret, ret imm16, jmp rel8/rel32."""
        b = self.read(t - 5, 5)
        if len(b) < 5: return False
        return b[4] in (0xCC, 0xC3) or b[2] == 0xC2 or b[0] == 0xE9 or b[3] == 0xEB

    def hit_kind(self, r, a):
        """how scan r reaches address a: straddle / branch / fallthrough / after_call / None."""
        seen = r["seen"]
        for x, sz in seen.items():
            if x < a < x + sz: return "straddle"
        if a not in seen: return None
        kind = None
        for x, sz in seen.items():
            ins = self.cache.get(x)
            if not ins: continue
            mn, op = ins[2], ins[3]
            if (mn.startswith("j") or mn.startswith("loop")) and op == hex(a): return "branch"
            if x + sz == a:
                kind = "after_call" if mn == "call" else ("after_term" if mn in ("ret", "jmp", "int3") else "fallthrough")
        if kind in (None, "after_term"):
            for v, l in r["tables"]:
                for k in range(0, l, 4):
                    if self.dword(v + k) == a: return "branch"
        return kind or "root"

    def seh_handlers(self, fstart, window=0x8000):
        """__except filter/handler and __finally addresses from an SEH scope table pushed in the
        prologue (`push -1; push offset scopetable; push offset __except_handler3` or
        `push size; push offset scopetable; call __SEH_prolog`)."""
        out = []
        a = fstart
        for _ in range(8):
            ins = self.insn(a)
            if ins is None: break
            _, sz, mn, op = ins
            if mn == "push" and op.startswith("0x"):
                st = int(op, 16)
                if not (self.tlo <= st < self.thi) and self.off(st) is not None:
                    got = []
                    for k in range(16):
                        b = self.read(st + 12 * k, 12)
                        if len(b) < 12: break
                        enc, flt, hnd = struct.unpack("<iII", b)
                        inwin = lambda v: fstart < v < fstart + window
                        if not (-1 <= enc < 64) or not inwin(hnd) or not (flt == 0 or inwin(flt)): break
                        got += [x for x in (flt, hnd) if x]
                    if got:
                        out += got; break
            if mn in ("ret", "jmp"): break
            if mn == "call" and out: break
            a += sz
        return out

    def eh_handlers(self, fstart):
        """catch-handler addresses from the C++ EH FuncInfo named by the prologue's
        `push -1; push offset __ehhandler$...` (thunk = mov eax, FuncInfo; jmp ___CxxFrameHandler)."""
        out = []
        a = fstart
        for _ in range(8):
            ins = self.insn(a)
            if ins is None: break
            _, sz, mn, op = ins
            if mn == "push" and op.startswith("0x"):
                h = int(op, 16)
                b = self.read(h, 10)
                if len(b) == 10 and b[0] == 0xB8 and b[5] == 0xE9:
                    fi = struct.unpack("<I", b[1:5])[0]
                    v = self.read(fi, 20)
                    if len(v) == 20:
                        magic, maxs, pun, ntry, ptry = struct.unpack("<5I", v)
                        if magic in (0x19930520, 0x19930521, 0x19930522) and ntry < 256:
                            for i in range(ntry):
                                e = self.read(ptry + 20 * i, 20)
                                if len(e) < 20: break
                                lo, hi, ch, nc, pha = struct.unpack("<iiiiI", e)
                                for j in range(min(nc, 64)):
                                    hb = self.read(pha + 16 * j, 16)
                                    if len(hb) == 16: out.append(struct.unpack("<4I", hb)[3])
                    break
            if mn in ("ret", "jmp", "call"): break
            a += sz
        return out

    def next_start(self, a):
        i = bisect.bisect_right(self.sstarts, a)
        return self.sstarts[i] if i < len(self.sstarts) else None

    def entry_candidates(self, a, back=0x4000, n=16):
        """possible entries before a, nearest first: manifest starts and post-int3-padding addresses."""
        out = set()
        i = bisect.bisect_left(self.sstarts, a)
        k = i - 1
        while k >= 0 and a - self.sstarts[k] < back and len(out) < n:
            out.add(self.sstarts[k]); k -= 1
        lo = max(self.tlo + 1, a - back)
        o = a - self.tlo
        j = o - 1
        cnt = 0
        while j > lo - self.tlo and cnt < n:
            if self.text[j - 1] == 0xCC and self.text[j] != 0xCC and (self.text[j - 2] in (0xCC, 0xC3) or self.text[j - 4] == 0xC2):
                out.add(self.tlo + j); cnt += 1
            j -= 1
        return sorted(out, reverse=True)

    def prev_start(self, a):
        i = bisect.bisect_left(self.sstarts, a)
        return self.sstarts[i - 1] if i > 0 else None

    def insn(self, a):
        c = self.cache.get(a)
        if c is None:
            if not (self.tlo <= a < self.thi): return None
            o = a - self.tlo
            c = next(self.md.disasm_lite(self.text[o:o + 16], a, 1), None)
            self.cache[a] = c
        return c

    def xref_kinds(self, a):
        k = defaultdict(int)
        if self.call_src.get(a): k["call"] = len(self.call_src[a])
        if self.jmp_src.get(a): k["jmp"] = len(self.jmp_src[a])
        for sec, s in self.abs_src.get(a, ()):
            if sec == ".text":
                op = self.text[s - 1 - self.tlo]
                k["imm_mov_eax" if op == 0xB8 else ("imm_push" if op == 0x68 else "imm_text")] += 1
            else:
                k["data_" + sec] += 1
        return dict(k)

    # ---- jump tables
    def _table(self, fstart, trace, idx, base_reg, T):
        """-> (targets, [(va,len) tables], how)."""
        bound = None; two = None; how = ""
        for (a, sz, mn, op) in reversed(trace[-12:]):
            if mn == "movzx" and two is None:
                m = MOVZXB.match(op)
                if m and r32(m.group(1)) == idx:
                    two = (int(m.group(3), 16), m.group(2)); idx = m.group(2); continue
            if mn == "cmp" and bound is None:
                m = CMPIMM.match(op)
                if m and r32(m.group(1)) == idx:
                    bound = int(m.group(2), 0); break
            if mn in ("mov", "movzx", "movsx", "lea", "add", "sub", "dec", "inc", "and") and op.split(",")[0].strip() == idx:
                # index derived from another register; keep going but accept a cmp on its source
                srcop = op.split(",", 1)[1].strip() if "," in op else ""
                if re.fullmatch(r"e\w\w", srcop) and mn in ("mov",): idx = srcop
        tables = []; targets = []
        limit_hi = fstart + 0x20000
        def ok(v): return v is not None and fstart <= v < T   # VC7.1 emits the tables after the case code
        if two is not None:
            T2 = two[0]
            n2 = bound + 1 if bound is not None and bound < 4096 else None
            if n2 is None:
                n2 = 0
                while n2 < 256 and self.read(T2 + n2, 1) and (T2 + n2) not in self.starts and self.read(T2 + n2, 1)[0] < 64:
                    n2 += 1
                how = "2lvl-heur"
            else:
                how = "2lvl-cmp"
            bt = self.read(T2, n2)
            n1 = (max(bt) + 1) if bt else 0
            tables.append((T2, n2))
        elif bound is not None and bound < 4096:
            n1 = bound + 1; how = "cmp"
        else:
            n1 = None; how = "heur"
        ents = []
        if n1 is None:
            k = 0; nent = 0
            while k < 512:
                v = self.dword(T + 4 * k)
                if k and (T + 4 * k) in self.starts: break
                if v == 0:   # hole left by an __assume(0)/unreachable case; keep it only if a valid entry follows
                    k += 1; continue
                if not ok(v): break
                ents.append(v); k += 1; nent = k
        else:
            nent = 0
            for k in range(n1):
                v = self.dword(T + 4 * k)
                if v == 0: continue
                if not ok(v): how += "-short"; break
                ents.append(v); nent = k + 1
        tables.append((T, 4 * nent))
        return ents, tables, how

    # ---- the scan
    def scan(self, fstart, cap=0x20000):
        key = (fstart, cap)
        if not hasattr(self, "_scans"): self._scans = {}
        r = self._scans.get(key)
        if r is None:
            r = self._scans[key] = self._scan(fstart, cap)
        return r

    def _scan(self, fstart, cap=0x20000):
        S = self.starts
        seen = {}          # addr -> size
        tables = []
        flags = defaultdict(list)
        work = [(fstart, [])]
        queued = {fstart}
        for h in self.seh_handlers(fstart):
            if fstart < h <= fstart + cap and h not in queued:
                flags["seh_scope"].append(h); queued.add(h); work.append((h, []))
        ehs = self.eh_handlers(fstart)
        has_eh = bool(ehs)   # only the parent (EH prologue) owns the catch continuations
        for h in ehs:
            if fstart < h <= fstart + cap and h not in queued:
                flags["eh_catch"].append(h); queued.add(h); work.append((h, []))
        while work:
            a, trace = work.pop()
            trace = list(trace)
            while True:
                if a in seen: break
                if a < fstart:
                    flags["back_jump"].append(a); break
                if a - fstart > cap:
                    flags["runaway"].append(a); break
                ins = self.insn(a)
                if ins is None:
                    flags["bad_decode"].append(a); break
                _, sz, mn, op = ins
                seen[a] = sz
                trace.append(ins)
                nxt = a + sz
                if mn in ("ret", "retf", "iretd", "hlt", "ud2", "int3"):
                    if mn == "int3": flags["hit_int3"].append(a)
                    break
                if mn == "jmp":
                    if op.startswith("0x"):
                        t = int(op, 16)
                        if t < fstart:
                            flags["tail_back"].append(t); break
                        if t - fstart > cap or (t != fstart and (self.solid_start(t) or a == fstart or
                                                                  (t in S and self.after_terminator(t)))):
                            flags["tailcall"].append(t); break
                        if t in S and t != fstart: flags["jmp_into_start"].append(t)
                        if t not in queued: queued.add(t); work.append((t, []))
                        break
                    m = JMPTAB.match(op)
                    if m:
                        T = int(m.group(3), 16)
                        ents, tbs, how = self._table(fstart, trace, m.group(2), m.group(1), T)
                        flags["jtable"].append("%08x:%d(%s)" % (T, len(ents), how))
                        for (tv, tl) in tbs:
                            if tl: tables.append((tv, tl))
                        for v in ents:
                            if v not in queued: queued.add(v); work.append((v, []))
                        break
                    flags["indirect_jmp" if "[" not in op or "*" in op else "vjmp"].append(a); break
                if mn.startswith("j") or mn.startswith("loop") or mn == "jecxz":
                    if op.startswith("0x"):
                        t = int(op, 16)
                        if t < fstart: flags["jcc_back"].append(t)
                        elif t not in queued: queued.add(t); work.append((t, list(trace)))
                    a = nxt; continue
                if mn == "call":
                    t = None
                    if op.startswith("0x"):
                        t = int(op, 16)
                        if t in self.noret_fn:
                            flags["noret_call"].append(t); break
                    else:
                        m = IATCALL.match(op)
                        if m and int(m.group(1), 16) in self.noret:
                            flags["noret_iat"].append(self.iat[int(m.group(1), 16)]); break
                    if nxt < self.thi and self.text[nxt - self.tlo] == 0xCC:
                        flags["call_then_int3"].append(t if t is not None else op); break
                    if nxt in S and self.solid_start(nxt):
                        flags["call_then_start"].append(nxt); break
                    a = nxt; continue
                if mn == "mov" and op.startswith("eax, 0x"):
                    # catch block epilogue `mov eax, offset $continuation; ret`
                    t = int(op[5:], 16)
                    n2 = self.insn(nxt)
                    if n2 and n2[2] == "ret" and fstart < t <= fstart + cap and t not in queued and has_eh:
                        flags["eh_continuation"].append(t); queued.add(t); work.append((t, []))
                if nxt in S and nxt != fstart:
                    flags["fell_into_start"].append(nxt)
                a = nxt
        code_end = max((a + s for a, s in seen.items()), default=fstart)
        tab_end = max((v + l for v, l in tables if v >= fstart), default=fstart)
        ext = max(code_end, tab_end) - fstart
        # gaps inside [fstart, fstart+ext) that are neither reached code nor tables
        cov = bytearray(ext)
        for a, s in seen.items():
            for i in range(a - fstart, min(a - fstart + s, ext)): cov[i] = 1
        for v, l in tables:
            for i in range(v - fstart, min(v - fstart + l, ext)):
                if i >= 0: cov[i] = 1
        gap = 0; gap_nonfill = 0; i = 0
        while i < ext:
            if cov[i]: i += 1; continue
            j = i
            while j < ext and not cov[j]: j += 1
            gap += j - i
            if not is_filler(self.text[fstart + i - self.tlo:fstart + j - self.tlo]): gap_nonfill += j - i
            i = j
        return {"start": fstart, "extent": ext, "code_end": code_end - fstart, "tab_end": tab_end - fstart,
                "tables": tables, "flags": dict(flags), "ninsn": len(seen), "gap": gap,
                "gap_nonfill": gap_nonfill, "seen": seen}

    def padded(self, va, n):
        return is_filler(self.read(va, n))


def fmt(r):
    f = r["flags"]
    bits = []
    for k in ("jtable", "seh_scope", "eh_catch", "eh_continuation", "jmp_into_start", "tailcall", "noret_call", "noret_iat", "call_then_int3", "call_then_start", "fell_into_start",
              "back_jump", "jcc_back", "tail_back", "indirect_jmp", "runaway", "bad_decode", "hit_int3"):
        if k in f:
            v = f[k]
            bits.append("%s=%s" % (k, ",".join((("%08x" % x) if isinstance(x, int) else str(x)) for x in v[:4])
                                    + ("..." if len(v) > 4 else "")))
    return "; ".join(bits)


if __name__ == "__main__":
    E = Exe()
    for x in sys.argv[1:]:
        a = int(x, 16)
        r = E.scan(a)
        ns = E.next_start(a)
        print("%08x extent=%d code_end=%d tab_end=%d gap=%d/%d next_start=%s(+%d) xrefs=%s | %s" % (
            a, r["extent"], r["code_end"], r["tab_end"], r["gap"], r["gap_nonfill"],
            ("%08x" % ns) if ns else "-", (ns - a) if ns else 0, E.xref_kinds(a), fmt(r)))
