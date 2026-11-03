# mkbind.py <base pairs> <new pairs> <final rows> <binders_all.tsv> <out BINDINGS.tsv> : wave 34 RETIRE / ADD rows.
# RETIRE (s, t): t is a target the wave's rows reference, s is bound in reloc_bindings, no final row emits s any more, s was
#   emitted before by a final row (baseline pairs) or its recorded first binder is a final row, and no other landed binder of t
#   (xref, not rechecked here) has a source that still names s's leading identifiers (conservative KEEP otherwise).
# ADD (s, t): symbols the final rows emit that reloc_bindings does not hold.
import sys, re, collections, os
base = [l.rstrip("\n").split("\t") for l in open(sys.argv[1])]; base = [p for p in base if len(p) == 4]
new = [l.rstrip("\n").split("\t") for l in open(sys.argv[2])]; new = [p for p in new if len(p) == 4]
final = set(l.strip() for l in open(sys.argv[3]) if l.strip())
base = [p for p in base if p[0] in final]
emitted = set(p[2] for p in new)
T = set(p[1] for p in new) | set(p[1] for p in base)
before = set((p[2], p[1]) for p in base)
rb = [l.rstrip("\n").split("\t")[:3] for l in open("rebuild/oracles/reloc_bindings.tsv")][1:]
rb = [r for r in rb if len(r) == 3]
have_syms = set(r[0] for r in rb)
xb = collections.defaultdict(set)
for l in open(sys.argv[4]):
    p = l.rstrip("\n").split("\t")
    if len(p) >= 6 and p[4] == "LANDED" and p[0] in T and p[3] not in final: xb[p[0]].add(p[5])
STOP = {"std", "allocator", "QAE", "QBE", "UAE", "AAE", "IAE", "XZ", "XXZ", "__false_type", "__true_type", "less", "pair"}
def toks(s):
    ids = [t for t in re.findall(r"[A-Za-z_][A-Za-z0-9_]{2,}", s) if t not in STOP]
    return ids[:2]
src_cache = {}
known = collections.defaultdict(set)      # optional 6th arg: pairs of other landed rows (rechecked unchanged): exact emission
if len(sys.argv) > 6:
    for l in open(sys.argv[6]):
        q = l.rstrip("\n").split("\t")
        if len(q) == 4: known[q[0]].add(q[2])
    for l in open(sys.argv[6].replace("pairs.tsv", "grades.tsv")):
        q = l.split("\t")
        if len(q) > 1 and q[0] not in known and q[1] in ("MATCH", "RELOCATION_MATCH"): known[q[0]] = set()
def rowof(rel): return re.search(r"_([0-9a-f]{8})\.cpp$", rel).group(1)
def text(rel):
    if rel not in src_cache:
        try: src_cache[rel] = open("rebuild/src/compiled/" + rel, errors="replace").read()
        except OSError: src_cache[rel] = ""
    return src_cache[rel]
ret, keep = [], []
for s, t, by in rb:
    if t not in T or s in emitted or s.startswith("$L"): continue
    if not ((s, t) in before or by in final): continue
    tk = toks(s); others = [r for r in xb[t] if (s in known.get(rowof(r), ()) if rowof(r) in known else (tk and all(k in text(r) for k in tk)))]
    (keep if others else ret).append((s, t, by, others))
basepair = {(p[0], p[1]): p[2] for p in base}
baserows = set(p[0] for p in base)
import glob as _g
def origtext(a):
    fs = _g.glob(os.environ["S"] + f"/wave34/orig/rebuild/src/compiled/{a[:2]}/{a[2:4]}/*_{a}.cpp")
    return open(fs[0], errors="replace").read() if fs else None
first = {}
for a, t, s, k in new:
    if s.startswith("$L") or s in have_syms: continue
    if a in baserows:
        if basepair.get((a, t)) == s: continue          # emitted before the wave: not a respelling
    else:
        o = origtext(a); tk = toks(s)
        if o is None or (tk and all(x in o for x in tk) and not any(x in s for x in ("NUISystem", "NProfileTimer", "NProgressDisplay"))): continue
    if s not in first or int(t, 16) < int(first[s][0], 16): first[s] = (t, a)
with open(sys.argv[5], "w") as f:
    f.write("action\tsymbol\ttarget\tby_row\tnote\n")
    for s, t, by, o in ret: f.write(f"RETIRE\t{s}\t{t}\t{by}\tnaming wave 34: second spelling no landed binder emits after the respell (final recheck pairs; xref binders' sources checked)\n")
    for s, (t, a) in sorted(first.items(), key=lambda x: x[1]): f.write(f"ADD\t{s}\t{t}\t{a}\tnaming wave 34: retail spelling the respelled binder emits (PDB-proven per the queue row / STATUS)\n")
print("retire", len(ret), "keep", len(keep), "add", len(first))
for s, t, by, o in keep: print("KEEP", t, s[:100], "<-", " ".join(o[:3]))
by_t = collections.defaultdict(set)
for a, t, s, k in new:
    if not s.startswith("$L"): by_t[s].add(t)
for s, ts in by_t.items():
    if len(ts) > 1: print("MULTI-TARGET", s, sorted(ts))
