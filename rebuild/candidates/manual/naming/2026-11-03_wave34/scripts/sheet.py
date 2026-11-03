# sheet.py <tag-regex> : worksheet for open PRIORITY rows whose chunk matches: target, good, each wrong spelling with the landed
# xref binders whose source text contains all its name components (likely emitters)
import re, sys, os, collections
S = os.environ["S"]; W = S + "/wave34"
rb = collections.defaultdict(list)
for l in open("rebuild/oracles/reloc_bindings.tsv").read().splitlines()[1:]:
    s, t, a = l.split("\t")[:3]; rb[t].append((s, a))
bind = collections.defaultdict(set)
for l in open(W + "/binders_all.tsv"):
    q = l.rstrip("\n").split("\t")
    if q[4] == "LANDED": bind[q[0]].add((q[3], q[5]))
srccache = {}
def src(p):
    if p not in srccache:
        try: srccache[p] = open("rebuild/src/compiled/" + p, encoding="utf-8", errors="replace").read()
        except OSError: srccache[p] = ""
    return srccache[p]
def comps(sym):
    if not sym.startswith("?"): return [sym.lstrip("_@").split("@")[0]]
    s = sym[1:]
    if s.startswith("?$"): s = s[2:]
    elif s.startswith("?_"): s = s[3:]
    elif s.startswith("?"): s = s[2:]
    elif s.startswith("$"): s = s[1:]
    head = s.split("@@")[0]
    parts = [p for p in head.split("@") if p and not p.startswith("?$") and not p.startswith("$")]
    return [re.sub(r"^\?\$", "", p) for p in parts if len(p) > 1]
tag = re.compile(sys.argv[1])
for l in open(W + "/triage.tsv"):
    i, ch, t, st, good, bad = l.rstrip("\n").split("\t")
    if not tag.fullmatch(ch): continue
    b = bind.get(t, set())
    print(f"=== {i} {ch} {t} {st} landed_xref={len(b)}\n  GOOD {good}")
    for s, a in rb.get(t, []):
        if s == good: print(f"   good-bound first={a}"); continue
        c = comps(s)
        hits = sorted(r for r, p in b if c and all(x in src(p) for x in c))
        print(f"  BAD {s}  first={a}  comps={c}\n      emitters?={' '.join(hits[:12])}{' ...+%d' % (len(hits)-12) if len(hits)>12 else ''}")
        if len(sys.argv) > 2: open(sys.argv[2], "a").write("\n".join(hits + [a]) + "\n")
