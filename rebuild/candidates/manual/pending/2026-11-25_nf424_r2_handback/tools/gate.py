#!/usr/bin/env python3
"""gate.py: pre-vland gates for each RELOCATION_MATCH thin row (r2/work/diag.tsv):
 ident  : non-S0 -> llvm-undname class of the leaf == retail RTTI class of the row's vftable (hist retail_cls) AND a FableWin / Ego_r
          name-set entry for the class (hist egor / fwin); S0 -> s0cls sources V + P + C (C = the vptr-storing function names the class)
 root   : owner source followed through thin includers to the root owner (address, Source, sha1)
 ownerok: root and chosen owner not under a foreign lock, an Account B newest claim, T3_PORT_QUEUE col 2/3
 flags  : owner (root) flags vs the row's own catalog flags
 oldref : the row's old self symbol (self_symbols / hist self_leaf) referenced by another row in reloc_bindings (user address)
Writes r2/work/gate.tsv."""
import sys, re, json, hashlib, subprocess, os
from collections import defaultdict
A = "/tmp/claude-0/-home-user-FableDecomp/40effb6c-294b-5111-9a88-ca0c32f095ec/scratchpad/w22/agentNF424/"
R = "/home/user/FableDecomp/"; S = "/tmp/claude-0/-home-user-FableDecomp/40effb6c-294b-5111-9a88-ca0c32f095ec/scratchpad/"
sys.path.insert(0, A + "tools"); import nf
hexre = re.compile(r"0x([0-9A-Fa-f]{8})")
H = [l.split("\t") for l in open(A + "work/hist.tsv").read().splitlines()]; hist = {r[0]: dict(zip(H[0], r)) for r in H[1:]}
gt = {l.split("\t")[0]: l.rstrip("\n").split("\t") for l in open(A + "r2/work/gen_thin.tsv")}
s0 = {l.split("\t")[0]: l.rstrip("\n").split("\t") for l in open(A + "r2/work/s0cls.tsv")}
diag = [l.rstrip("\n").split("\t") for l in open(A + "r2/work/diag.tsv")]
src2addr = {}
for m in re.finditer(r"Address = '([0-9A-Fa-f]{8})'\s*\n\s*Module = '[^']*'\s*\n\s*Source = '([^']*)'", nf.PS): src2addr.setdefault(m.group(2), m.group(1).lower())
newest = {}
for l in open(R + "rebuild/candidates/manual/COORDINATION_CLAIMS.tsv").read().splitlines()[1:]:
    q = l.split("\t")
    if len(q) < 5: continue
    for h in hexre.findall(q[2]): newest[h.lower()] = (q[1], q[4])
t3q = set()
for l in open(R + "rebuild/candidates/manual/naming/T3_PORT_QUEUE.tsv").read().splitlines()[1:]:
    q = l.split("\t")
    for col in q[1:3]:
        for h in hexre.findall(col): t3q.add(h.lower())
selfs = {}
for l in open(R + "rebuild/oracles/self_symbols.tsv").read().splitlines()[1:]:
    q = l.split("\t"); selfs[q[0]] = q[1]
users = defaultdict(set)
for l in open(R + "rebuild/oracles/reloc_bindings.tsv").read().splitlines()[1:]:
    q = l.split("\t")
    if len(q) >= 3: users[q[0]].add(q[2].lower())
def ownerbad(a):
    r = []
    p = S + "w22/nflocks/%s/owner" % a
    if os.path.exists(p):
        o = open(p).read().strip()
        if o not in ("released", "NF424", ""): r.append("lock:" + o)
    if a in newest and newest[a][0] == "B" and newest[a][1] != "released": r.append("claimB")
    if a in t3q: r.append("T3Q")
    return r
def root_of(src):
    chain = []
    while True:
        t = (nf.R / "rebuild/src/compiled" / src).read_text(encoding="utf-8")
        code = [l for l in t.splitlines() if l.strip() and not l.strip().startswith("//")]
        if len(code) == 1 and code[0].startswith('#include "../src/compiled/'):
            src = code[0].split('"')[1][len("../src/compiled/"):]; chain.append(src); continue
        return src, t, chain
def undcls(leaf):
    o = subprocess.run(["llvm-undname", leaf], capture_output=True, text=True).stdout.strip().splitlines()[-1]
    m = re.search(r"__thiscall (.+)::`scalar deleting dtor'", o); return m.group(1) if m else None
out = []
for d in diag:
    a, shape, st = d[0], d[1], d[2]
    if st != "RELOCATION_MATCH": continue
    g = gt[a]; owner, osrc, leaf = g[2], g[3], g[4]
    h = hist[a]
    # identity
    if shape == "S0":
        ev = s0[a][6]; ok = all(x in ev for x in ("V(", "P(")) and " C" in ev
        ident = ("OK " if ok else "NO ") + ev
    else:
        c = undcls(leaf); rc = h["retail_cls"]
        pdb = (h["egor"] + h["fwin"]).strip()
        ok = c is not None and c == rc and pdb != ""
        ident = ("OK " if ok else "NO ") + "rtti=%s leafcls=%s egor=%s fwin=%s" % (rc, c, h["egor"], h["fwin"])
    rsrc, rtext, chain = root_of(osrc)
    raddr = src2addr.get(rsrc, "?")
    sha = hashlib.sha1(rtext.encode("utf-8")).hexdigest()[:12]
    bad = ownerbad(owner) + (ownerbad(raddr) if raddr != owner else [])
    ofl = nf.flags_of(raddr if raddr != "?" else owner, rtext)
    rfl = nf.field(a, "CompilerFlags")
    rfl = [x for x in rfl.split() if x not in ("/O2", "/Oy", "/W3")] if rfl else []
    flg = "SAME" if ofl == rfl else "CHANGE row=%s owner=%s" % (" ".join(rfl) or "-", " ".join(ofl) or "-")
    old = selfs.get(a) or h["self_leaf"]
    ou = sorted(u for u in users.get(old, ()) if u != a)
    out.append("\t".join([a, shape, leaf, ident, owner, raddr, rsrc, sha, ">".join(chain) or "-", ",".join(bad) or "-", flg, old, ",".join(ou) or "-"]))
open(A + "r2/work/gate.tsv", "w").write("address\tshape\tleaf\tident\towner\troot\troot_src\troot_sha1\tchain\towner_excl\tflags\told_self\told_users\n" + "\n".join(out) + "\n")
from collections import Counter
print(len(out), Counter(o.split("\t")[3][:2] for o in out), Counter(o.split("\t")[9] != "-" for o in out), Counter(o.split("\t")[10][:6] for o in out), Counter(o.split("\t")[12] != "-" for o in out), Counter(o.split("\t")[8] != "-" for o in out))
