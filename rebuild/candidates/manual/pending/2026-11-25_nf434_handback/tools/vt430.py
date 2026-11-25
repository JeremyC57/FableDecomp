#!/usr/bin/env python3
"""vt430.py <wd> <rows.txt> <table.tsv>: for each row object, every emitted ??_7 COMDAT whose name is bound in <table> to a retail VA:
slot k relocation symbol -> retail address (table, else the landed self symbols) vs the retail vftable dword k.
Prints per vftable: agree / WRONG (bound to a different retail function) / unknown; RTTI-spelling check is n/a (RTTI-less vftables)."""
import sys, subprocess, glob, re, struct
sys.path.insert(0, "/tmp/claude-0/-home-user-FableDecomp/40effb6c-294b-5111-9a88-ca0c32f095ec/scratchpad/w22/agentNF427/tools"); import pe
wd, rl, tab = sys.argv[1:4]
s2a = {}
a2s = {}
for l in open(tab).read().splitlines()[1:]:
    s, t, w = l.split("\t"); s2a.setdefault(s, set()).add(int(t, 16))
for l in open("/home/user/FableDecomp/rebuild/oracles/self_symbols.tsv").read().splitlines()[1:]:
    f = l.split("\t")
    if len(f) > 1 and f[1]: s2a.setdefault(f[1], set()).add(int(f[0], 16)); a2s[int(f[0], 16)] = f[1]
def rdw(va): return struct.unpack("<I", pe.read_va(pe.EXES["retail"], va, 4))[0]
tot = {"agree": 0, "WRONG": 0, "unknown": 0}
for a in open(rl).read().split():
    objs = glob.glob(f"{wd}/**/{a}/{a}.obj", recursive=True)
    if not objs: print(a, "NOOBJ"); continue
    o = objs[0]
    t = subprocess.run(["objdump", "-t", o], capture_output=True, text=True).stdout
    secsym = {}
    for l in t.splitlines():
        m = re.search(r"\(sec +(\d+)\).*\(scl +2\).*\s(\?\?_7\S+)$", l)
        if m: secsym[int(m.group(1))] = m.group(2)
    r = subprocess.run(["objdump", "-r", o], capture_output=True, text=True).stdout
    # objdump -r prints per-section blocks; map section index by order of headers
    hdr = subprocess.run(["objdump", "-h", o], capture_output=True, text=True).stdout
    idx2name = {}
    for l in hdr.splitlines():
        m = re.match(r"\s*(\d+)\s+(\S+)\s", l)
        if m: idx2name[int(m.group(1)) + 1] = m.group(2)
    blocks = re.split(r"RELOCATION RECORDS FOR \[(.*?)\]:", r)
    named = {}
    for i in range(1, len(blocks), 2): named.setdefault(blocks[i], []).append(blocks[i + 1])
    for sec, vt in secsym.items():
        vas = s2a.get(vt)
        if not vas or len(vas) != 1: print(a, vt, "unbound"); continue
        va = next(iter(vas)); secname = idx2name.get(sec)
        # find the block for this section: the k-th block with that name (COMDAT names repeat); use all and pick by offsets
        rel = []
        for blk in named.get(secname, []):
            for l in blk.splitlines():
                m = re.match(r"([0-9a-f]{8}) \S+\s+(\S+)", l)
                if m: rel.append((int(m.group(1), 16), m.group(2)))
            if rel: break
        res = []
        for off, sym in sorted(rel):
            k = off // 4; want = rdw(va + off)
            got = s2a.get(sym)
            if not got:
                own = a2s.get(want)
                if own and own != sym and not (sym.startswith("??_E") and own == "??_G" + sym[4:]):
                    res.append("W"); tot["WRONG"] += 1; print(a, vt, "slot", k, sym, "retail %08x self %s" % (want, own))
                else: res.append("u"); tot["unknown"] += 1
            elif want in got: res.append("a"); tot["agree"] += 1
            else: res.append("W"); tot["WRONG"] += 1; print(a, vt, "slot", k, sym, "retail %08x" % want)
        print(a, vt, "%08x" % va, "".join(res))
print("TOTAL", tot)
