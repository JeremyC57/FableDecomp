# emit.py <pairs.tsv> <tag-regex>: per open queue row: target -> {symbol: [rows]} from compiled pairs, marking GOOD/BAD
import sys, os, re, collections
S = os.environ["S"]; W = S + "/wave34"
em = collections.defaultdict(lambda: collections.defaultdict(set))
for p in sys.argv[1].split(","):
    for l in open(p):
        q = l.rstrip("\n").split("\t")
        if len(q) >= 3 and q[1] not in ("NOOBJ", "ERR"): em[q[1]][q[2]].add(q[0])
tag = re.compile(sys.argv[2])
for l in open(W + "/triage.tsv"):
    i, ch, t, st, good, bad = l.rstrip("\n").split("\t")
    if not tag.fullmatch(ch): continue
    print(f"=== {i} {ch} {t} GOOD {good[:150]}")
    for s, rows in sorted(em.get(t, {}).items()):
        print(f"   {'ok ' if s == good else 'BAD'} {s[:170]}  <- {' '.join(sorted(rows)[:15])}{' +%d' % (len(rows)-15) if len(rows) > 15 else ''}")
