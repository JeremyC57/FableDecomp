#!/usr/bin/env python3
"""meta434.py <batch> <spec.tsv> <verdict> <reason>: lander files for a port batch (self unchanged): RESULTS, selfmap (identity),
SELF_SYMBOLS, UNITS, HEADERS (every hdr/engine/*.h: NEW or opt-in), empty RENAMES / CATALOG_CHANGES / TABLE_CHANGES, expect.tsv."""
import sys, os
W = "/tmp/claude-0/-home-user-FableDecomp/40effb6c-294b-5111-9a88-ca0c32f095ec/scratchpad/w22/agentNF436"
b, spec, verdict, reason = sys.argv[1:5]; B = W + "/batches/" + b
rows = [l.split("\t") for l in open(spec).read().splitlines() if l and not l.startswith("#")]
st = {}
for l in open("/home/user/FableDecomp/rebuild/oracles/self_symbols.tsv").read().splitlines()[1:]:
    f = l.split("\t"); st[f[0]] = l
with open(B + "/RESULTS.tsv", "w") as r, open(B + "/selfmap.tsv", "w") as m, open(B + "/SELF_SYMBOLS.tsv", "w") as s, open(B + "/UNITS.tsv", "w") as u:
    r.write("address\tverdict\told\tnew\treason\n"); s.write("address\tsymbol\tstatus\tleaf_given\tcurrent_line\n"); u.write("address\tunit\tkind\n")
    for f in rows:
        a, leaf = f[0], f[-1]; cur = st.get(a, "-")
        status = cur.split("\t")[2] if cur != "-" else "RELOCATION_MATCH"
        r.write("%s\t%s\t%s\t%s\t%s\n" % (a, verdict, leaf, leaf, reason.replace("{C}", f[1])))
        m.write("%s\t%s\t%s\n" % (a, leaf, leaf)); s.write("%s\t%s\t%s\t%s\t%s\n" % (a, leaf, status, leaf, cur)); u.write("%s\t%s\t-\n" % (a, a))
with open(B + "/HEADERS.tsv", "w") as h:
    h.write("header\topt_in\tinclude_users_in_batch\n")
    for x in sorted(os.listdir(B + "/hdr/engine")):
        new = not os.path.exists("/home/user/FableDecomp/rebuild/include/engine/" + x)
        users = [f[0] for f in rows if f[1] + ".h" == x] or ["(see rows)"]
        h.write("engine/%s\t%s\t%s\n" % (x, "NEW class header" if new else "add-only opt-in (see orig/hdr_%s)" % x, " ".join(users)))
open(B + "/RENAMES.tsv", "w").write("address\tmodule\tname\tmangled_leaf\told_catalog_name\told_leaf\tevidence\told_source\tnew_source\tnew_test_source\n")
open(B + "/CATALOG_CHANGES.tsv", "w").write("address\tfield\told\tnew\tnote\n")
open(B + "/TABLE_CHANGES.tsv", "w").write("action\tsymbol\ttarget\twitness\tnote\n")
open(B + "/expect.tsv", "w").write("address\told\tnew\n")
print("meta", b, len(rows))
