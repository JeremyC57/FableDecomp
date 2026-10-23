# Build a --recheck-landed payload + oracle for landed rows, straight from the catalog.
# usage: recheck.py <outdir> <addr|src-file>...   (a src path under rebuild/src/compiled is mapped to its row)
import re, sys, json, os
out = sys.argv[1]; want = sys.argv[2:]
ps = open("rebuild/build_candidates.ps1", encoding="utf-8-sig").read()
ent = {}
for blk in re.findall(r"\[pscustomobject\]@\{(.*?)\n    \}", ps, re.S):
    kv = dict(re.findall(r"(\w+) = '([^']*)'", blk))
    if "Address" in kv: ent[kv["Address"].lower()] = kv
bysrc = {v.get("Source"): a for a, v in ent.items()}
cat = {}
for l in open("rebuild/oracles/auto-re-candidates.tsv").read().splitlines()[1:]:
    q = l.split("\t"); cat[q[0].lower()] = q
BASE = ["/O2", "/Oy", "/W3"]
auth = []; orc = ["address\tname\tlength\tbytes"]
for w in want:
    a = w.lower()
    if a not in ent:
        rel = os.path.relpath(w, "rebuild/src/compiled").replace(os.sep, "/"); a = bysrc.get(rel, "")
    if a not in ent or a not in cat: print("skip", w); continue
    kv = ent[a]; src = open("rebuild/src/compiled/" + kv["Source"], encoding="utf-8").read()
    tst = open("rebuild/tests/" + kv["TestSource"], encoding="utf-8").read() if kv.get("TestSource") else ""
    q = cat[a]; name = q[1]; mod, leafname = name.rsplit("::", 1) if "::" in name else ("_global", name)
    m = re.search(r"^//\s*Leaf:\s*(\S+)", src, re.M)
    e = dict(address=a, module=kv.get("Module", mod), name=leafname, source_cpp=src, test_cpp=tst,
             pass_pattern=kv.get("PassPattern", f"{a}_TEST PASS"), leaf=m.group(1) if m else leafname)
    fl = kv.get("CompilerFlags", "").split()
    extra = [f for f in fl if f not in BASE] if fl else []
    if fl and "/Oy" not in fl and "/Oy-" not in extra: extra.append("/Oy-")
    if extra: e["flags"] = extra
    auth.append(e); orc.append("\t".join(q[:4]))
json.dump({"result": {"authored": auth}}, open(f"{out}/payload.json", "w"))
open(f"{out}/oracle.tsv", "w").write("\n".join(orc) + "\n")
print(len(auth), "rows")
