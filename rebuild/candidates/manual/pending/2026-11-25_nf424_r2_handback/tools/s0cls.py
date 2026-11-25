#!/usr/bin/env python3
"""s0cls.py: class attribution for S0 rows that sit in a vftable.
Source V (vftable siblings): slot functions referenced by exactly one .rdata dword (i.e. only this vftable) carry the most-derived
class's own overrides; their landed / egor names give a class fragment.  Source P (PDB): FableWin or Ego_r name sets hold
??_G / ??_E / ??1 of that fragment with a virtual (UAE) dtor.  Source C (ctor): the vptr-storing function's name holds the
fragment (??0frag, or CreateActiveEntityScript / Alloc of the bound class).  Source D (dtor callee): the row calls ??1frag.
Writes r2/work/s0cls.tsv."""
import sys, json, re, struct
from collections import Counter, defaultdict
A = "/tmp/claude-0/-home-user-FableDecomp/40effb6c-294b-5111-9a88-ca0c32f095ec/scratchpad/w22/agentNF424/"
R = "/home/user/FableDecomp/"
sys.path.insert(0, A + "tools"); import nf
secs = nf.exe(); rd = [s for s in secs if s[0] == ".rdata"][0]
cnt = Counter(struct.unpack_from("<%dI" % (len(rd[2]) // 4), rd[2][:len(rd[2]) // 4 * 4]))
a2n = json.load(open(A + "work/addr2names.json"))
pdb = defaultdict(set)
for f, tag in (("ghidra_out/egor_pdb_names.tsv", "egor"), ("ghidra_out/fablewin_pdb_names.tsv", "fwin")):
    for l in open(R + f):
        p = l.rstrip("\n").split("\t")
        if len(p) > 1: pdb[p[1]].add(tag)
PH = re.compile(r"_[0-9a-fA-F]{8}|_[0-9a-f]{6}\b|CVtable_|CHelper_|CNavButton_|@T@@|^\?\?_7|Unproven|Virtual\d|_vector_deleting|Composite|_scalar_deleting|^\?$")
def frags(name):
    """every split of a member-function mangled name into a class-scope fragment ending in '@@'"""
    if not name.startswith("?") or name.startswith("??"):
        if name.startswith("??0") or name.startswith("??1"): body = name[3:]
        else: return []
    else:
        i = name.find("@"); body = name[i + 1:]
    out = []
    for m in re.finditer("@@", body):
        out.append(body[:m.end()])
    return out
def pdbhit(fr):
    hits = []
    for k, nm in (("G", "??_G%sUAEPAXI@Z"), ("E", "??_E%sUAEPAXI@Z"), ("D", "??1%sUAE@XZ")):
        n = nm % fr
        if n in pdb: hits.append(k + ":" + "+".join(sorted(pdb[n])))
    return hits
st = defaultdict(set)
for l in open(A + "r2/work/vtstores.tsv"):
    v, f, o, d = l.split("\t"); st[v].add(f)
H = [l.split("\t") for l in open(A + "work/hist.tsv").read().splitlines()]; hist = {r[0]: dict(zip(H[0], r)) for r in H[1:]}
out = []
for l in open(A + "r2/work/s0slots.tsv"):
    q = l.rstrip("\n").split("\t"); a, vt, slot, n = q[:4]; slots = q[4:]
    vcand = Counter(); vev = defaultdict(list)
    for s in slots:
        k, rest = s.split(":", 1); fa, nm = rest.split("=", 1)
        if fa == a: continue
        if cnt[int(fa, 16)] != 1 or PH.search(nm): continue
        for fr in frags(nm):
            if pdbhit(fr): vcand[fr] += 1; vev[fr].append("%s:%s" % (k, fa))
    # ctor / store source
    cc = Counter()
    for f in st.get(vt, ()):
        for nm in a2n.get(f, []):
            if PH.search(nm): continue
            for fr in vcand:
                core = fr[:-2]
                m = re.match(r"\?\$CActiveEntityScript@(V.+)@@$", fr)
                if nm.startswith("??0" + fr) or nm.startswith("?Alloc@" + fr) or (m and nm.startswith("?CreateActiveEntityScript@?$CEntityScriptBinding@" + m.group(1) + "@@")):
                    cc[fr] += 1
    callees = hist[a]["callees"]
    best = None
    if vcand:
        # most-derived: the fragment whose PDB dtor exists and that has the most unique-slot evidence
        best = sorted(vcand, key=lambda fr: (-vcand[fr], len(fr)))[0]
    if best:
        srcs = ["V(%s)" % ",".join(vev[best])]
        srcs.append("P(%s)" % ",".join(pdbhit(best)))
        if cc[best]: srcs.append("C")
        if ("??1" + best) in callees: srcs.append("D")
        amb = [fr for fr in vcand if fr != best]
        out.append("\t".join([a, hist[a]["group"], vt, slot, n, "??_G%sUAEPAXI@Z" % best, " ".join(srcs), "amb=" + ";".join(amb) if amb else "", callees]))
    else:
        out.append("\t".join([a, hist[a]["group"], vt, slot, n, "-", "", "", callees]))
open(A + "r2/work/s0cls.tsv", "w").write("\n".join(out) + "\n")
print(Counter("named" if o.split("\t")[5] != "-" else "none" for o in out))
print(Counter((o.split("\t")[1], "named" if o.split("\t")[5] != "-" else "none", int(o.split("\t")[0],16) < 0xC89C10) for o in out))
