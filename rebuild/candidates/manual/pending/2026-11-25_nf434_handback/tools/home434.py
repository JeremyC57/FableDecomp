#!/usr/bin/env python3
"""home434.py <wd> <rows.txt> [--skip-leaf]: retail homes for every .text COMDAT a row object defines other than its leaf and
anonymous-namespace scaffolding: reloc-masked body search over retail .text (R1 tier 2), then each relocation target at the match
checked against reloc_bindings / landed selves (REL32 call / jmp, DIR32 absolute). Prints <addr> <sym> <len> <matches> <home> <relocs>."""
import sys, struct, re, glob, os
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
src = open(os.path.dirname(os.path.abspath(__file__)) + "/objcmp.py").read().split("A=load")[0]; exec(src)
exe = open(os.environ["FABLE_EXE"], "rb").read()
pe = struct.unpack_from("<I", exe, 0x3c)[0]; ns = struct.unpack_from("<H", exe, pe + 6)[0]; op = struct.unpack_from("<H", exe, pe + 20)[0]
SS = [struct.unpack_from("<IIII", exe, pe + 24 + op + 40 * i + 8) for i in range(ns)]
vs, rva, rs, ro = SS[0]; text = exe[ro:ro + rs]; tbase = 0x400000 + rva
tab = {}
for l in open("/home/user/FableDecomp/rebuild/oracles/reloc_bindings.tsv", errors="replace").read().splitlines()[1:]:
    p = l.split("\t"); tab.setdefault(p[0], set()).add(int(p[1], 16))
for l in open("/home/user/FableDecomp/rebuild/oracles/self_symbols.tsv").read().splitlines()[1:]:
    p = l.split("\t")
    if len(p) > 1 and p[1]: tab.setdefault(p[1], set()).add(int(p[0], 16))
wd, rl = sys.argv[1:3]
for a in open(rl).read().split():
    o = glob.glob(f"{wd}/**/{a}/{a}.obj", recursive=True)[0]
    secs = load(o)[0]
    leaf = None
    srcs = glob.glob(f"{wd}/**/{a}/{a}.cpp", recursive=True)
    if srcs:
        m = re.search(r"^// Leaf:\s*(\S+)", open(srcs[0], errors="replace").read(), re.M); leaf = m.group(1) if m else None
    for (name, sym), (data, rel, ch) in sorted(secs.items(), key=lambda x: x[0][1]):
        if name != ".text" or not sym or sym == leaf or "?A0x" in sym: continue
        pat = bytearray(data); mask = set()
        for off, s, typ in rel:
            for k in range(4): mask.add(off + k)
        rx = b"".join(b"." if i in mask else re.escape(bytes([pat[i]])) for i in range(len(pat)))
        hits = [m.start() for m in re.finditer(rx, text, re.S) if (m.start() + tbase) % 16 == 0]
        res = []
        for h in hits:
            va = tbase + h; ok = True
            for off, s, typ in rel:
                r = struct.unpack_from("<I", text, h + off)[0]
                tgt = (va + off + 4 + struct.unpack("<i", struct.pack("<I", r))[0]) & 0xffffffff if typ == 20 else r
                want = tab.get(s)
                if want is not None and tgt not in want: ok = False
            res.append((va, ok))
        eqs = [va for va, ok in res if ok]
        home = ("%08x" % eqs[0]) if len(eqs) == 1 else ("set:%d" % len(eqs) if eqs else "NONE")
        print("%s\t%s\t%d\tmasked=%d\tEQ_home=%s" % (a, sym, len(data), len(hits), home))
