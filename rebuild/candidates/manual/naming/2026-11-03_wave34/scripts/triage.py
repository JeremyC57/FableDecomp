# triage.py: for every open PRIORITY queue row (+ SECOND_SPELLINGS queued_correct_open targets), list the bound spellings of its target
import re, sys, collections
Q = "rebuild/candidates/manual/naming/NAME_FIXES_QUEUE.tsv"
rb = collections.defaultdict(list)
for l in open("rebuild/oracles/reloc_bindings.tsv").read().splitlines()[1:]:
    s, t, a = l.split("\t")[:3]; rb[t.lower()].append((s, a))
rows = []
for i, l in enumerate(open(Q).read().splitlines(), 1):
    q = l.split("\t")
    if len(q) < 4 or "PRIORITY" not in l or " done]" in l: continue
    m = re.match(r"0x([0-9A-Fa-f]{8})$", q[1].strip())
    t = m.group(1).lower() if m else None
    rows.append((i, q[0], t, q[2], q[4] if len(q) > 4 else ""))
out = open(sys.argv[1], "w")
for i, tag, t, good, wrong in rows:
    b = rb.get(t, []) if t else []
    goodb = [x for x in b if x[0] == good]; bad = [x for x in b if x[0] != good]
    st = "NOTARGET" if not t else ("CLEAN" if not bad and goodb else ("UNBOUND" if not b else ("WRONGONLY" if not goodb else "MIXED")))
    out.write("\t".join([str(i), tag, t or "-", st, good, " | ".join(f"{s}@{a}" for s, a in bad)]) + "\n")
